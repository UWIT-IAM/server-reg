/* ldap group library */

#ifndef UWLDAP_H
#define UWLDAP_H

#include <sys/types.h>

/* ---- ldap defaults: these must match the ldap schema - */

/* server hostnames */
/* these are for wueries only **/
#define UWLDAP_GDS_HOST         "groups.u.washington.edu"      /* groups */
#define UWLDAP_EDS_HOST         "eds.u.washington.edu"     /* eds */
#define UWLDAP_EDS_EVAL_HOST    "rufus21.s.uw.edu"      /* eds-eval */

/* eds hosts to update */
#define UWLDAP_EDS_HOST_LIST  {"seneca21.s.uw.edu","seneca22.s.uw.edu","seneca23.s.uw.edu"}
#define N_UWLDAP_EDS_HOST 4


/* ports for plain and ssl connections */
#define UWLDAP_PORT          389
#define UWLDAP_SSL_PORT      389

/* Base of searches */
#define UWLDAP_GDS_BASE      "ou=groups,dc=washington,dc=edu"
#define UWLDAP_EDS_BASE      "ou=UWAuthorizations,dc=personregistry,dc=washington,dc=edu"
#define UWLDAP_USR_BASE     "ou=People,dc=personregistry,dc=washington,dc=edu"

typedef struct WPInfo_ {
  char *name;
  char *dept;
  char *title;
  char *phone;
  char *email;
  char *publish;
} WPInfo;

/* procedures  */

void *uwldap_connect(char *host, char *bindcert, char *bindkey, char *ca);
void uwldap_disconnect(void *ldv);

char *uwldap_get_username(void *ldv, char *id);
int uwldap_is_member(void *ldv, char *id, char *group);
int uwldap_is_eds_member(void *ldv, char *cn, char *group);
int uwldap_add_eds_member(void *ldv, char *cn, char *group);
int uwldap_remove_eds_member(void *ldv, char *cn, char *group);
char *uwldap_get_regid(void *ldv, char *netid);
char *uwldap_get_netid(void *ldv, char *regid);

WPInfo *uwldap_get_wp_netid(void *ldv, char *netid);

#endif /* UWLDAP_H */
