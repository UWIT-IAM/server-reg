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

/* EDS registration admin page */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "pgdb.h"
#include "srext.h"
#include "srgr.h"

extern char *remote_user;
extern char *user_list;
extern int user_list_len;
extern int errno;
char *errstr = NULL;

#define REQ(s) (req && !strcmp(req,s))

#define IMAGE_OK    "/server-reg/icons/greenball.gif"
#define IMAGE_WANTS "/server-reg/icons/orangeball.gif"
#define IMAGE_BLANK "/server-reg/icons/whiteball.gif"
#define IMAGE_BAD   "/server-reg/icons/redball.gif"

char *q5_choices[] = {
 "q5_admin",
 "q5_dept",
 "q5_xxx",
 "q5_other",
};
#define NUM_Q5 4

/* If a choice is checked, do its dynamic block */
static void set_choice(WebTemplate W, char *q)
{
   char q_chk[128];
   char mv[128];
   char *qc, *qac;

   strcat(strcpy(q_chk, q), "_checked");
   qc = WebTemplate_macro_value(W, q_chk);
   if (qc) {
      strcat(strcpy(mv, "detail."), q_chk);
      WebTemplate_parse_dynamic(W, mv);
   }
}

/* add all user choices to the detail page */
static void set_choices(WebTemplate W, Application *app)
{
   int c;

   for (c=0; c<NUM_Q5; c++) set_choice(W, q5_choices[c]);
   for (c=0; c<NUM_AUTHDEF; c++) set_choice(W, sr_auth_def[c].qname);
   
}


// set the 'authorizations dot' in the list entry
static void set_list_auth(WebTemplate W, Application *app)
{
   int a;

   for (a=0;a<SR_AUTH_NUM; a++) {
      char *img = IMAGE_BLANK;
      if (app->auth_st[a]&SR_AUTH_ST_REQ) {
         img = IMAGE_WANTS;
         if (app->auth_st[a]&SR_AUTH_ST_OK) img = IMAGE_OK;
      } else if (app->auth_st[a]!=0) img = IMAGE_BAD;
      WebTemplate_assign(W, "auth_img", img);
      WebTemplate_parse_dynamic(W, "list.cn.auth");
   }
}


// set the 'authorizations' in the detail entry

#define AUTH_ACTION_GRANT 1
#define AUTH_ACTION_REVOKE 2
#define AUTH_ACTION_LDAPGRANT 4  // these two are fixes
#define AUTH_ACTION_LDAPREVOKE 8

