/* groups auth client definitions */

#ifndef GR_H 
#define GR_H

/* group names */

#define GR_GROUP   "u_cac_internal_groups_read"
#define GR_COURSE  "u_groups_courses_read"

/* Our id */
#define GR_CL_ID    "server-reg.cac.washington.edu"

/* function */

int gr_add(char *grp, char *cn, char **rsp);
int gr_remove(char *grp, char *cn, char **rsp);

#endif /* GR_H */
