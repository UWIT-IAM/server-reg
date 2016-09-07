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

/* Simple filesystem database for form storage and retrieval
   for applications using webTemplates */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "srext.h"

#include "fsdb.h"

extern char *remote_user;
extern int errno;


/* Compute path to the form directory */
static char *make_form_dir(char *proj, char *dir) 
{
  char *path = (char*) malloc(strlen(DIRROOT)+strlen(proj)+strlen(dir)+5);
  sprintf(path, "%s/%s/%s", DIRROOT, proj, dir);
  return (path);
}

/* Compute path to the form file */
static char *make_form_path(char *proj, char *dir, char *name, char *ext) 
{
  char *path = (char*) malloc(strlen(DIRROOT)+strlen(proj)+strlen(dir)+strlen(name)+strlen(ext)+5);
  sprintf(path, "%s/%s/%s/%s.%s", DIRROOT, proj, dir, name, ext);
  return (path);
}


/* Write a form to the db. 

   Form template (tpl) showld have 

      <!-- BDB: question -->
      :{Q}:  {RESP}
      :end:
      <!-- EDB: question -->
      
   Form questions and values (q*) in the input template (W) are 
   written to (tpl).  
   
 */

static int write_a_form(WebTemplate W, char *cn, char *path, char *tpl)
{
  char *p;
  FILE *f;
  char **rspl, *rsp;
  time_t now = time(NULL);
  WebTemplate F = newWebTemplate();
  int a;
  char *an;
  char *av;
  char *ov;
  char ts[24];

  if (!(f=fopen(path,"w"))) {
    perror(path);
    return (errno);
  }
  WebTemplate_set_output(F, fileno(f));
  WebTemplate_set_noheader(F);
  WebTemplate_set_comments(F, "#", NULL);
  WebTemplate_get_by_name(F, "page", tpl);
  WebTemplate_assign(F, "cn", cn);
  WebTemplate_assign(F, "remote_user", remote_user);
  WebTemplate_assign(F, "show_date", ctime(&now));
  sprintf(ts, "%d", now);
  WebTemplate_assign(F, "time", ts);
  
  /* scan form values and add any 'q*' to output */ 
  for (a=0;;) {
     if (!(an=WebTemplate_get_next_arg(W, &a, &av))) break;
     if (*an != 'q') continue;  // not one of our's
     
     /* record the q&a for retireval by the caller */
     if (strncmp(an,av,strlen(an))) p = av;
     else p = av+strlen(an)+1;
     WebTemplate_assign(F, an, p);

     /* store this q&a in the db */
     WebTemplate_assign(F, "Q", an);
     WebTemplate_assign(F, "RESP", av);
     WebTemplate_parse_dynamic(F, "page.question");
     free(an);
     if (av) free(av);
  }

  WebTemplate_parse(F, "PAGE", "page");
  WebTemplate_write(F, "PAGE");
  fclose(f);
  return (0);
}


char *save_the_form(WebTemplate W, char *proj, char *cn)
{
   char *path;
   char ts[24];
   time_t now = time(NULL);

   /* save the database file */
   path = make_form_path(proj, "db", cn, "q");
   write_a_form(W, cn, path, "req_q.tpl");
   free(path);

   /* save the log file */
   sprintf(ts, "q%d", now);
   path = make_form_path(proj, "log", cn, ts);
   write_a_form(W, cn, path, "req_log.tpl");
   return (path);
}


/* save suthorizations in a form file ( :q*: value ) */
int save_the_auth(char *proj, char *cn, char *authq, char **auths)
{
   char *path;
   char ts[24];
   time_t now = time(NULL);
   FILE *f;

   /* save the auth database file */
   path = make_form_path(proj, "db", cn, "a");
   if (f=fopen(path, "w")) {
      fprintf(f, "# Authorizations for %s\n", cn);
      while (*auths) {
         fprintf(f, ":%s: %s\n:end:\n", authq, *auths);
         auths++;
      }
   }
   fclose(f);
   free(path);

   return (0);
}

