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

/* EDS database reconciliation */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "pgdb.h"
#include "srext.h"
#include "srgr.h"

#define PROJECT "eds2"
extern char *remote_user;
extern char *user_list;
extern int user_list_len;
extern int errno;
char *errstr = NULL;

int recon_app(char *cn)
{
   int a;
   Application *app;
   int isauth;

   if (app = db_get_application(PROJECT, cn)) {
      for (a=0;a<SR_AUTH_NUM; a++) {
         AuthDef *A = &sr_auth_def[a];
         int ast = app->auth_st[a];

         printf("  %s: ", A->fname);

         isauth = A->auth_type==AUTH_TYPE_DIRECT? sr_is_eds_member(app->appid, A->auth_group_name): sr_is_member(app->appid, A->auth_group_name);

      if (!ast) {
         printf(" no_request ");
         if (isauth) printf(" .. but authorized!");
      } else if (ast&SR_AUTH_ST_OK) {
         printf(" approved ");
         if (!isauth) printf(" .. but not yet authorized");
      } else if (ast&SR_AUTH_ST_REQ) {
         printf(" requested ");
         if (isauth) printf(" .. but already authorized");
      } else {
         printf( "unknown");
      }
       printf("\n");

     }
  }

}



main() 
{
   char **cnlist;
   char **cnlp;
   char *cn;

   char *edshome = DIRROOT "/"  PROJECT;
  
   chdir(edshome);
   if (get_config(DIRROOT "/"  PROJECT "/" EDS_CONFIG)) {
      perror(EDS_CONFIG);
      exit(1);
   }

   remote_user = "fox";

   cnlist = db_list_of_apps(PROJECT, 0);
   for (cnlp=cnlist; *cnlp; cnlp++) {
     printf("cn: %s\n", *cnlp);
     recon_app(*cnlp);
   }

}
