/* server-reg external interface tools */

#include <stdio.h>

#include <ldap.h>
#include <lber.h>
#include <sasl/sasl.h>
#include <sys/time.h>

#include "util.h"
#include "uwldap.h"
#include "srext.h"


/* -------------------------------------------  */
static void *connect_to_ldap(char *host) {
   void *L;
   L = uwldap_connect(host, base_path_name(CRTFILE), base_path_name(KEYFILE), base_path_name(CAFILE));
   return (L);
}

/* get the name of a user */
char *sr_username(char *netid)
{
  void *L;
  char *name = NULL;

  if (L=connect_to_ldap(UWLDAP_EDS_HOST)) {
     name = uwldap_get_username(L, netid);
     uwldap_disconnect(L);
  }
  if (!name) name = strdup(netid);
  return (name);
}


/* get the regid of a user */
char *sr_regid(char *netid)
{
  void *L;
  char *id = NULL;

  if (L=connect_to_ldap(UWLDAP_EDS_HOST)) {
     id = uwldap_get_regid(L, netid);
     uwldap_disconnect(L);
  }
  return (id);
}

/* get the netid of a user */
char *sr_netid(char *regid)
{
  void *L;
  char *id = NULL;

  if (L=connect_to_ldap(UWLDAP_EDS_HOST)) {
     id = uwldap_get_netid(L, regid);
     uwldap_disconnect(L);
  }
  return (id);
}


/* nice looking name */
char *sr_nicename(char *id)
{
   char *n = sr_username(id);
   char *p;
   int c = 0;
   for (p=n;*p;p++) {
     if (c && isupper(*p)) *p = tolower(*p);
     if (isspace(*p)) c = 0;
     else c++;
   }
   return (n);
}

/* get ewp info */
WPInfo *sr_ewp_info(char *id)
{
  WPInfo *wp = NULL;
  void *L;

  if (L=connect_to_ldap(UWLDAP_EDS_HOST)) {
     wp = uwldap_get_wp_netid(L, id);
     uwldap_disconnect(L);
  }
  return (wp);
}


/* check user in group */
int sr_is_member(char *id, char *grp)
{
  void *L;
  int rc = 0;

  if (L=connect_to_ldap(UWLDAP_GDS_HOST)) {
     rc = uwldap_is_member(L, id, grp);
     uwldap_disconnect(L);
  }
  return (rc);
}

/* check user in eds auth group */
int sr_is_eds_member(char *id, char *grp)
{
  void *L;
  int rc = 0;

  if (L=connect_to_ldap(UWLDAP_EDS_HOST)) {
     rc = uwldap_is_eds_member(L, id, grp);
     uwldap_disconnect(L);
  }
  return (rc);
}


/* add user to eds auth group */
/* need error checking here */

static char *eds_host_list[] = UWLDAP_EDS_HOST_LIST;

int sr_add_eds_member(char *id, char *grp)
{
  void *L;
  int rc = 0;
  int i;

  for (i=0; i<N_UWLDAP_EDS_HOST; i++) {
     if (L=connect_to_ldap(eds_host_list[i])) {
        rc = uwldap_add_eds_member(L, id, grp);
        uwldap_disconnect(L);
     }
  }
  if (L=connect_to_ldap(UWLDAP_EDS_EVAL_HOST)) {
     rc = uwldap_add_eds_member(L, id, grp);
     uwldap_disconnect(L);
  }
  return (rc);
}



/* remove user from eds auth group */
/* need error checking here */
int sr_remove_eds_member(char *id, char *grp)
{
  void *L;
  int rc = 0;
  int i;

  for (i=0; i<N_UWLDAP_EDS_HOST; i++) {
     if (L=connect_to_ldap(eds_host_list[i])) {
        rc = uwldap_remove_eds_member(L, id, grp);
        uwldap_disconnect(L);
     }
  }
  if (L=connect_to_ldap(UWLDAP_EDS_EVAL_HOST)) {
     rc = uwldap_remove_eds_member(L, id, grp);
     uwldap_disconnect(L);
  }
  return (rc);
}


