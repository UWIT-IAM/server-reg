/* ========================================================================
 * Copyright (c) 2006-2007 The University of Washington
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

/* EDS form handler */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "pgdb.h"
#include "uwldap.h"
#include "srext.h"

extern char *remote_user;
extern int errno;
char *errstr = NULL;

#define REQ(s) (req && !strcmp(req,s))


// check form of auth group
static int chk_group_name(WebTemplate W, char *ag) {
   if (strncmp(ag, "u:", 2) && strncmp(ag, "u_", 2)) {
      WebTemplate_assign(W, "EMSG", "Invalid group name");
      return (0);
   }
   return (1); // OK
}

// unset fields that we don't allow copy
static char *unset_fields[] = {
  "q3",
  "q5_admin_checked", "q5_dept_checked", "q5_xxx_checked", "q5_other_checked",
  "q6_basic_checked", "q6_groups_checked", "q6_uwwib_checked", "q6_student_checked", "q6_courses_checked",
  "q6_employee_checked", "q6_alumni_checked",
  "q7", "q13",
   NULL
};
static void unset_nocopy(WebTemplate W)
{
   char **q;
   for (q=unset_fields;*q;q++) WebTemplate_assign(W, *q, "");
}


// set the 'authorizations' part of the detail page header
static void set_authstatus(WebTemplate W, Application *app)
{
   int a;

   if (!app) return;

   for (a=0;a<SR_AUTH_NUM; a++) {
      if (!app->auth_st[a]) continue;
      
      WebTemplate_assign(W, "auth_want", sr_auth_def[a].fname);
      if (app->auth_st[a]&SR_AUTH_ST_OK) {
          WebTemplate_assign(W, "auth_status", "authorized");
       } else if (app->auth_st[a]&SR_AUTH_ST_REQ) {
          WebTemplate_assign(W, "auth_status", "pending");
       } else WebTemplate_assign(W, "auth_status", "unknown");
       WebTemplate_parse_dynamic(W, "form.auth_show");
   }
   WebTemplate_assign(W, "last_updater", app->userid);
   WebTemplate_assign(W, "last_update", ctime(&app->rsp_date));
   // WebTemplate_parse_dynamic(W, "form.old_form");
   
}

/* Save the auth requests out of the form data.  */

static int save_auth_requests(WebTemplate W, char *cn,  Application *app)
{
   char **q6 = WebTemplate_get_arg_list(W, "q6");
   char **p;
   Authorization **auths = (Authorization**) malloc(sizeof(Authorization*)*(SR_AUTH_NUM+1));
   int ret;
   int authn = 0;
   Authorization *auth;
   int a;

   memset(auths, 0, sizeof(Authorization*)*(SR_AUTH_NUM+1));

   // seed with existing auths - except requests
   if (app) {
      for (a=0;a<SR_AUTH_NUM;a++) {
         if (app->auth_st[a]) {
            auth = db_new_authorization(cn, a);
            auth->status = app->auth_st[a] & ~SR_AUTH_ST_REQ;
            auth->req_id = strdup(remote_user);
            auths[authn++] = auth;
         }
      }
   }

   // adjust for this form's requests
   for (p=q6; p && *p; p++) {
      char *v = *p;
      int n;
      if (strncmp(v, "q6_", 3)) continue;
      v += 3;
      for (a=0;a<SR_AUTH_NUM;a++) if (!strcmp(v, sr_auth_def[a].name)) break;
      if (a==SR_AUTH_NUM) continue;
      for (n=0;n<authn;n++) if (auths[n]->auth_code==a) break;
      if (n==authn) {
          auth = db_new_authorization(cn, a);
          auths[authn++] = auth;
      } else auth = auths[n];
      auth->status |= SR_AUTH_ST_REQ;
      auth->req_id = strdup(remote_user);
   }
   auths[authn] = NULL;
   ret = db_update_authorizations(auths);
   return (ret);
}

// standard way to print and exit
static void showpage_and_exit(WebTemplate W)
{
  WebTemplate_parse(W, "PAGE", "page");
  WebTemplate_write(W, "PAGE");
  db_disconnect();
  exit (0);
}