static void set_detail_auth(WebTemplate W, Application *app)
{
   int a;
   int isauth;
   int need_button = 0;

   for (a=0;a<SR_AUTH_NUM; a++) {
      AuthDef *A = &sr_auth_def[a];
      int ast = app->auth_st[a];
      int auth_action = 0;

      // if (!ast) continue;
      isauth = A->auth_type==AUTH_TYPE_DIRECT? sr_is_eds_member(app->appid, A->auth_group_name): sr_is_member(app->appid, A->auth_group_name);

      // show status - lots of different combinations
      // use green to show ok, yellow needs action, red needs a problem fixed
      WebTemplate_assign(W, "auth_name", A->fname);

      if (ast&SR_AUTH_ST_REQ) {

         // user has requested access

         WebTemplate_assign(W, "auth_req", "Yes");
         if (ast&SR_AUTH_ST_OK) {
            // approved
            WebTemplate_assign(W, "auth_app", "Yes");
            WebTemplate_assign(W, "auth_req_class", "status_green");
            auth_action |= AUTH_ACTION_REVOKE;
            if (isauth) {
               // ldap server has authorization
               WebTemplate_assign(W, "auth_ldap", "Yes");
               WebTemplate_assign(W, "auth_app_class", "status_green");
               WebTemplate_assign(W, "auth_ldap_class", "status_green");
            } else {
               WebTemplate_assign(W, "auth_ldap", "No");
               WebTemplate_assign(W, "auth_app_class", "status_yellow");
               WebTemplate_assign(W, "auth_ldap_class", "status_red");
               auth_action |= AUTH_ACTION_LDAPGRANT;
            }
         } else {
            // not approved
            WebTemplate_assign(W, "auth_app", "No");
            WebTemplate_assign(W, "auth_req_class", "status_yellow");
            auth_action |= AUTH_ACTION_GRANT;
            if (isauth) {
               // ldap server already has authorization
               WebTemplate_assign(W, "auth_ldap", "Yes");
               WebTemplate_assign(W, "auth_ldap_class", "status_red");
               auth_action |= AUTH_ACTION_LDAPGRANT;
            } else {
               WebTemplate_assign(W, "auth_ldap", "No");
               WebTemplate_assign(W, "auth_app_class", "status_yellow");
               WebTemplate_assign(W, "auth_ldap_class", "status_yellow");
            }
         }

      } else {

         // user has not requested access

         WebTemplate_assign(W, "auth_req", "No");
         if (ast&SR_AUTH_ST_OK) {
            // approved
            WebTemplate_assign(W, "auth_app", "Yes");
            WebTemplate_assign(W, "auth_req_class", "status_yellow");
            auth_action |= AUTH_ACTION_REVOKE;
            if (isauth) {
               // ldap server has authorization
               WebTemplate_assign(W, "auth_ldap", "Yes");
               WebTemplate_assign(W, "auth_app_class", "status_yellow");
               WebTemplate_assign(W, "auth_ldap_class", "status_red");
            } else {
               WebTemplate_assign(W, "auth_ldap", "No");
               WebTemplate_assign(W, "auth_app_class", "status_yellow");
               WebTemplate_assign(W, "auth_ldap_class", "status_yellow");
            }
         } else {
            // not approved
            WebTemplate_assign(W, "auth_app", "No");
            WebTemplate_assign(W, "auth_request_class", "status_green");
            if (isauth) {
               // ldap server already has authorization
               WebTemplate_assign(W, "auth_ldap", "Yes");
               WebTemplate_assign(W, "auth_app_class", "status_yellow");
               WebTemplate_assign(W, "auth_ldap_class", "status_red");
               auth_action |= AUTH_ACTION_LDAPREVOKE;
            } else {
               WebTemplate_assign(W, "auth_ldap", "No");
               WebTemplate_assign(W, "auth_app_class", "status_green");
               WebTemplate_assign(W, "auth_ldap_class", "status_green");
               continue;  // no request, no grants, no show
            }
         }
      }

      // add possible admin actions
      fprintf(stderr, "user: %s, admin group: %s\n", remote_user, A->admin_group_name);
      if ( sr_is_member(remote_user, A->admin_group_name) ) {
         if (auth_action&AUTH_ACTION_GRANT) WebTemplate_parse_dynamic(W, "detail.auth_show.grant");
         if (auth_action&AUTH_ACTION_REVOKE) WebTemplate_parse_dynamic(W, "detail.auth_show.revoke");
         if (auth_action&AUTH_ACTION_LDAPGRANT) WebTemplate_parse_dynamic(W, "detail.auth_show.ldapgrant");
         if (auth_action&AUTH_ACTION_LDAPREVOKE) WebTemplate_parse_dynamic(W, "detail.auth_show.ldaprevoke");
      }

      WebTemplate_parse_dynamic(W, "detail.auth_show");
      if (auth_action) need_button++;
   }
   if (need_button) WebTemplate_parse_dynamic(W, "detail.auth_button");

}

// standard way to print and exit
static void showpage_and_exit(WebTemplate W)
{
  WebTemplate_parse(W, "PAGE", "page");
  WebTemplate_write(W, "PAGE"); 
  db_disconnect();
  exit (0);
}

static void cat_msg(char **str, char *add1, char *add2)
{
   *str = (char*)realloc(*str, strlen(*str)+strlen(add1)+strlen(add2)+16);
   strcat(strcat(strcat(*str, "<br>"), add1), add2);
}

