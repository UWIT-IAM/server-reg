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

/* server-reg tools */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <openssl/sha.h>

#include "util.h"
#include "webtpl.h"


/* browser and system type */

extern char *remote_user;


Browser get_browser()
{
   char *agent = getenv("HTTP_USER_AGENT");
   Browser B = (Browser) malloc(sizeof(Browser_));
   
   B->type = BR_XXX;
   B->version = 0;
   B->full_name = NULL;
   B->system = BS_XXX;

   if (agent) {
      B->full_name = strdup(agent);
      if (strstr(agent,"Opera")) B->type = BR_OPERA;
      else if (strstr(agent,"MSIE")) B->type = BR_IE;
      else if (strstr(agent,"Netscape")) B->type = BR_NS;
      else if (strstr(agent,"Chimera")) B->type = BR_CHIMERA;
      else if (strstr(agent,"OmniWeb")) B->type = BR_OMNIWEB;
      else if (strstr(agent,"Safari")) B->type = BR_SAFARI;
      else if (strstr(agent,"Gecko")) B->type = BR_GECKO;
   }

   if (agent) {
      if (strstr(agent,"Mac_Power")) B->system = BS_MAC;
      else if (strstr(agent,"Macintosh")) B->system = BS_MAC;
      else if (strstr(agent,"Windows")) B->system = BS_WIN;
      else if (strstr(agent,"Linux")) B->system = BS_LINUX;
   }

   return (B);
}

/* get configuration */

SRConfig sr_config;
int get_config(char *file)
{
   char rec[256];
   FILE *fp;

   sr_config = NULL;

   if (!(fp=fopen(file,"r"))) {
      return (errno);
   }

   sr_config = (SRConfig) malloc(sizeof(SRConfig_));
   memset(sr_config, 0, sizeof(SRConfig_));

   while (fgets(rec, 255, fp)) {
      char *k = rec;
      char *p;
      if (p=strchr(rec,'\n')) *p = '\0';
      while (*k && isspace(*k)) k++;
      if (*k=='#') continue;
      for (p=k; *p && (isalnum(*p)||(*p=='_')); p++);
      *p++ = '\0';
      while (*p && (isspace(*p)||(*p=='='))) p++;
      if (!strcmp(k,"eds_db")) sr_config->eds_db = strdup(p);
      if (!strcmp(k,"eds_user")) sr_config->eds_user = strdup(p);
      if (!strcmp(k,"eds_pw")) sr_config->eds_pw = strdup(p);
      if (!strcmp(k,"eds_from")) sr_config->eds_from = strdup(p);
      if (!strcmp(k,"eds_req_to")) sr_config->eds_req_to = strdup(p);
      if (!strcmp(k,"eds_app_to")) sr_config->eds_app_to = strdup(p);
      if (!strcmp(k,"eds_app_gds_to")) sr_config->eds_app_gds_to = strdup(p);
      if (!strcmp(k,"eds_app_pds_to")) sr_config->eds_app_pds_to = strdup(p);
   }
   fclose(fp);
   return (0);
}

/* read from a file into a string */

char *get_file(char *file, int *len)
{
   FILE *f = fopen(file,"r");
   char *str;
   long l;

   if (!f) return (NULL);
   fseek(f, 0, SEEK_END);
   l = ftell(f);
   rewind(f);

   str = (char*) malloc(l+1);
   fread(str, l, 1, f);
   fclose(f);

   str[l] = '\0';
   if (len) *len = l;

   return(str);
}

/* write a string to a file 
   returns true on success */

int put_file(char *file, char *str)
{
   FILE *f = fopen(file, "w");
   int r;
   r = fputs(str, f);
   return (r!=EOF);
}

int put_file_2(char *file, char *s1, char *s2)
{
   FILE *f = fopen(file, "w");
   int r;
   r = fputs(s1, f);
   r = fputs(s2, f);
   return (r!=EOF);
}

/* put a string to a temp file.  Returns the name
   of the file, or NULL if error. */