/* --- Copying form data to the active template ---- */

/* We have to be able to clear the values we load, as
   we might get run more than once on a single request. 
   Using static variables works if our clients only
   use one output template - which is always the case. */

static WebTemplate lastW = NULL;
static char *lastQs[256];  /* could there be more than 256 questions? */
static int nQs = 0;

int clear_form_data(WebTemplate W)
{
   char **q;
   if (!lastW) return (1);
   if (lastW!=W) return (0);
   for (q=lastQs; *q; q++) {
      WebTemplate_assign(W, *q, NULL);
      free(*q);
   }
   lastW = NULL;
   nQs = 0;
   return (1);
}

static int record_form_question(WebTemplate W, char *q)
{
   if (nQs>254) return (0);
   if (nQs==0) lastW = W;
   lastQs[nQs++] = strdup(q);
   lastQs[nQs] = NULL;
   return (1);
}

/* Copy form values to the template.
   Only return non-zero if the file cannot be opened. */

int load_form_from_file(WebTemplate W, char *fn)
{
   FILE *f;
   char rec[2048];
   char q[48];
   char rsp[4096];
   char *e;
   char *p;

   if (!(f=fopen(fn,"r"))) {
      // perror(fn);       // not all forms have answers
      return (errno);
   }

   while (fgets(rec, 2048, f)) {
       if (rec[0]!=':') continue;
       if (rec[1]!='q') continue;

       // is a q?, get the question 
       p = rec+1;
       for (e=rec+2; *e && *e!=':' && !isspace(*e); e++);
       *e++ = '\0';
       if (strlen(p)>47) return (-1);
       strcpy(q, p);

       // get the answer
       rsp[0] = '\0';
       while (*e && isspace(*e)) e++;
       if (*e) strcat(rsp, e);
       while (fgets(rec, 2048, f)) {
           if (!strncmp(rec, ":end:", 5)) break;
           strcat(rsp, rec);
       }

       // set in the output form (value or check)
       if (!strncmp(q, rsp, strlen(q))) {
          char *ck = (char*) malloc(strlen(q)+20);
          char *t;
          for (t=rsp; *t; t++) if (*t!='_' && !isalnum(*t)) break;
          *t = '\0';
          sprintf(ck, "%s_checked", rsp);
          WebTemplate_assign(W, ck, "checked");
          record_form_question(W, ck);
       } else if (*rsp) {
          char *frsp = WebTemplate_text2html(rsp);
          WebTemplate_assign(W, q, frsp);
          record_form_question(W, q);
       }
   }
   fclose(f);
   return (0);
}
   

/* Load form values from a database. Return non-zero
   if the database does not exist. */

int load_form_from_db(WebTemplate W, char *proj, char *cn)
{
   int r;
   char *p = make_form_path(proj, "db", cn, "q");
   clear_form_data(W);
   r = load_form_from_file(W, p);
   free(p);
   if (r) return (r);
   p = make_form_path(proj, "db", cn, "a");
   load_form_from_file(W, p);
   free(p);
   return (0);
}


/* Get a list of forms */

char **list_of_forms(char *proj)
{
   int i,n;
   struct dirent **list;
   char *path = make_form_dir(proj, "db");
   char **ret;
   char **rp;
   char *form;
   char *p;

   n = scandir(path, &list, 0, alphasort);
   if (n<0) {
      perror("scandir");
      return (NULL);
   }

   ret = (char**) malloc(sizeof(char*)*n);
   rp = ret;
   for (i=0;i<n;i++) {
      if (list[i]->d_name[0] == '.') continue;
      if (!(p=strstr(list[i]->d_name, ".q"))) continue;
      *p = '\0';
      *rp++ = strdup(list[i]->d_name);
      free(list[i]);
   }
   *rp++ = NULL;
   return (ret);
}
