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

/* C&C PC client -> keyserver.

   Contacts remote server to do stuff.

   by Fox, Sep 2003

*/


#include <stdio.h>
#include <string.h>

#include <openssl/bio.h>
#include <openssl/err.h>
#include <openssl/rand.h>
#include <openssl/ssl.h>
#include <openssl/x509v3.h>


#include "ks.h"

int  kscmd_debug = 0;


static void ERR(char *msg)
{
   fprintf(stderr,"ERR: %s:%d %s\n", __FILE__,__LINE__,msg);
   ERR_print_errors_fp(stderr);
   exit (1);
}

/* SSL setup and connection tools */

int verify_callback(int ok, X509_STORE_CTX *store)
{
    char data[256];
 
#ifdef DOINGCERTCHECK
    if (!ok) {
      X509 *cert = X509_STORE_CTX_get_current_cert(store);
      int  depth = X509_STORE_CTX_get_error_depth(store);
      int  err = X509_STORE_CTX_get_error(store);
 
      fprintf(stderr, "-Error with certificate at depth: %i\n", depth);
      X509_NAME_oneline(X509_get_issuer_name(cert), data, 256);
      fprintf(stderr, "  issuer   = %s\n", data);
      X509_NAME_oneline(X509_get_subject_name(cert), data, 256);
      fprintf(stderr, "  subject  = %s\n", data);
      fprintf(stderr, "  err %i:%s\n", err, X509_verify_cert_error_string(err));
    }
    return ok;
#endif
    return 1;
}

SSL_CTX *setup_client_ctx(void)
{
    SSL_CTX *ctx;

    /* Keyserver insists on the TLSv1 method */
    ctx = SSL_CTX_new(TLSv1_client_method()); 
    if (SSL_CTX_load_verify_locations(ctx, DIRROOT "/" CA_CERT, NULL) != 1)
        ERR("Error loading PC file and/or directory");
    if (SSL_CTX_set_default_verify_paths(ctx) != 1)
        ERR("Error loading default PC file and/or directory");
    if (SSL_CTX_use_certificate_chain_file(ctx, DIRROOT "/" CL_CERT) != 1)
        ERR("Error loading certificate from file");
    if (SSL_CTX_use_PrivateKey_file(ctx, DIRROOT "/" CL_CERT, SSL_FILETYPE_PEM) != 1)
        ERR("Error loading private key from file");
    SSL_CTX_set_verify(ctx, SSL_VERIFY_PEER, verify_callback);
    SSL_CTX_set_verify_depth(ctx, 1);
    return ctx;
}




/* Verify that the cert's cn is something we expect */

long cert_cn_check(SSL *ssl, char *cn)
{
   X509      *cert = NULL;
   X509_NAME *subj;
   char      data[256];
   int       extcount;
   int       ok = 0;
 
   if ((cert=SSL_get_peer_certificate(ssl)) && cn) {

     if ((subj=X509_get_subject_name(cert)) &&
         X509_NAME_get_text_by_NID(subj, NID_commonName, data, 256) > 0) {
       data[255] = 0;
       if (strcasecmp(data, cn) == 0) {
          X509_free(cert);
          return SSL_get_verify_result(ssl);
       }
     }
   }
   if (cert) X509_free(cert);
   return X509_V_ERR_APPLICATION_VERIFICATION;
}


/* This only works on linux */
void seed_prng(void)
{
  time_t t = time(NULL);
  RAND_load_file("/dev/urandom", 1024);
  RAND_seed((unsigned char *)&t, sizeof(t));
  t = time(NULL);
  RAND_seed((unsigned char *)&t, sizeof(t));
  RAND_seed((unsigned char *)&t, sizeof(t));
}


/* ------------------------------------------------- */




/* These defaults can be altered before calling ca_cmd */
char *cahost = KS_HOST;
char *caport = KS_PORT;

int do_cert_cn_check = 1;

int ks_cmd(char *cmd, char *cn, char *test, char **rettxt) 
{

   int err;
 
   BIO     *conn;
   SSL     *ssl;
   SSL_CTX *ctx;

   int castatus = 0;
   struct timeval tv;
   char *s, *e, *p;
   int sslerr;
   int content_length = 0;
   int rspl;
   char caserver[256];
   char rsp[4096];
   int n;

   if (!test) test = "";
   sprintf(caserver,"%s:%s", cahost, caport);

   /* Setup ssl connection */

    if (!SSL_library_init()) ERR("OpenSSL initialization failed!");
    SSL_load_error_strings();
    seed_prng(  );

    ctx = setup_client_ctx(  );

    conn = BIO_new_connect(caserver);
    if (!conn) ERR("Error creating connection BIO");

    if (BIO_do_connect(conn)<=0) {
      ERR("Error connecting to server");
      ERR_print_errors_fp(stderr);
    }

    ssl = SSL_new(ctx);
    SSL_set_bio(ssl, conn, conn);
    if (SSL_connect(ssl) <= 0) ERR("Error connecting SSL object");
    if (do_cert_cn_check && (err = cert_cn_check(ssl, cahost)) != X509_V_OK)
    {
        if (0) fprintf(stderr, "-Error: peer certificate: %s\n", X509_verify_cert_error_string(err));
        // ERR("Error checking cn after connection");
    }

   /* Send the command and get the response */

   sprintf(rsp,"GET https://%s/?genkey=%s?setkey=%s;%s\r\n\r\n",
         caserver, cmd, cn, test);
         
   err = SSL_write(ssl, rsp, strlen(rsp));
   if (err <= 0) return 1;

   p = rsp;
   *p = '\0';
   for (;;) {
      n = SSL_read(ssl,rsp, sizeof(rsp)-(p-rsp));
      if (n<0) return (5);
      if (n==0) break;
      p += n;
      *p = '\0';
   }
   
   castatus = 1;
   if (strncmp(rsp,"OK",2)) castatus = 0;

   if (rettxt) *rettxt = strdup(rsp);
   return (castatus);
}


