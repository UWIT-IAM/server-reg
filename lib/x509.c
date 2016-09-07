/* x509 tools */


#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <time.h>

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


/* from openssl x509 code */
/* Get a normal integer from an asn1 integer */

int i2i_ASN1_INTEGER(ASN1_INTEGER *a)
{
  int i;
  int h1, h2;
  int ret = 0;

  for (i=0;i<a->length;i++) {
     h1 = ((unsigned char)a->data[i]>>4)&0x0f;
     h2 = ((unsigned char)a->data[i]   )&0x0f;
     ret = ret*256 + h1*16 + h2;
  }
  return (ret);
}


/* ASN1 to time_t ( from coded-out part of a_utctm.c) */

#define g2(p) (((p)[0]-'0')*10+(p)[1]-'0')
time_t ASN1_UTCTIME_get(ASN1_UTCTIME *s)
{
   struct tm tm;
   int offset;

   memset(&tm,'\0',sizeof tm);

   tm.tm_year=g2(s->data);
   if(tm.tm_year < 50) tm.tm_year+=100;
   tm.tm_mon=g2(s->data+2)-1;
   tm.tm_mday=g2(s->data+4);
   tm.tm_hour=g2(s->data+6);
   tm.tm_min=g2(s->data+8);
   tm.tm_sec=g2(s->data+10);
   if(s->data[12] == 'Z') offset=0;
   else {
      offset=g2(s->data+13)*60+g2(s->data+15);
      if(s->data[12] == '-')
              offset= -offset;
   }
   return (mktime(&tm)-offset*60);
}
#undef g2

/* --------- end of openssl x509 extraction ----------- */


char *cert_start = "-----BEGIN CERTIFICATE-----\n";
char *cert_end = "\n-----END CERTIFICATE-----";

/* Get a cert from a string */

X509 *ssl_txt2crt(char *str)
{
  X509 *crt=NULL;
  BIO *cbio;
  char *p;
  char *pemstr;

  /* make sure cert has begin-end */
  if (strncmp(str,cert_start,strlen(cert_start))) {
     pemstr = (char*) malloc(strlen(cert_start)+strlen(str)+strlen(cert_end));
     for (p=str;*p&&((*p=='\n')||isspace(*p));p++);
     strcat(strcpy(pemstr,cert_start),p);
     for (p=pemstr;*p;p++);
     for (p--;(p>pemstr)&&((*p=='\n')||isspace(*p));p--);
     strcpy(++p,cert_end);
  } else {
     pemstr = str;
  }

  cbio = BIO_new_mem_buf(pemstr, strlen(pemstr));
  if (cbio==NULL) {
    ERR_print_errors_fp(stderr);
    return (NULL);
  }

  if(!(crt = PEM_read_bio_X509(cbio, NULL, NULL, NULL))) {
      fprintf(stderr, "Can't parse cert [%s]\n", pemstr);
      ERR_print_errors_fp(stderr);
      return (NULL);
  }
  return (crt);
}


/* Get the DN from a cert */

char *ssl_get_dn(X509 *crt)
{
  X509_NAME *subj;
  subj = X509_get_subject_name(crt);
  return (X509_NAME_oneline(subj,NULL,0));
}

/* Get the issuer from a cert */

char *ssl_get_issuer(X509 *crt)
{
  X509_NAME *issuer;
  issuer = X509_get_issuer_name(crt);
  return (X509_NAME_oneline(issuer,NULL,0));
}

/* Get the serial from a cert */

int ssl_get_sn(X509 *crt)
{
  X509_NAME *subj;
  int sn;
  sn = i2i_ASN1_INTEGER(crt->cert_info->serialNumber);
  return (sn);

}

/* Get the issue date from a cert */

time_t ssl_get_issue(X509 *crt)
{
  X509_NAME *subj;
  ASN1_UTCTIME *str;
  str = X509_get_notBefore(crt);
  return (ASN1_UTCTIME_get(str));
}


/* Get the expire date from a cert */

time_t ssl_get_expire(X509 *crt)
{
  X509_NAME *subj;
  ASN1_UTCTIME *expstr;
  expstr = X509_get_notAfter(crt);
  return (ASN1_UTCTIME_get(expstr));
}

 

