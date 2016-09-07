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

/* CA utility: test domain */

#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "webtpl.h"
#include "ckcn.h"

extern char *remote_user;
extern int errno;
char *errstr = NULL;


#define FULLREPORT


char **netact_get_host_ids(char *name);
char **netact_get_domain_ids(char *name);


main(int argc, char **argv)
{
  char *arg;
  char *dn;
  char *rsp;
  char **list;
  char *dp,*edp;
  char *id = NULL;
  WebTemplate W = newWebTemplate();


  if (argc==2) {
     dn = argv[1];
  } else if (argc==3) {
     dn = argv[1];
     id = argv[2];
  } else {
     fprintf(stderr,"usage: %s dns_name\n", argv[0]);
     exit (1);
  }

  if (id) {
     rsp = chk_netacts(dn, strlen(dn), id);
     if (rsp) printf(" %s\n", rsp);
     else printf(" %s is OK for %s\n", id, dn);
     exit (0);
  }

  if (dn && *dn) {
     char **onrs;
     int nown;
  
#ifdef FULLREPORT
     /* show dns */

     rsp = chk_dns(dn, 0);
     if (!rsp) printf("dns: OK\n");
     else printf("dns: %s\n", rsp);
    
     /* show host */

     list = netact_get_host_ids(dn);
     if (list && *list) {
        while (*list) {
           printf("host: %s owned by %s\n", dn, *list);
           list++;
        }
     }
    

     /* Show subdomains */

     if (edp=strstr(dn,".washington.edu")) {
        for (dp=strchr(dn,'.');dp&&dp<edp;dp=strchr(dp+1,'.')) {
          list = netact_get_domain_ids(dp+1);
          if (list && *list) {
            while (*list) {
              printf("subdomain: %s owned by %s\n", dp+1, *list);
              list++;
            }
          }
        }
     }
#endif

    nown = 0;
    onrs = get_dns_owners(W, dn, &nown);
    printf("%d owners:\n", nown);
    for (; onrs&&*onrs; onrs++) printf(" .. %s\n", *onrs);
    printf("\n");
  }

           
}

