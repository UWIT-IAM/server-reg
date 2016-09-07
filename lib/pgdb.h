/* ========================================================================
 * Copyright (c) 2007 The University of Washington
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

/* form definitions */

#include "webtpl.h"
#include "srgr.h"

/* authorizations */

#define SR_AUTH_BASIC      0
#define SR_AUTH_EMPLOYEE   1
#define SR_AUTH_STUDENT    2
#define SR_AUTH_ALUMNI     3
#define SR_AUTH_GROUPS     4
#define SR_AUTH_COURSES    5

#define SR_AUTH_NUM        6

#define AUTH_TYPE_GROUP  1
#define AUTH_TYPE_DIRECT 2

typedef struct AuthDef_ {
   char *name;               /* auth name */
   char *fname;              /* full name */
   char *qname;              /* form question name */
   char *admin_group_name;   /* group of administrators for this type */
   char *auth_group_name;    /* ldap group name for the authorization */
   int auth_type;            /* means of authorization */
} AuthDef;

#ifdef PGDB_LIBRARY
AuthDef sr_auth_def[] = {
  {"basic", "PDS: basic", "q6_basic", "u_cac_admin_eds-basic", "BasicAccess", AUTH_TYPE_DIRECT},
  {"student", "PDS: student", "q6_student", "u_cac_admin_eds-student", "StudentAccess", AUTH_TYPE_DIRECT},
  {"employee", "PDS: employee", "q6_employee", "u_cac_admin_eds-employee", "EmployeeAccess", AUTH_TYPE_DIRECT},
  {"alumni", "PDS: alumni", "q6_alumni", "u_cac_admin_eds-alumni", "AlumniAccess", AUTH_TYPE_DIRECT},
  {"groups", "GDS: basic", "q6_groups", "u_cac_admin_gds-basic", GR_GROUP, AUTH_TYPE_GROUP},
  {"courses", "GDS: courses", "q6_courses", "u_cac_admin_gds-courses", GR_COURSE, AUTH_TYPE_GROUP},
};
#else
extern AuthDef sr_auth_def[];
#endif
#define NUM_AUTHDEF 6



/* auth status bits */

#define SR_AUTH_ST_REQ       1  // requested
#define SR_AUTH_ST_OK        2  // granted
#define SR_AUTH_ST_REAL      4  // installed on ldap servers
#define SR_AUTH_ST_VERIFIED  8  // ldap status verified  ( not from our database )


typedef struct Application_ {
  char *appid;
  char *proj;
  char *userid;
  char *app_owner;
  char *app_email;
  int status;
  int rsp_no;
  time_t rsp_date;
  int auth_st[SR_AUTH_NUM];
} Application;
  
typedef struct Authorization_ {
  char *appid;
  int auth_code;
  int status;
  char *req_id;
  time_t req_date;
  char *auth_id;
  time_t auth_date;
} Authorization;

/* functions */

int db_connect();
void db_disconnect();

int db_set_app_ownership(WebTemplate W, char *proj, char *cn, char *owneowner);
char *db_save_form(WebTemplate W, char *proj, char *cn, char *logtpl);
int db_update_authorizations(Authorization **auths);
Authorization *db_new_authorization(char *appid, int a);

void db_free_application(Application *app);
Application *db_get_application(char *proj, char *appid);

int db_load_form_from_db(WebTemplate W, Application *app);

char **db_list_of_apps(char *proj, int active);

