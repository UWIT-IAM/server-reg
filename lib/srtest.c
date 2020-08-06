/* server-reg ldap tools tester */

#include <stdio.h>

#include <ldap.h>
#include <lber.h>
#include <sasl/sasl.h>
#include <sys/time.h>

#include "srext.h"
#include "uwldap.h"

#define PRINTF if (1) printf

char *prog;

void usage() 
{
   fprintf(stderr, "usage: %s [-CA cafile] [-C certfile] [-K keyfile]\n");
   fprintf(stderr, "        [-g group -u userid]\n");
   fprintf(stderr, "        [-u userid]\n");
   exit (9);
}


main(int argc, char**argv)
{
  void *L;
  char **types;
  char *id = "fox";
  char *grp = NULL;
  char *crs = NULL;
  char *crsonr = NULL;
  char *cafile = CAFILE;
  char *crtfile = CRTFILE;
  char *keyfile = KEYFILE;
  int r;
  int rc;

  prog = argv[0];
  while (--argc) {
     argv++;
     if (!strcmp(argv[0],"-CA")) {
        if (--argc<=0) usage();
        cafile = (++argv)[0];
     } else if (!strcmp(argv[0],"-C")) {
        if (--argc<=0) usage();
        crtfile = (++argv)[0];
     } else if (!strcmp(argv[0],"-K")) {
        if (--argc<=0) usage();
        keyfile = (++argv)[0];
     } else if (!strcmp(argv[0],"-g")) {
        if (--argc<=0) usage();
        grp = (++argv)[0];
     } else if (!strcmp(argv[0],"-u")) {
        if (--argc<=0) usage();
        id = (++argv)[0];
     }
  }

  if (!(id&&crtfile&&cafile)) usage();


      PRINTF("Connecting via to %s for name check\n", UWLDAP_EDS_HOST);
      L = uwldap_connect(UWLDAP_EDS_HOST, crtfile, keyfile, cafile);

      rc = 0;

      if (L) {
         char *name;
         PRINTF("..initialize OK\n");
         
         name = uwldap_get_username(L,id);
            if (name) PRINTF("... %s\n", name);
    
      } else {
         PRINTF("..initialize failed.\n");
      }
      uwldap_disconnect(L);


  if (grp) {

      PRINTF("Connecting via to %s for group check\n", UWLDAP_GDS_HOST);
      L = uwldap_connect(UWLDAP_GDS_HOST, crtfile, keyfile, cafile);

      rc = 0;

      if (L) {
         PRINTF("..initialize OK\n");
         
         if (grp && id) {
            PRINTF("..checking if %s is in %s\n", id, grp);
            rc = uwldap_is_member(L, id, grp);
            PRINTF("... %s\n", rc?"yes":"no");
         }
    
      } else {
         PRINTF("..initialize failed.\n");
      }
      uwldap_disconnect(L);
  }
     
  exit (1-rc);
}
