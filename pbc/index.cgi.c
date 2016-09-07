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

/* pubcookie auth main page */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "srext.h"

extern char *remote_user;
extern int errno;
char *errstr = NULL;

char **netact_get_host_ids(char *name);
char **netact_get_domain_ids(char *name);

#define REQ(s) (req && !strcmp(req,s))

/* handle client disconnect */

void catch_pipe(int a)
{
   signal(SIGPIPE, catch_pipe);
}

/* --- form data we want to record -- */

typedef struct SvRsp__ {
  char *key;
  char *label;
  int req;
} SvRsp_, *SvRsp;


static SvRsp_ survey[] = {
  {"q1", "What resources will this server protect?", 0},
  {"q2", "Are you already authenticating users?", 0},
  {"q3", "Will use 2-factor?", 0},
  {"q4", "How many different users?", 0},
  {"q5", "How many logins on busy day?", 0},
  {"q6", "Do all users have UW NetIDs?", 0},
  {"q7", "User affiliations?", 0},
  {"q8", "How to handle authorization?", 0},
  {"q9", "Need to distinguish one affiliation from another?", 0},
  {"q10", "What server platform?", 0},
  {"q11", "SSL certificate signer?", 0},
  {"q12", "Working with UW Medicine?", 0},
  {"q13", "Additional comments?", 0},
  {NULL,NULL,0}
};


static char *form_path(char *dir, char *cn)
{
  static char path[512];
  sprintf(path, "%s/%s%c%s", DIRROOT, dir, cn?'/':'\0', cn?cn:"");
  return (path);
}

static int save_the_form(WebTemplate W, char *cn, char *path)
{
  FILE *f;
  SvRsp R;
  char **rspl, *rsp;
  char *email;
  time_t now = time(NULL);

  if (!(f=fopen(path,"w"))) {
    perror(path);
    return (0);
  }
  fprintf(f, "Server name:  %s\nRequested by: %s\nDate:         %s\n",
     cn, remote_user, ctime(&now));

  for (R=survey;R->key;R++) {
     if ((rspl=WebTemplate_get_arg_list(W, R->key)) && *rspl) {
       fprintf(f, "%s\n\n", R->label);
       for (rsp=*rspl; rsp; rsp=*(++rspl)) {
         while (*rsp) {
           char *n = strchr(rsp,'\n');
           if (n) *n = '\0';
           fprintf(f, "     %s\n", rsp);
           if (!n) break;
           rsp = ++n;
         }
       }
       fprintf(f, "\n\n");
     }
  }
  fclose(f);
  return (1);
}