main()
{
  WebTemplate W = newWebTemplate();
  char *userid;
  char *req;
  char *seqstr;
  int seq = 0;
  int s;
  char *xseq;
  char *doc_root = URLROOT;
  char *cn;
  int alp;
  char *onr;
  char *path;
  char mailsub[512];
  char *rsp = NULL;
  struct dirent **list;
  int i,n;
  char *gets_mail = NULL;
  char *from_mail = NULL;
 
  WebTemplate_set_comments(W, "#", NULL);
  WebTemplate_get_by_name(W, "page", "../../page.tpl");

  if (get_config(path_name(EDS_CONFIG))) {
     WebTemplate_assign(W, "EMSG", "Configuration error");
     showpage_and_exit(W);
  }

  /* Get arguments */

  WebTemplate_get_args(W);

  if (cn=strchr(remote_user, '@')) *cn = '\0';
  userid = remote_user;
  seqstr = WebTemplate_get_arg(W, "seq");
  if (seqstr) seq = atoi(seqstr);
  req = WebTemplate_get_arg(W, "req");
  xseq = WebTemplate_get_arg(W, "xseq");
  cn = WebTemplate_get_arg(W, "cn");
  if (cn && (strlen(cn)>254)) cn[254] = '\0';

  WebTemplate_assign(W, "TITLE",
             "EDS registration maintenance");
  WebTemplate_assign(W, "CURPAGE", "Administration");
  xsec_set_valid_form(W);
  WebTemplate_assign(W, "ROOT", doc_root);

  WebTemplate_parse_dynamic(W, "page.admin");

  crumb_home(W);
  add_crumb(W, "", NULL, "EDS");

  /* rest of page depends on request */

  /* send the 'services:about' page */

  if (REQ("help")) {

     WebTemplate_assign(W, "PAGETITLE", "EDS Help");
     WebTemplate_get_by_name(W, "help", "help.tpl");
     WebTemplate_parse(W, "DETAIL", "help");
     add_crumb(W, "admin/", NULL, "Admin");
     req = "skip";

 
  /* show a filled-out form, with auth buttons */

  } else if (REQ("detail")) {

     char em[512];
     char *formtxt;
     Application *app;

     add_crumb(W, "admin/", NULL, "Admin");
     add_crumb(W, "admin/", "req=apps", "Forms");

     WebTemplate_assign(W, "PAGETITLE", cn);
     WebTemplate_assign(W, "CN", cn);
     WebTemplate_get_by_name(W, "detail", "detail.tpl");
     if (cn && (app=db_get_application(PROJECT, cn))) {
        db_load_form_from_db(W, app);
        set_detail_auth(W, app);
        set_choices(W, app);
        WebTemplate_assign(W, "last_updater", app->userid);
        WebTemplate_assign(W, "last_update", ctime(&app->rsp_date));
     }
     WebTemplate_parse(W, "BODY", "detail");
     req = "skip";


  /* perform some actions */

  } else if (REQ("action")) {

     char **actions = WebTemplate_get_arg_list(W, "auth_action");
     char **acp;
     char *authset, *p;
     AuthDef *authdef;
     int authn;
     char cr1[128];
     Application *app;
     char *msgs = strdup("");
     char *emsgs = strdup("");

     if (!xsec_chk_valid_form(W)) {
        printf("bad xsec?\n");
        exit (1);
     }

     add_crumb(W, "admin/", NULL, "Admin");
     add_crumb(W, "admin/", "req=apps", "Forms");
     snprintf(cr1, 128, "req=detail&cn=%s", cn);
     add_crumb(W, "admin/", cr1, cn);

     WebTemplate_assign(W, "PAGETITLE", cn);
     WebTemplate_assign(W, "CN", cn);
     WebTemplate_get_by_name(W, "actions", "actions.tpl");

     // do something for each action request

     for (acp=actions; acp && *acp; acp++) {
         int grant = 0;
         int ldap = 0;
         char *tgt = *acp;
         if (!strncmp(tgt, "grant ", 6)) {
            tgt += 6;
            grant = 1;
         } else if (!strncmp(tgt, "revoke ", 7)) {
            tgt += 7;
         } else if(!strncmp(tgt, "ldapgrant ", 10)) {
            tgt += 10;
            grant = 1;
            ldap = 1;
         } else if(!strncmp(tgt, "ldaprevoke ", 11)) {
            tgt += 11;
            ldap = 1;
         } else continue;
     
     
         // see what auth is wanted (authn and authdef)
         authdef = NULL;
         for (authn=0; authn<NUM_AUTHDEF; authn++) {
            if (tgt && !strcmp(tgt,sr_auth_def[authn].fname)) {
               authdef = &sr_auth_def[authn];
               break;
            }
         }
         if (!authdef) {
            cat_msg(&emsgs, "Bogus auth ignored. ", "");
            continue;
         }

         // printf("<p>cn=%s, auth name=%s<p>\n", cn, as->name);
         // printf("<p>user=%s, admin group=%s, auth group=%s<p>\n", remote_user, as->admin_group_name, as->auth_group_name);
     
         // check the app to be auth'd
         if (cn && (app=db_get_application(PROJECT, cn))) {

            // check request
            if (!sr_is_member(remote_user, authdef->admin_group_name)) {
               cat_msg(&emsgs, tgt, "no permission");
      
            } else if (grant && (!ldap) && app->auth_st[authn]&SR_AUTH_ST_OK) {
               cat_msg(&emsgs, tgt, "already authorized");
   
            } else if ((!grant) && (!ldap) && (!app->auth_st[authn]&SR_AUTH_ST_OK)) {
               cat_msg(&emsgs, tgt, "not authorized");
   
            } else if (grant && !(app->auth_st[authn]&SR_AUTH_ST_REQ)) {
               cat_msg(&emsgs, tgt, "authorization not requested");
   
            } else {
     
               /* do the authz */

               char *em = (char*)malloc(256);
               Authorization **auths, *auth;
               char *cc_mail = NULL;
               char *msg;
               int ccsize;
               char **onrs, **op;
               char *c;
               int uwl;
               char *mail_msg;
               WebTemplate M =  newWebTemplate();
               char *msgpart = NULL;
               int nonrs;

               // activate 
               if (authdef->auth_type==AUTH_TYPE_DIRECT) {
                  if (grant) sr_add_eds_member(cn, authdef->auth_group_name);
                  else sr_remove_eds_member(cn, authdef->auth_group_name);
               } else {
                  char *rsp;
                  if (grant) gr_add(authdef->auth_group_name, cn, &rsp);
                  else gr_remove(authdef->auth_group_name, cn, &rsp);
               }

               // update the db
               auth = db_new_authorization(cn, authn);
               if (grant) auth->status = app->auth_st[authn] | SR_AUTH_ST_OK;
               else auth->status = app->auth_st[authn] & ~SR_AUTH_ST_OK;
               auth->auth_id = strdup(remote_user);
           
               auths = (Authorization**) malloc(sizeof(Authorization*)*2);
               auths[0] = auth;
               auths[1] = NULL;
               db_update_authorizations(auths);

               /* mail to interested parties 
                  Notice to app's email, cc to person-reg and dns owners 
                */

               sprintf(em, "Auth '%s' for %s %s by %s\n", authdef->fname, cn, grant?"approved":"revoked", remote_user);
               cat_msg(&msgs, "", em);

               if (ldap) continue;  /* No mail on ldap fixes */
            
               // format the message
               WebTemplate_set_comments(M, "#", NULL);
               WebTemplate_get_by_name(M, "mail", "mailer.tpl");
               WebTemplate_assign(M, "CN", cn);
               WebTemplate_assign(M, "access", authdef->name);

               if (!strncmp(authdef->fname, "PDS", 3)) {
                  cc_mail = strdup(sr_config->eds_app_pds_to);
                  msgpart = grant? "mail.pds_app":"mail.pds_rev";
               } else if (!strncmp(authdef->fname, "GDS", 3)) {
                  cc_mail = strdup(sr_config->eds_app_gds_to);
                  msgpart = grant? "mail.gds_app":"mail.gds_rev";
               }

               if (!cc_mail) cc_mail = strdup(sr_config->eds_app_to);
               if (!(from_mail=sr_config->eds_from)) from_mail = "help@uw.edu";
            
               // use the group email if there is one
               gets_mail = app->app_email;
               if (!gets_mail) {
                  gets_mail = (char*) malloc(strlen(app->userid)+32);
                  sprintf(gets_mail, "%s@uw.edu", app->userid);
               }

               // add dns owners to the cc list
               onrs = get_dns_owners(W, cn, &nonrs);
               if (cc_mail) {
                  cc_mail = (char*) realloc(cc_mail, 16 + nonrs*64 + strlen(cc_mail));
                  for (c=cc_mail; *c; c++);
               } else {
                  cc_mail = (char*) malloc(16 + nonrs*64);
                  c = cc_mail;
               }
               for (op=onrs; op&&*op; op++) {
                 if (c!=cc_mail) *c++ = ',';
                 strcat(strcpy(c, *op), "@uw.edu");
                 for (; *c; c++);
               }
               *c = '\0';

               WebTemplate_parse_dynamic(M, msgpart);
               WebTemplate_parse(M, "msg", "mail");
               if (mail_msg = WebTemplate_macro_value(M, "msg")) {
                  send_mail_cc(gets_mail, cc_mail, em, mail_msg, from_mail);
               }
 

#ifdef OWNERS_OWN_MAIL
               /* send the owners a notice */
               onrs = get_dns_owners(W, cn, &nonrs);
               cc_mail = (char*) malloc(nonrs*64);
               for (c=cc_mail,op=onrs; op&&*op; op++) {
                 if (c!=cc_mail) *c++ = ',';
                 strcat(strcpy(c, *op), "@uw.edu");
                 for (; *c; c++);
               }
               *c = '\0';

               if (grant) WebTemplate_parse_dynamic(M, "mail.noaction_app");
               else WebTemplate_parse_dynamic(M, "mail.noaction_rev");
               WebTemplate_parse_dynamic(M, msgpart);
               WebTemplate_parse(M, "msg", "mail");
               if (mail_msg = WebTemplate_macro_value(M, "msg")) {
                  send_mail_cc(cc_mail, NULL, em, mail_msg, from_mail);
               }
#endif


            }
          } else {
             cat_msg(&emsgs, cn, "not found");
          }

        } // loop on actions

        WebTemplate_assign(W, "MSG", msgs);
        WebTemplate_assign(W, "EMSG", emsgs);
        WebTemplate_parse(W, "BODY", "actions");
        req = "skip";

  } // req(action)

  /* Always send one of the admin main pages */

  if (REQ("apps")) {

     char *rf = WebTemplate_get_arg(W, "select");
     char **fl;
     char **fp;
     int n = 0;
     char *qv;
     int active = 1;
     Application *app;

     // select=all means all forms, else just active ones
     if (rf && !strcmp(rf,"all")) active = 0;

     add_crumb(W, "admin/", NULL, "Admin");

     WebTemplate_assign(W, "PAGETITLE", "EDS Administration");
     WebTemplate_get_by_name(W, "list", "requests.tpl");
     if (active) WebTemplate_parse_dynamic(W, "list.actapps");
     else WebTemplate_parse_dynamic(W, "list.allapps");

     fl = db_list_of_apps(PROJECT, active);
     for (fp=fl; *fp; fp++) {
        if (app = db_get_application(PROJECT, *fp)) {
          db_load_form_from_db(W, app);
          set_list_auth(W, app);
          WebTemplate_assign(W, "CN", app->appid);
          WebTemplate_assign(W, "app_grp", app->app_owner?app->app_owner:"");
          WebTemplate_assign(W, "app_email", app->app_email?app->app_email:"");
          WebTemplate_parse_dynamic(W, "list.cn");
        }
        free(*fp);
        n++;
     }
     free(fl);
     if (n==0) WebTemplate_parse_dynamic(W, "list.none");
     WebTemplate_parse(W, "BODY", "list");


  } else if (!REQ("skip")) {

     WebTemplate_assign(W, "PAGETITLE", "EDS Administration");
     WebTemplate_get_by_name(W, "menu", "menu.tpl");
     WebTemplate_parse(W, "MENU", "menu");
     WebTemplate_parse_dynamic(W, "page.menu");
     WebTemplate_get_by_name(W, "home", "home.tpl");
     WebTemplate_parse(W, "BODY", "home");

  }

  WebTemplate_parse(W, "PAGE", "page");
  WebTemplate_write(W, "PAGE");

}