char *put_tmp_file(char *str, char *pfx)
{
   static char name[64];
   FILE *tf;
   sprintf(name,"%s/tmp/%s.%d",DIRROOT,pfx,getpid());
   unlink(name);
   tf = fopen(name, "w");
   if (tf) {
      fputs(str,tf);
      fclose(tf);
   }
   if (tf) return (name);
   else return (NULL);
}


/* webify a string. arrows goto codes */

char *util_webify(char *str)
{
   char *p;
   char *w;
   int n,c;

   for (n=0,c=0,p=str;*p;p++,n++) if (*p=='<' || *p=='>') c++;
   w = (char*) malloc(n+c*4+3);

   for (p=w;*str;str++) {
      if (*str=='<') strcpy(p,"&lt;"), p+=4;
      else if (*str=='>') strcpy(p,"&gt;"), p+=4;
      else *p++ = *str;
   }
   *p = '\0';
   return (w);

}

/* fix string as a query parameter - spaces only */

char *util_paramify(char *txt)
{
   char *ret;
   char *s;
   int nc = 1;;

   for (s=txt; *s; s++,nc++) if ((*s==' ')||(*s=='&')) nc+=4;
   ret = (char*) malloc(nc);
   for (s=ret; *txt; txt++) {
      if (*txt==' ') {
         *s++ = '%';
         *s++ = '2';
         *s++ = '0';
  /**
      } else if (*txt=='&') {
         *s++ = '%';
         *s++ = '2';
         *s++ = '6';
   **/
      } else *s++ = *txt;
   }
   *s = '\0';
   return (ret);
}


/* date strings */

char *date2mdy(time_t d)
{
   static char ret[12];
   struct tm *t;
   
   if (d) {
     t = localtime(&d);
     sprintf(ret, "%02d/%02d/%4d", t->tm_mon+1, t->tm_mday, t->tm_year+1900);
   } else strcpy(ret, "---");
   return(ret);
}

/* int strings */

char *int2dec(int i)
{
   static char ret[16];
   sprintf(ret, "%d", i);
   return(ret);
}

char *int2hex(int i)
{
   static char ret[16];
   sprintf(ret, "0x%x", i);
   return(ret);
}

/* cross site script provention */

/* This method generates a token unique to the input key.
   The token is added to forms and verified upon submission.
   We believe it is difficult to fake the token without
   knowing the encrypt seed (xsec_key). */

/* Get a token */

char *xsec_gen_token(char *key)
{
   int tk = time(NULL)/300;
   int i;
   static char tok[12];
   unsigned int v1=0;
   FILE *f;

   f = fopen(path_name("etc/xsec_key"), "r");
   if (f) { 
     fscanf(f, "%6d", &v1);
     fclose(f);
   }

   if (!key) key = getenv("REMOTE_USER");
   if (!key) key = getenv("REMOTE_ADDR");
   if (!key) key = "nokey";
   for (i=0;*key;key++,i++) v1+=(*key<<(i%24));

   srandom(v1);
   v1 = random() % 1000000000;
   sprintf(tok,"%0d", v1);
   return (tok);
}
   

/* Returns 0 if the token matches the key */

int xsec_chk_token(char *key, char *token)
{
   char *gk;
   if (!token) return (1);
   if (!key) key = getenv("REMOTE_USER");
   if (!key) key = getenv("REMOTE_ADDR");
   if (!key) key = "nokey";
   gk = xsec_gen_token(key);
   return (strcmp(gk,token));
}


/* Get a token */