main()
{
  WebTemplate W = newWebTemplate();
  char *req;
  char *seqstr;
  int seq = 0;
  int s;
  char *xseq;
  char *doc_root = URLROOT;
  int i_am_approved = 0;
  char *cn;
  int alp;
  char *onr;
  char *path;
  char mailsub[512];
  char *rsp = NULL;
  char *gets_mail;
  char *from_mail;
 
  char *preapp_group = "u_weblogin_preapproved";

  signal(SIGPIPE, catch_pipe);

  WebTemplate_set_comments(W, "#", NULL);
  WebTemplate_get_by_name(W, "page", "../page.tpl");

  /* Get arguments */

  WebTemplate_get_args(W);
  if (cn=strchr(remote_user,'@')) *cn = '\0';

  if (strlen(remote_user)>12) remote_user[11] = '\0';
  seqstr = WebTemplate_get_arg(W, "seq");
  if (seqstr) seq = atoi(seqstr);
  req = WebTemplate_get_arg(W, "req");
  xseq = WebTemplate_get_arg(W, "xseq");
  cn = WebTemplate_get_arg(W, "cn");
  if (cn && (strlen(cn)>254)) cn[254] = '\0';

  WebTemplate_assign(W, "TITLE",
             "UW Weblogin Server Registration");
  WebTemplate_assign(W, "CURPAGE", "PCauth home");
  WebTemplate_assign(W, "XSEQ", xsec_gen_token(remote_user));
  WebTemplate_assign(W, "ROOT", doc_root);
  WebTemplate_assign(W, "REQ", req);

  WebTemplate_parse_dynamic(W, "page.not_admin");

  write_log(LOG, remote_user, "req", req);

  if (sr_is_member(remote_user, preapp_group)) {
     WebTemplate_parse_dynamic(W, "page.preapp");
     i_am_approved = 1;
  }


  crumb_home(W);

  /* rest of page depends on request */

  /* send the 'services:about' page */

  if (REQ("help")) {

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Help");
     WebTemplate_get_by_name(W, "help", "help.tpl");
     WebTemplate_parse(W, "BODY", "help");
     add_crumb(W, NULL, NULL, "Weblogin");

 
  /* send a permission denied page */

  } else if (REQ("adminnop")) {

     WebTemplate_assign(W, "EMSG", "You are not permitted to administer Pubcookie");
     req = "";

  /* send a form */

  } else if (REQ("form")) {

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Server Registration");
     WebTemplate_get_by_name(W, "form", "req_form.tpl");
     if (i_am_approved) WebTemplate_parse_dynamic(W, "form.preapp");
     WebTemplate_assign(W, "REMOTE_USER", remote_user);
     WebTemplate_parse(W, "BODY", "form");
     add_crumb(W, NULL, NULL, "Weblogin");

  /* receive a form */

  } else if (REQ("sub")) {

     write_log(LOG, remote_user, "sub1", cn);
     WebTemplate_get_by_name(W, "reply", "req_reply.tpl");

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Application received ");
     if (xsec_chk_token(remote_user, xseq)) {
        WebTemplate_assign(W, "PAGE", "Bad seq? try again.");
        WebTemplate_write(W, "PAGE");
        write_log(LOG, remote_user, "fail", "seq");
        exit(1);
     }

     if (cn && *cn && (!(onr=chk_netacts(cn, strlen(cn), remote_user))) &&
              !(alp=ks_cmd("permit",cn,"test",&rsp)) ) {

        char fromaddr[64];
        WebTemplate_assign(W, "CN", cn);
        path = form_path(i_am_approved?"approved":"pending", cn);
       
        if (!save_the_form(W, cn, path)) {
          WebTemplate_parse_dynamic(W, "reply.err");

        } else if (i_am_approved) {
          int r;
          char *rsp = NULL;
          r = ks_cmd("permit",cn, NULL, &rsp);
          if (!r) {
            WebTemplate_assign(W, "EMSG", rsp);
          } else {
            WebTemplate_parse_dynamic(W, "reply.permitted");
            write_log(LOG, remote_user, "self-approve", cn);
          }
        } else {
          WebTemplate_parse_dynamic(W, "reply.submitted");
          write_log(LOG, remote_user, "sub2", cn);
        }
        sprintf(mailsub, "%s pubcookie key request for %s", 
            i_am_approved?"pre-approved":"", cn);
        char *msgtxt = get_file(path, NULL);
        char *msg = (char*) malloc(strlen(msgtxt)+64);
        sprintf(msg, "//proxy\n//requestor: %s@uw.edu\n\n%s\n", remote_user, msgtxt);
        sprintf(fromaddr, "%s@uw.edu", remote_user);

        send_mail("iam-support@uw.edu", mailsub, msg, fromaddr);

     } else { /* no cn or not owner or already authorized */

        write_log(LOG, remote_user, "fail2", cn);
        write_log(LOG, remote_user, "failonr", onr);
        write_log(LOG, remote_user, "failrsp", rsp);
        char buf[28];
        sprintf(buf, "%d", alp);
        write_log(LOG, remote_user, "failarp", buf);
        if (cn && *cn) {
          WebTemplate_assign(W, "CN", cn);
          if (onr) {
            show_dn_help(W, cn);
          }

        } else {
          WebTemplate_parse_dynamic(W, "reply.nodns");
        }
        WebTemplate_parse_dynamic(W, "reply.goback");
     }
     WebTemplate_parse(W, "BODY", "reply");
       

  /* check a domain */

  } else if (REQ("dntest")) {

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

  /* send granting cert as pem */

  } else if (REQ("gcrt")) {

     char *pem;
     int nc, nr;

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Granting Certificate");
     WebTemplate_get_by_name(W, "grant", "gcert.tpl");
     pem = get_file(DIRROOT "/grant.crt", NULL);
     WebTemplate_assign(W, "PEM", pem);
     WebTemplate_parse(W, "BODY", "grant");

  } else {

     req = "";

  }

  if (!*req) {

     WebTemplate_parse_dynamic(W, "page.crumbadmin");

     WebTemplate_get_by_name(W, "menu", "menu.tpl");
     WebTemplate_parse(W, "MENU", "menu");
     WebTemplate_parse_dynamic(W, "page.menu");

     WebTemplate_assign(W, "PAGETITLE", "");
     WebTemplate_get_by_name(W, "home", "home.tpl");
     WebTemplate_parse(W, "BODY", "home");
  }
 
  WebTemplate_parse(W, "PAGE", "page");
  WebTemplate_write(W, "PAGE");

}