// print a standalone error page and exit
static void error_page(WebTemplate W, char *block, char *back)
{
   WebTemplate_assign(W, "PAGETITLE", "EDS Registration: error");
   WebTemplate_get_by_name(W, "reply", "req_reply.tpl");
   WebTemplate_parse_dynamic(W, block);
   if (back) {
      WebTemplate_assign(W, "BACK", back);
      WebTemplate_parse_dynamic(W, "reply.goback");
   }
   WebTemplate_parse(W, "BODY", "reply");
   showpage_and_exit(W);
}
     
/* copy form data to page */
static void preserve_input_data(WebTemplate W, Application *app)
{
  char *an;
  char *av;
  int a;

  /* scan form values and add any 'q*' to output */
  for (a=0;;) {
     if (!(an=WebTemplate_get_next_arg(W, &a, &av))) break;
     if (*an != 'q') continue;  // not one of the questions
     if (strncmp(an, av, strlen(an))) {
        WebTemplate_assign(W, an, WebTemplate_text2html(av));
     } else {
        char *ck = (char*) malloc(strlen(av)+20);
        sprintf(ck, "%s_checked", WebTemplate_text2html(av));
        WebTemplate_assign(W, ck, "checked");
     }
     free(an);
     if (av) free(av);
  }
  if (app) set_authstatus(W, app);
}


