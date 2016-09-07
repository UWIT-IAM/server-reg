/* ========================================================================
 * Copyright (c) 2005 The University of Washington
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ========================================================================
 */

/* host ownership tools */

#include <stdio.h>
#include <time.h>
#include <netdb.h>
#include <resolv.h>
#include <string.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/ssl.h>
#include <openssl/buffer.h>
#include <openssl/objects.h>
#include <openssl/evp.h>
#include <openssl/rand.h>
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>
#include <openssl/pkcs12.h>
#include <openssl/des.h>

#include "webtpl.h"

extern char *remote_user;
char **netact_get_host_ids(char *name);
char **netact_get_domain_ids(char *name);

/* Make sure no special chare in dns name */

void fix_dns_name(char *n)
{
   for (;*n;n++) {
      if (isupper(*n)) *n = tolower(*n);
      if (isalnum(*n)||(*n=='.')||(*n=='-')||(*n=='*')) continue;
      *n = '_';
   }
}


/* Verify that the user is an owner of the cn.
   Return NULL on OK, else return an error message. */

char *chk_netacts(char *cn, int lcn, char *id) {
  char *hdom;
  char *msg = NULL;
  int r;
  if (lcn==0) lcn = strlen(cn);
  hdom = (char*) malloc(lcn+1);
  memcpy(hdom,cn,lcn);
  hdom[lcn] = '\0';
  fix_dns_name(hdom);
  r = netact_test_host(hdom, id);
  if (!r) {
     char *edp, *dp;
     /* maybe owns the domain */
     if (edp=strrchr(hdom,'.')) {
        for (dp=hdom;dp&&dp<edp;dp=strchr(dp,'.')+1) {
          if (r=netact_test_domain(dp, id)) break;
        }
     }
     if (!r) {
       msg = (char*) malloc(lcn+64);
       sprintf(msg,"CN: %s is not an owner of the host '%s'", id, hdom);
     }
  }
  if ((!msg) && strchr(hdom,'*')) {
     msg = (char*) malloc(lcn+48);
     sprintf(msg,"WC: wildcard domains require administrator approval");
  }
  free(hdom);
  return (msg);
}


/* Verify that a cn is a valid DNS name.
   Accept any record.
   Allow "*." wildcard.
   Return NULL on OK, else return an error message. */

char *chk_dns(char *cn, int lcn) {
  char *msg = NULL;
  char rbuf[PACKETSZ];
  char *hdom;
  int r;

  if (lcn==0) lcn = strlen(cn);
  hdom = (char*) malloc(lcn+2);
  if (strncmp(cn,"*.",2)) memcpy(hdom,cn,lcn);
  else {
    lcn -= 2;
    memcpy(hdom,cn+2,lcn);
  }
  hdom[lcn] = '.';
  hdom[lcn+1] = '\0';
  fix_dns_name(hdom);
  res_init();
  r = res_search(hdom, C_IN, T_ANY, rbuf, PACKETSZ);
  if (r<0) {
    msg = (char*) malloc(lcn+48);
    sprintf(msg,"DNS domain '%s' not found", hdom);
  }
  free(hdom);
  return (msg);
}


/* Fill out the dn helper form ('reply') */

void show_dn_help(WebTemplate W, char *dn)
{
   char **list;
   char *dp, *edp;
   int nown;

     /* Check for valid domain name */

     if (chk_dns(dn,0)) {
        WebTemplate_parse_dynamic(W, "reply.nodns");

     /* Check netops */

     } else if (netact_test_name(dn, remote_user)) {
        WebTemplate_parse_dynamic(W, "reply.ok");


     /* Show who's got what */

     } else {

        nown = 0;
        WebTemplate_assign(W, "SDN", dn);
        list = netact_get_host_ids(dn);
        if (list && *list) {
           while (*list) {
              WebTemplate_assign(W, "ID", *list);
              WebTemplate_parse_dynamic(W, "reply.notok.host.owners.id");
              list++;
              nown++;
           }
           if (nown>1) WebTemplate_parse_dynamic(W, "reply.notok.host.owners.many");
           else WebTemplate_parse_dynamic(W, "reply.notok.host.owners.one");
           WebTemplate_parse_dynamic(W, "reply.notok.host.owners");
        } else WebTemplate_parse_dynamic(W, "reply.notok.host.noowner");
        WebTemplate_parse_dynamic(W, "reply.notok.host");


        /* Show subdomains */

        if (edp=strstr(dn,".washington.edu")) {
           for (dp=strchr(dn,'.');dp&&dp<edp;dp=strchr(dp+1,'.')) {
             nown = 0;
             WebTemplate_assign(W, "SDN", dp+1);
             list = netact_get_domain_ids(dp+1);
             if (list && *list) {
               while (*list) {
                 WebTemplate_assign(W, "ID", *list);
                 WebTemplate_parse_dynamic(W, "reply.notok.domain.owners.id");
                 list++;
                 nown++;
               }
               if (nown>1) WebTemplate_parse_dynamic(W, "reply.notok.domain.owners.many");
               else WebTemplate_parse_dynamic(W, "reply.notok.domain.owners.one");
               WebTemplate_parse_dynamic(W, "reply.notok.domain.owners");
             } else WebTemplate_parse_dynamic(W, "reply.notok.domain.noowner");
             WebTemplate_parse_dynamic(W, "reply.notok.domain");
           }
        }
        WebTemplate_parse_dynamic(W, "reply.notok");
     }
}

/* get a list of all owners of a domain */

static int dns_sorter(const void *a, const void *b)
{
   return (strcmp(*(char**)a, *(char**)b));
}

char **get_dns_owners(WebTemplate W, char *dn, int *nowner)
{
   char **list, **owners;
   char **lp, **op;
   char *dp, *edp;
   int nown;
   int n;

  /* Check for valid domain name */

  if (chk_dns(dn,0)) {
     WebTemplate_assign(W, "EMSG", "invalid dns name");
     return (NULL);
  }

  /* Check host ownership */

  nown = 0;
  owners = netact_get_host_ids(dn);
  if (owners) {
     for (op=owners; *op; nown++,op++);
  }

  /* Add subdomains */

  if (edp=strchr(dn,'.')) {
     for (dp=dn;dp&&dp<edp+2;dp=strchr(dp,'.')+1) {
        list = netact_get_domain_ids(dp);
        if (!list) continue;
        for (n=0,lp=list; *lp; n++,lp++);
        nown += n;
        owners = (char**) realloc(owners, sizeof(char**)*(nown+2));
        for (op=owners; *op; op++);
        for (lp=list; *lp; lp++) *op++ = *lp;
        free(list);
        *op = NULL;
     }
  }

  // let's get rid of the dups
  qsort(owners, nown, sizeof(char*), dns_sorter);
  for (nown=0,op=owners,lp=owners; *lp; lp++) {
     if ( (op==owners) || (strcmp(*(op-1), *lp))) *op++=*lp, nown++;
  }
  *op = NULL;

  if (nowner) *nowner = nown;
  return (owners);
}