#ifdef OLDWAY
char *xsec_gen_token2(char *k2)
{
   int i;
   char *k1 = remote_user;
   static char tok[24];
   unsigned int v1=0;
   char *seed = get_file(path_name(XSEC_KEY_FILE), NULL);

   if (!(seed && k1 && k2)) return ("noxsec");
   for (i=0;*seed;seed++,i++) v1+=(*seed<<(i%12));
   for (i=0;*k1;k1++,i++) v1+=(*k1<<(i%12));
   for (i=0;*k2;k2++,i++) v1+=(*k2<<(i%12));

   srandom(v1);
   v1 = random() % 1000000000;
   sprintf(tok,"%0d", v1);
   return (tok);
}
#else
char *xsec_gen_token2(char *k2)
{
   unsigned char h[SHA_DIGEST_LENGTH];
   char *tok = (char*) malloc(SHA_DIGEST_LENGTH*2 + 1);
   unsigned char *k;
   char *k1 = remote_user;
   char *seed = get_file(path_name(XSEC_KEY_FILE), NULL);
   int kl;
   int i,j;

   if (!(seed&&k1&&k2)) return (NULL);  /* invalid use */

   k = (char*) malloc(strlen(seed)+strlen(k1)+strlen(k2)+8);
   strcat(strcat(strcpy(k,seed),k1),k2);
   kl = strlen(k);
   SHA1(k, kl, h);
   for (i=0,j=0;i<SHA_DIGEST_LENGTH;i++) {
      unsigned int x;
      tok[j++] = ((x=h[i]%16)<10)?x+'0':x-10+'a';
      tok[j++] = ((x=h[i]/16)<10)?x+'0':x-10+'a';
   }
   tok[j] = '\0';
   free (k);
   return (tok);
}

#endif
   

/* check for key match */

int xsec_chk_token2(char *k2, char *token)
{
   char *gk;
   gk = xsec_gen_token2(k2);
   if (gk && token && !strcmp(gk, token)) return (1);
   return (0);
}

/* Set xsec form fields */
void xsec_set_valid_form(WebTemplate W)
{
    time_t now = time(NULL);
    char nowtxt[24];
    sprintf(nowtxt,"%d",now);
    WebTemplate_assign(W, "XTIME", nowtxt);
    WebTemplate_assign(W, "XSITE", xsec_gen_token2(nowtxt));
}

/* See if the xsec tokens from the form are valid */

int xsec_chk_valid_form(WebTemplate W)
{
    char *xtime = WebTemplate_get_arg(W, "xtime");
    char *xsite = WebTemplate_get_arg(W, "xsite");
    if (!(xtime&&xsite)) {
       WebTemplate_assign(W, "EMSG", "Invalid form.");
       return (0);
    }
    time_t exp = atoi(xtime) + XSEC_FORM_TIMEOUT;
    time_t now = time(NULL);
    if (now>exp) {
       WebTemplate_assign(W, "EMSG", "Expired form.");
       return (0);
    }
    if (!xsec_chk_token2(xtime, xsite)) {
       WebTemplate_assign(W, "EMSG", "Xsite form.");
       return (0);
    }
    return (1);

}


/* Send mail */

static char *wday[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};
static char *mon[] = {"Jan","Feb","Mar","Apr","May","Jun",
                      "Jul","Aug","Sep","Oct","Nov","Dec"};

int send_mail_cc(char *to, char *cc, char *sub, char *msg, char *from)
{
   char *mailer = (char*) malloc(strlen(from)+128);
   FILE *sm;
   char dat[24];
   struct tm *tt;
   time_t t;
   int tz;
   char *nl;
   char *rfp;
   char *rfrom;

   t = time(NULL);
   tt = localtime(&t);
   tz = timezone/3600;
   if (daylight) tz -= 1;
   
   if (nl=strchr(to,'\n')) *nl = 0;
   if (cc && (nl=strchr(cc,'\n'))) *nl = 0;
   if (nl=strchr(sub,'\n')) *nl = 0;
   if (nl=strchr(from,'\n')) *nl = 0;

   /* get the from address without the decorations */
   rfp = strdup(from);
   if (rfrom=strchr(rfp,'<')) {
      rfrom++;
      if (nl=strchr(rfrom,'>')) *nl = 0;
      /* should be error */
   } else rfrom = rfp;
   
   if (!msg) msg = "No message\n";

   sprintf(mailer, "/usr/sbin/sendmail -f %s -t >> /dev/null 2>&1", rfrom);
   sm = popen(mailer, "w");
   if (sm) {
      fprintf(sm,"Date: %s, %d %s %d %02d:%02d:%02d -%02d00\n",
                   wday[tt->tm_wday], tt->tm_mday, mon[tt->tm_mon],
                   tt->tm_year+1900, 
                   tt->tm_hour, tt->tm_min, tt->tm_sec, tz);
      fprintf(sm,"To: %s\n", to);
      if (cc&&*cc) fprintf(sm,"Cc: %s\n", cc);
      fprintf(sm,"From: %s\n", from);
      fprintf(sm,"Reply-To: %s\n", rfrom); 
      fprintf(sm,"Errors-To: %s\n", rfrom); 
      fprintf(sm,"X-Auto-Response-Suppress: NDR, OOF, AutoReply\n"); 
      fprintf(sm,"Precedence: Special-Delivery, never-bounce\n");
      fprintf(sm,"Subject: %s\n%s\n", sub, msg);
      pclose(sm);
      free(rfp);
      return (1);
   }
   return (0);
}

