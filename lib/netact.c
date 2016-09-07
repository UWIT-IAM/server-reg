/* library to test ownership of a host or domain */

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#define HOST_URL "https://umbra.cac.washington.edu/daw/json/DNS/v1/UWNetidsFromFQDN/fqdn/"
#define DOMAIN_URL "https://umbra.cac.washington.edu/daw/json/Net-Contacts/v1/UWNetidsFromDomain/domain/"


/* --- StrList tools --------------------

   A StrList is a list of char*.  The pos and len allow
   the list to grow and to be reused without concern for
   memory allocation.
 */

typedef struct StrList__ {
  int pos;
  int len;
  char **txt;
  int inc;
} StrList_, *StrList;

/* create a string list */
StrList newStrList()
{
  StrList S = (StrList) malloc(sizeof(StrList_));
  S->pos = 0;
  S->len = 10;
  S->inc = 1000;
  S->txt = (char**) malloc(S->len*sizeof(char*));
  return (S);
}

/* add a string to a list */
void addStrList(StrList S, char *str)
{
  int i;
  if (!str) return;

  // ignore dup value
  if (inStrList(S, str)>=0) return;

  S->txt[S->pos++] = strdup(str);
  if (S->pos>=S->len) {
     S->len += S->inc;
     S->txt = (char**) realloc(S->txt, S->len*sizeof(char*));
  }
}

/* remove strings from a list */
void clearStrList(StrList S)
{
  int i;
  if (S) {
    for (i=0;i<S->pos;i++) if (S->txt[i]) free(S->txt[i]);
    S->pos = 0;
  }
}

/* free a list */
void freeStrList(StrList S)
{
   if (!S) return;
   clearStrList(S);
   free(S->txt);
   free(S);
}

/* find a string */
int inStrList(StrList S, char *s) {
   int i;
   for (i=0;i<S->pos;i++) {
      if (!S->txt[i]) continue;
      if (strcasecmp(S->txt[i], s)) continue;
      return(i);
   }
  return (-1);
}
   

struct MemoryStruct {
  char *memory;
  size_t size;
};
 
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
  size_t realsize = size * nmemb;
  struct MemoryStruct *mem = (struct MemoryStruct *)userp;
 
  mem->memory = realloc(mem->memory, mem->size + realsize + 1);
  memcpy(&(mem->memory[mem->size]), contents, realsize);
  mem->size += realsize;
  mem->memory[mem->size] = 0;
  return realsize;
}
   
  
/* Add owners to the list of owners */
int add_owners(StrList owners, char *url) {
   CURL *curl;
   CURLcode res;
   struct MemoryStruct data;
   data.memory = malloc(1);
   data.size = 0;

   curl_global_init(CURL_GLOBAL_DEFAULT);
   curl = curl_easy_init();
   curl_easy_setopt(curl, CURLOPT_URL, url);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
   curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&data);
   res = curl_easy_perform(curl);
   if (res==CURLE_OK) {
      // poor man's json parser
      // printf("got: %s\n", data.memory);
      char *p = strstr(data.memory, "\"row\":");
      char *q, *r;
      if (p) {
         p += 6;
         while (q=strstr(p, "\"uwnetid\"")) {
            q += 11;
            if (r=strchr(q, '"')) {
                *r = '\0';
                addStrList(owners, q);
                p = r+1;
            } else break;
         }
      }
      
   } else {
      printf ("error\n");
   }
   curl_easy_cleanup(curl);
   curl_global_cleanup();
 
   return (1);
}

/* test if a host is controlled by the specified id */

int netact_add_host(StrList owners, char *host)
{
  char buf[1024];
  snprintf(buf, 1024, "%s%s", HOST_URL, host);

  add_owners(owners, buf); 
  return (0);
}

int netact_add_domain(StrList owners, char *host)
{
  char buf[1024];
  snprintf(buf, 1024, "%s%s", DOMAIN_URL, host);

  add_owners(owners, buf); 
  return (0);
}

/* test if a host is controlled by the specified id */

int netact_test_host(char *name, char *id)
{
   int ret = 0;
   StrList owners = newStrList();
   netact_add_host(owners, name);
   if (inStrList(owners, id)>=0) ret = 1;
   freeStrList(owners);
   return ret;
}

int netact_test_domain(char *name, char *id)
{
   int ret = 0;
   StrList owners = newStrList();
   netact_add_domain(owners, name);
   if (inStrList(owners, id)>=0) ret = 1;
   freeStrList(owners);
   return ret;
}


/* test if a name is controlled by the specified id */

int netact_test_name(StrList owners, char *name, char *id)
{
   char *edp, *dp;

   /* First test as hostname */

   if (netact_test_host(name,id)) return (1);

   /* Else try domains */

   if (!(edp=strrchr(name,'.'))) return (0);
   if (!(edp=strrchr(edp-1,'.'))) return (0);

   for (dp=strchr(name,'.');dp&&dp<edp;dp=strchr(dp+1,'.')) {
      if (netact_test_domain(dp+1, id)) return (1);
   }
   return (0);
}

/* get owners for a host */

char **netact_get_host_ids(char *host)
{
  char **ret, **p;
  int i;
  StrList owners = newStrList();
  netact_add_host(owners, host);
  ret = (char**) malloc(sizeof(char*) * ((owners->pos)+1));
  p = ret;
  for (i=0;i<owners->pos;i++) {
     *p++ = strdup(owners->txt[i]);
  }
  *p = NULL;
  return (ret);
}

/* get owners for a domain */

char **netact_get_domain_ids(char *domain)
{
  char **ret, **p;
  int i;
  StrList owners = newStrList();
  netact_add_domain(owners, domain);
  ret = (char**) malloc(sizeof(char*) * ((owners->pos)+1));
  p = ret;
  for (i=0;i<owners->pos;i++) {
     *p++ = strdup(owners->txt[i]);
  }
  *p = NULL;
  return (ret);
}


#ifdef __DO_MAIN__
int main() {
   StrList owners = newStrList();
   netact_test_name(owners, "x315.cac.washington.edu", "fox");

   char **p = netact_get_host_ids("x315.cac.washington.edu");
   while (*p) {
      printf(">%s\n", *p);
      p++;
   }
   
   p = netact_get_domain_ids("cac.washington.edu");
   while (*p) {
      printf(">%s\n", *p);
      p++;
   }
   
   exit (1);
}
#endif
