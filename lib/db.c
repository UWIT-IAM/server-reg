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

/* pubcookie authorization database routines */

#include <string.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>


#include "db.h"
#include "util.h"

extern char *remote_user;
extern int errno;


/* Manage list of approved users.
   The list is a flat text file.  Access is not locked.
   At worst an update can get lost.

   This is not a heavily used DB.
 */

#define USER_FILE "etc/users"
#define TEMP_FILE "etc/users.XXXXXX"

char *user_list = NULL;
int user_list_len;

int user_list_find(char *id)
{
   char usr[24];
   if (!user_list) user_list = get_file(path_name(USER_FILE), &user_list_len);
   if (!user_list) return (0);
   sprintf(usr,"\n%s\n", id);
   if (strstr(user_list, usr)) return (1);
   return (0);
}

/* Add an id to the user's list */

int user_list_add(char *id)
{
   char *lp, *ep;
   char *nu;
   int nfd;
   char tnam[64];
   int inlist = 0;

   if (user_list) free(user_list);
   user_list = NULL;
   if (user_list_find(id)) return (1);  /* already there */
   if (!user_list) return (0);

   strcpy(tnam, path_name(TEMP_FILE));
   nfd = mkstemp(tnam);
   if (!nfd) perror("tnam");
   
   /* Get to the insert point */
   for (lp=user_list,ep=strchr(lp,'\n'); ep; lp=++ep,ep=strchr(lp,'\n')) {
      if (strncmp(lp,"#BEGIN",6)==0) inlist=1;
      else if (strncmp(lp,"#END",4)==0) break;
      else if (inlist && strncmp(lp, id, ep-lp)>0) break;
   }

   write(nfd, user_list, lp-user_list);
   write(nfd, id, strlen(id));
   write(nfd, "\n", 1);
   write(nfd, lp, user_list_len - (lp - user_list));
   close(nfd);
   rename(tnam, path_name(USER_FILE));

   free(user_list);
   user_list = NULL;
   return (1);
}


/* Remove an id from the user's list */

int user_list_del(char *id)
{
   char *lp, *ep;
   char tnam[64];
   int nfd;
   int inlist = 0;

   if (user_list) free(user_list);
   user_list = NULL;
   if (!user_list_find(id)) return (1);  /* not there */
   if (!user_list) return (0);

   strcpy(tnam, path_name(TEMP_FILE));
   nfd = mkstemp(tnam);
   if (!nfd) perror("tnam");
   
   /* Find the id */
   for (lp=user_list,ep=strchr(lp,'\n'); ep; lp=++ep,ep=strchr(lp,'\n')) {
      if (strncmp(lp,"#BEGIN",6)==0) inlist = 1;
      else if (strncmp(lp,"#END",4)==0) return (1);
      else if (inlist && strncmp(lp, id, ep-lp)==0) break;
   }

   write(nfd, user_list, lp-user_list);
   ep++;
   write(nfd, ep, user_list_len - (ep - user_list));
   close(nfd);
   rename(tnam, path_name(USER_FILE));

   free(user_list);
   user_list = NULL;
   return (1);
}

/* Decide if we are pre-approved */
int am_approved()
{
   return (user_list_find(remote_user));
}