main()
{
  WebTemplate W = newWebTemplate();
  char *req;
  char *seqstr;
  int s;
  char *xseq;
  char *doc_root = URLROOT;
  int i_am_approved = 0;
  char *cn;
  char *ag;
  int alp;
  char *onr;
  char *ags;
  char *path;
  char mailsub[512];
  char *rsp = NULL;
  char *gets_mail;
  char *from_mail;
  char *m;
 
  char *preapp_group = "u:cac:server-req.eds.preapproved";

  WebTemplate_set_comments(W, "#", NULL);
  WebTemplate_get_by_name(W, "page", "../page.tpl");

  if (get_config(path_name(EDS_CONFIG))) {
     WebTemplate_assign(W, "EMSG", "Configuration error");
     showpage_and_exit(W);
  }

  /* Get arguments */

  WebTemplate_get_args(W);
  if (m=strchr(remote_user, '@')) *m = '\0';

  if (strlen(remote_user)>12) remote_user[11] = '\0';
  req = WebTemplate_get_arg(W, "req");
  xseq = WebTemplate_get_arg(W, "xseq");
  cn = WebTemplate_get_arg(W, "cn");
  if (cn && (strlen(cn)>254)) cn[254] = '\0';
  if (cn) fix_dns_name(cn);

  WebTemplate_assign(W, "TITLE",
             "UW EDS Client Application Registration");
  WebTemplate_assign(W, "CURPAGE", "PCauth home");
  xsec_set_valid_form(W);
  WebTemplate_assign(W, "ROOT", doc_root);
  WebTemplate_assign(W, "REQ", req);

  WebTemplate_parse_dynamic(W, "page.not_admin");

  crumb_home(W);

  /* rest of page depends on request */

// use this as an easy way to send an error and branch to another page
#define EMSG_RETRY(m,r) { WebTemplate_assign(W, "EMSG", m); req=r; break; }

  /* send the 'services:about' page */

  while (REQ("help")) {

     WebTemplate_assign(W, "PAGETITLE", "EDS Help");
     WebTemplate_get_by_name(W, "help", "help.tpl");
     add_crumb(W, NULL, NULL, "EDS");
     WebTemplate_parse(W, "BODY", "help");
     showpage_and_exit(W);
 
  }

  /* Receive a completed application form */

  while (REQ("sub")) {

     char *cc_mail = NULL;
     char *cmp;
     char **cn_owners;
     int ncn_owners;
     char **cop;
     Application *app;
     char *mail_msg;
     char *form_text;
     WebTemplate M = newWebTemplate();

     WebTemplate_get_by_name(W, "reply", "req_reply.tpl");

     WebTemplate_assign(W, "PAGETITLE", "EDS application received ");

     /* Check for valid form.  Errors here can be terse
        as they should have been cought before. */

     // if (strcmp(remote_user, "fox") && !xsec_chk_valid_form(W)) showpage_and_exit(W);
     if (!xsec_chk_valid_form(W)) showpage_and_exit(W);
     if ((!cn) || (!*cn)) showpage_and_exit(W);
     WebTemplate_assign(W, "CN", cn);
     app = db_get_application(PROJECT, cn);

#ifdef APPS_HAVE_OWNERS
     /* user is ok if they own domain or are in owner group */
     onr = chk_netacts(cn, strlen(cn), remote_user);
     if (onr) {
        if (!app) error_page(W, "reply.noperm", NULL);
        if (app->app_owner) {
              if (!sr_is_member(remote_user, app->app_owner)) error_page(W, "reply.noperm", NULL);
        } else error_page(W, "reply.noperm", NULL);
     }
#else 
     /* ok if valid dns name */
     // if (onr=chk_dns(cn,0)) EMSG_RETRY(onr, "req_2");
     if (onr=chk_netacts(cn,strlen(cn),remote_user)) EMSG_RETRY(onr, "req_2");
#endif

     // should check for required elemments

     if (!(form_text=db_save_form(W, PROJECT, cn, "req_log.tpl"))) EMSG_RETRY("Error saving the application", "req_2");

     add_crumb(W, NULL, NULL, "EDS");
     save_auth_requests(W, cn, app);
     WebTemplate_parse_dynamic(W, "reply.submitted");
     write_log(LOG, remote_user, "request", cn);

     sprintf(mailsub, "EDS req for %s", cn);
     if (!(gets_mail=sr_config->eds_req_to)) gets_mail = "fox@washington.edu";

     // cc the domain owners 
     cn_owners = get_dns_owners(W, cn, &ncn_owners);
     cc_mail = (char*) malloc(ncn_owners*64);
     for (cmp=cc_mail,cop=cn_owners; cop&&*cop; cop++) {
        if (cmp!=cc_mail) *cmp++ = ',';
        strcat(strcpy(cmp, *cop), "@uw.edu");
        for (; *cmp; cmp++);
     }
     *cmp = '\0';

     WebTemplate_set_comments(M, "#", NULL);
     WebTemplate_get_by_name(M, "mail", "mailer.tpl");

     WebTemplate_assign(M, "CN", cn);
     WebTemplate_assign(M, "FORM_TEXT", form_text);
     WebTemplate_parse_dynamic(M, "mail.action");
     WebTemplate_parse(M, "msg", "mail");
     // admin's email is from the user 
     from_mail = (char*) malloc(64);
     sprintf(from_mail, "%s@uw.edu", remote_user);
     if (mail_msg = WebTemplate_macro_value(M, "msg")) {
        send_mail_cc(gets_mail, NULL, mailsub, mail_msg, from_mail);
     }
     // owners' email is from serverreg
     free(from_mail);
     if (!(from_mail=sr_config->eds_from)) from_mail = "iam-support@uw.edu";
     WebTemplate_get_by_name(M, "nmail", "mailer.tpl");
     WebTemplate_parse_dynamic(M, "nmail.noaction");
     WebTemplate_parse(M, "msg", "nmail");
     if (mail_msg = WebTemplate_macro_value(M, "msg")) {
        send_mail_cc(cc_mail, NULL, mailsub, mail_msg, from_mail);
     }

     WebTemplate_parse(W, "BODY", "reply");
     showpage_and_exit(W);
       
  }

  /* Registration part two: questions */

  while (REQ("req_2") || REQ("req_2_tc") || REQ("req_2_ac")) {

     char *crm1;
     Application *app;
     char *v_q1u = NULL;
     char *v_q2u = NULL;

     WebTemplate_assign(W, "PAGETITLE", "EDS Registration: questionnaire");
     WebTemplate_get_by_name(W, "form", "req_2.tpl");

     if ((!cn) || (!*cn)) EMSG_RETRY("You must enter a DNS name", "req_1");
#ifdef APPS_HAVE_OWNERS
     /* user is ok if they own domain or are in owner group */
     onr = chk_netacts(cn, strlen(cn), remote_user);
#else 
     /* ok if valid dns name */
     // if (onr=chk_dns(cn,0)) EMSG_RETRY(onr, "req_1");
     if (onr=chk_netacts(cn,strlen(cn),remote_user)) EMSG_RETRY(onr, "req_1");
#endif

     add_crumb(W, NULL, NULL, "EDS");
     crm1 = (char*) malloc(strlen(cn)+24);
     sprintf(crm1, "req=req_1&cn=%s", cn);
     add_crumb(W, NULL, crm1, "DNS");
       
     WebTemplate_assign(W, "CN", cn);
     WebTemplate_assign(W, "REMOTE_USER", remote_user);
     app = db_get_application(PROJECT, cn);  /* existing app data from db */

     // see if they just wanted an id looked up

     v_q1u = WebTemplate_get_arg(W, "q1u");
     v_q2u = WebTemplate_get_arg(W, "q2u");

     if (REQ("req_2_tc")) {
        preserve_input_data(W, app);
        if (v_q1u && *v_q1u) {
           WPInfo *wp = sr_ewp_info(v_q1u);
           if (wp && wp->publish && ((*wp->publish=='y')||(*wp->publish=='Y'))) {
             WebTemplate_assign(W, "q1n", wp->name);
             WebTemplate_assign(W, "q1e", wp->email);
             WebTemplate_assign(W, "q1d", wp->dept);
             WebTemplate_assign(W, "q1t", wp->title);
             WebTemplate_assign(W, "q1p", wp->phone);
           }
        }
     } else if (REQ("req_2_ac")) {
        preserve_input_data(W, app);
        if (v_q2u && *v_q2u) {
           WPInfo *wp = sr_ewp_info(v_q2u);
           if (wp && wp->publish && ((*wp->publish=='y')||(*wp->publish=='Y'))) {
             WebTemplate_assign(W, "q2n", wp->name);
             WebTemplate_assign(W, "q2e", wp->email);
             WebTemplate_assign(W, "q2d", wp->dept);
             WebTemplate_assign(W, "q2t", wp->title);
             WebTemplate_assign(W, "q2p", wp->phone);
           }
        }

     } else {

        /* If first view of form and existing app, get values from db */
        if (app) {

#ifdef APPS_HAVE_OWNERS
            if (app->app_owner) {
                if (onr && !sr_is_member(remote_user, app->app_owner)) error_page(W, "reply.noperm", crm1);
                WebTemplate_assign(W, "AG", app->app_owner);
                WebTemplate_parse_dynamic(W, "form.app_owner");
            } else if (onr) error_page(W, "reply.noperm", crm1);
#endif

            db_load_form_from_db(W, app);
            set_authstatus(W, app);

        /* If first view of form and no db maybe offer copy option */
        } else {
            // new app - see if we should copy another
            char *ccn = WebTemplate_get_arg(W, "ccn");
            if (ccn && *ccn) {
#ifdef APPS_HAVE_OWNERS
                 onr = chk_netacts(ccn, strlen(ccn), remote_user);
#endif
                 if (!(app=db_get_application(PROJECT, ccn))) error_page(W, "reply.noccn", crm1);
#ifdef APPS_HAVE_OWNERS
                 if (onr && app->app_owner && !sr_is_member(remote_user, app->app_owner)) error_page(W, "reply.noperm", crm1);
#endif
                 db_load_form_from_db(W, app);
                 unset_nocopy(W);
             } else {
                 WebTemplate_parse_dynamic(W, "form.app_copy");
             }
         }
     }

     if ((!(v_q1u&&*v_q1u))) WebTemplate_assign(W, "tc_disabled", "disabled");
     if ((!(v_q2u&&*v_q2u))) WebTemplate_assign(W, "ac_disabled", "disabled");
     WebTemplate_parse(W, "BODY", "form");
     showpage_and_exit(W);

  }

  /* send the first application form page: DNS  */

  while (REQ("req_1")) {

     WebTemplate_assign(W, "PAGETITLE", "EDS Registration: DNS");
     WebTemplate_get_by_name(W, "form", "req_1.tpl");
     // if (i_am_approved) WebTemplate_parse_dynamic(W, "form.preapp");
     WebTemplate_assign(W, "REMOTE_USER", remote_user);
     WebTemplate_assign(W, "CN", cn);
     WebTemplate_parse(W, "BODY", "form");
     add_crumb(W, NULL, NULL, "EDS");
     showpage_and_exit(W);

  }

  /* Receive a completed assignment form */

  while (REQ("rag_3")) {

     WebTemplate_get_by_name(W, "reply", "req_reply.tpl");
     add_crumb(W, NULL, NULL, "EDS");

     WebTemplate_assign(W, "PAGETITLE", "EDS assignment completed ");

     if (!xsec_chk_valid_form(W)) showpage_and_exit(W);

     ag = WebTemplate_get_arg(W, "ag");
     if (cn && *cn && (!(onr=chk_netacts(cn, strlen(cn), remote_user))) &&
               ag && *ag && chk_group_name(W, ag) ) {

        WebTemplate_assign(W, "CN", cn);
        WebTemplate_assign(W, "AG", ag);
       
        if (db_set_app_ownership(W, PROJECT, cn, ag)<0) EMSG_RETRY("Error setting ownership", "rag_2");
        WebTemplate_parse_dynamic(W, "reply.agok");
        write_log(LOG, remote_user, "setowner", cn);

     } else { /* no cn or not owner or already authorized */

        if (cn && *cn) {
            WebTemplate_assign(W, "CN", cn);
            EMSG_RETRY(onr?"Not owner":"No cn", "rag_2");
        }
     }
     WebTemplate_parse(W, "BODY", "reply");
     showpage_and_exit(W);

  }
       
  /* send group part two: group name */

  while (REQ("rag_2")) {

     char *crm1;
     Application *app;

     WebTemplate_assign(W, "PAGETITLE", "EDS Authorization group assignment");
     WebTemplate_get_by_name(W, "form", "rag_2.tpl");
     add_crumb(W, NULL, NULL, "EDS");
     crm1 = (char*) malloc(strlen(cn)+24);
     sprintf(crm1, "req=req_1&cn=%s", cn);
     add_crumb(W, NULL, crm1, "DNS");

     if ((!cn) || (!*cn)) EMSG_RETRY("You must enter a DNS name", "rag_1");
     WebTemplate_assign(W, "CN", cn);

     if (onr=chk_netacts(cn, strlen(cn), remote_user)) error_page(W, "reply.notowner", crm1); 
       
     WebTemplate_assign(W, "REMOTE_USER", remote_user);
     if (app=db_get_application(PROJECT, cn)) {
#ifdef APPS_HAVE_OWNERS
         if (app->app_owner) WebTemplate_assign(W, "AG", app->app_owner);
#endif
     }
     WebTemplate_parse(W, "BODY", "form");

     showpage_and_exit(W);

  }

  /* send the first page of a group assignment  */

  while (REQ("rag_1")) {

     WebTemplate_assign(W, "PAGETITLE", "EDS Authorization group assignment: DNS");
     WebTemplate_get_by_name(W, "form", "rag_1.tpl");
     // if (i_am_approved) WebTemplate_parse_dynamic(W, "form.preapp");
     WebTemplate_assign(W, "REMOTE_USER", remote_user);
     WebTemplate_assign(W, "CN", cn);
     WebTemplate_parse(W, "BODY", "form");
     add_crumb(W, NULL, NULL, "EDS");
     showpage_and_exit(W);

  }


  /* check a domain */

  while (REQ("dntest")) {

     WebTemplate_get_by_name(W, "reply", "req_reply.tpl");

     if (cn && *cn) {
        WebTemplate_assign(W, "CN", cn);
        if (chk_netacts(cn, strlen(cn), remote_user)) show_dn_help(W, cn);
        else WebTemplate_parse_dynamic(W, "reply.ok");
     } else {
        WebTemplate_parse_dynamic(W, "reply.nodn");
     }

     WebTemplate_parse_dynamic(W, "reply.goback");
     WebTemplate_parse(W, "BODY", "reply");
     showpage_and_exit(W);

  }

  // default page

     WebTemplate_parse_dynamic(W, "page.crumbadmin");

     WebTemplate_get_by_name(W, "menu", "menu.tpl");
     WebTemplate_parse(W, "MENU", "menu");
     WebTemplate_parse_dynamic(W, "page.menu");

     WebTemplate_assign(W, "PAGETITLE", "");
     WebTemplate_get_by_name(W, "home", "home.tpl");
     WebTemplate_parse(W, "BODY", "home");
 
     showpage_and_exit(W);
}

