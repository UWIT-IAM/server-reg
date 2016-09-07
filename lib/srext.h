/* server-reg ldap interfaces */

#ifndef SREXT_H 
#define SREXT_H

#include "uwldap.h"

/* group names */

#define SRLDAP_NETID   "Admin.UWNetID.read"
#define SRLDAP_GROUP   "Admin.Groups.read"
#define SRLDAP_COURSE  "Admin.Courses.read"

/* Our id */
#define SRLDAP_ID    "server-reg.cac.washington.edu"

/* config defaults */

#define CAFILE "etc/uw-incommon-ca.crt"
#define CRTFILE "etc/certs.ck"

/* functions */

char *sr_netid(char *regid);
char *sr_regid(char *netid);
char *sr_username(char *id);
char *sr_nicename(char *id);
WPInfo *sr_ewp_info(char *id);
int sr_is_member(char *id, char *grp);
int sr_is_eds_member(char *id, char *grp);
int sr_add_eds_member(char *id, char *grp);
int sr_remove_eds_member(char *id, char *grp);


#endif /* SREXT_H */
