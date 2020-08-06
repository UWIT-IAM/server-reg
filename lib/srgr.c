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

/* Update a group membership */

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "openssl/ssl.h"
#include "openssl/x509.h"
#include "openssl/x509v3.h"
#include "openssl/err.h"

#include <curl/curl.h>

#include "srgr.h"
#include "srext.h"

char curl_error_buffer[CURL_ERROR_SIZE];

static void setrsp(char **rsp, char *msg)
{
   if (rsp) {
     *rsp = (char*) malloc(strlen(msg) + 32);
     sprintf(*rsp, "Err=\"%s\", errno=%d", msg, errno);
   }
}

/* SSL verify callback.
   If verifying, check that the cert name matches the peer name.
   Return 1 if OK. */

char *static_peer_name;
int static_debug;
static int sslverify_callback(int ok, X509_STORE_CTX *ctx)
{
   X509 *peercert;
   char cn[256];
   STACK_OF(GENERAL_NAME) *altnams;
   int i, na, r;
   int d =  X509_STORE_CTX_get_error_depth(ctx);
   
   peercert=X509_STORE_CTX_get_current_cert(ctx);
   X509_NAME_get_text_by_NID (X509_get_subject_name(peercert), NID_commonName, cn, sizeof(cn));

   if (!ok) {
      int err = X509_STORE_CTX_get_error(ctx);
      if (static_debug) printf("cert verify: d=%d, dn=%s, err=%d %s\n",
                  d, cn, err, X509_verify_cert_error_string(err));
      return (ok);
   }
   if (d) return (ok);
  
   /* Else check altnames first */

   altnams = X509_get_ext_d2i(peercert, NID_subject_alt_name, NULL, NULL);
   na = sk_GENERAL_NAME_num(altnams);
   for (i=0; i<na; i++) {
      char *altn;
      GENERAL_NAME *ck = sk_GENERAL_NAME_value(altnams, i);
      if (ck->type != GEN_DNS) continue;
      altn = (char *)ASN1_STRING_data(ck->d.ia5);
      if (!strcmp(altn, static_peer_name)) break;
   }
   GENERAL_NAMES_free(altnams);

   if (i<na) return (1); /* name ok */
   if (na<0) {  /* RFC2459: altnames must be used if present */
      if (!strcmp(cn,static_peer_name)) return (1);
   }

   /* make up an error */
   if (static_debug>1) printf("error, peer CN does not match peer name\n");
   SSLerr(SSL_F_SSL_VERIFY_CERT_CHAIN, SSL_R_PEER_ERROR_CERTIFICATE);
   return (0);
}



static CURLcode curl_ctx_callback(CURL *curl, void *ctx, void *parm)
{
   static_peer_name = (char*) parm;
   SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, sslverify_callback);
   return (0);
}


/* curl text reciever.  for now just ignore. */

static size_t page_reader(void *buf, size_t len, size_t num, void *wp)
{
  return (len*num);
}



/* perform GWS action */

int gr_action(char *grp, char *cn, char **rsp, int rm) 
{
   char prsp[512];
   char url[1024];
   char *emsg = NULL;
   char *msg = NULL;
   int ret;

   CURL *curl = curl_easy_init();;
  
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0);
   // curl_easy_setopt(curl, CURLOPT_SSL_CTX_FUNCTION, curl_ctx_callback);

   curl_easy_setopt(curl, CURLOPT_SSLCERT, CRTFILE);
   curl_easy_setopt(curl, CURLOPT_SSLCERTTYPE, "PEM");
   curl_easy_setopt(curl, CURLOPT_SSLKEY, KEYFILE);
   curl_easy_setopt(curl, CURLOPT_SSLKEYTYPE, "PEM");
   // curl_easy_setopt(curl, CURLOPT_CAINFO, CAFILE);

   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, page_reader);
   curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, curl_error_buffer);

   // curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
   if (rm) curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "DELETE");
   else curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, "PUT");

   sprintf(url, "https://iam-ws.u.washington.edu/group_sws/v1/group/%s/member/%s", grp, cn);
   curl_easy_setopt(curl, CURLOPT_URL, url);

   ret = curl_easy_perform(curl);

   if (ret) {
      char *s = curl_error_buffer;
      if (rsp) *rsp = s;
      return (ret);
   }

   return (1);
}

/* Add a cn to the indicated group */
int gr_add(char *grp, char *cn, char **rsp) 
{
   gr_action(grp, cn, rsp, 0);
}

/* Remove a cn from the indicated group */
int gr_remove(char *grp, char *cn, char **rsp) 
{
   gr_action(grp, cn, rsp, 1);
}