int send_mail(char *to, char *sub, char *msg, char *from)
{
   return (send_mail_cc(to, NULL, sub, msg, from));
}


/* Make a full path from a relative one */

char *path_name(char *file)
{
   char *path, *app;
   char *uri = (char*) getenv("REQUEST_URI");
    
   path = (char*) malloc( strlen(DIRROOT) + 64 + strlen(file));
   app = "/";
   if (uri && strstr(uri, "/pubcookie")) app = "/pubcookie/";
   else if (uri && strstr(uri, "/pbc")) app = "/pbc/";
   else if (uri && strstr(uri, "/groups")) app = "/groups/";
   else if (uri && strstr(uri, "/shib")) app = "/shib/";
   else if (uri && strstr(uri, "/eds2")) app = "/eds2/";
   else if (uri && strstr(uri, "/arp")) app = "/arp/";
   strcat(strcat(strcpy(path,DIRROOT), app), file);
   return (path);
}

/* Make a base path from a relative one */

char *base_path_name(char *file)
{
   char *path;
    
   path = (char*) malloc( strlen(DIRROOT) + 4 + strlen(file));
   strcat(strcat(strcpy(path,DIRROOT), "/"), file);
   return (path);
}

int write_log(char *name, char *user, char *action, char *cn)
{
   FILE *logfile = fopen(path_name(name),"a");
   if (logfile) {
      time_t now = time(NULL);
      struct tm *t;
      t = localtime(&now);
      fprintf(logfile,
        "%04d/%02d/%02d %02d:%02d:%02d user=%s, action=%s, cn=%s\n",
         t->tm_year+1900, t->tm_mon+1, t->tm_mday,
         t->tm_hour, t->tm_min, t->tm_sec,
          user, action, cn);
      fclose(logfile);
      return (1);
    }
    return (0);
}

/* convert a string to lower case */

char *lower_case(char *str)
{
  char *p;
  if (!str) return (NULL);
  for (p=str;*p;p++) if (isupper(*p)) *p = tolower(*p);
  return (str);
}


/* Add a crumb to a page */

void add_crumb(WebTemplate W, char *path, char *args, char *name)
{
  char link[512];
  snprintf(link, 512, "%s%s%s", (path?path:""), (args?"?":""),
                  (args?util_paramify(args):""));
  WebTemplate_assign(W, "CRUMBLINK", link);
  WebTemplate_assign(W, "CRUMBNAME", name);
  WebTemplate_parse_dynamic(W, "page.crumb.sep");
  WebTemplate_parse_dynamic(W, "page.crumb");
}

void crumb_home(WebTemplate W)
{
  WebTemplate_parse_dynamic(W, "page.crumbsr");
}

void add_crumb_help(WebTemplate W, char *link)
{
   WebTemplate_assign(W, "CRUMBLINK", util_paramify(link));
   WebTemplate_assign(W, "CRUMBNAME", "Help");
   WebTemplate_parse_dynamic(W, "page.crumbhelp");
}

