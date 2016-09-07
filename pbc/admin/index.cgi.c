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

/* pubcookie auth admin page */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"

extern char *remote_user;
extern char *user_list;
extern int user_list_len;
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



static char *form_path(char *dir, char *cn)
{
  char *path = (char*) malloc(48+strlen(dir)+strlen(cn));
  sprintf(path, "%s/%s%c%s", DIRROOT, dir, cn?'/':'\0', cn?cn:"");
  return (path);
}


char *id_from_form(char *form)
{
  static char id[24];
  char *p = strstr(form, "Requested by:");
  if (p) {
    p += 13;
    while (isspace(*p)) p++;
    strncpy(id,p,23);
    id[23] = '\0';
    if (p=strchr(id,'\n')) *p = '\0';
  }
  return (id);
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
  char *doc_root = "/pbc";
  char *cn;
  int alp;
  char *onr;
  char *path;
  char mailsub[512];
  char *rsp = NULL;
  struct dirent **list;
  int i,n;
  char *gets_mail;
  char *from_mail;
 
  signal(SIGPIPE, catch_pipe);

  WebTemplate_set_comments(W, "#", NULL);
  WebTemplate_get_by_name(W, "page", "../../page.tpl");

  /* Get arguments */

  WebTemplate_get_args(W);

  userid = remote_user;
  seqstr = WebTemplate_get_arg(W, "seq");
  if (seqstr) seq = atoi(seqstr);
  req = WebTemplate_get_arg(W, "req");
  xseq = WebTemplate_get_arg(W, "xseq");
  cn = WebTemplate_get_arg(W, "cn");
  if (cn && (strlen(cn)>254)) cn[254] = '\0';

  WebTemplate_assign(W, "TITLE",
             "UW Weblogin Server Authorization");
  WebTemplate_assign(W, "CURPAGE", "Administration");
  WebTemplate_assign(W, "XSEQ", xsec_gen_token(userid));
  WebTemplate_assign(W, "ROOT", doc_root);

  WebTemplate_parse_dynamic(W, "page.admin");

  crumb_home(W);
  add_crumb(W, NULL, NULL, "Weblogin user pages");

  /* rest of page depends on request */

  /* send the 'services:about' page */

  if (REQ("help")) {

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Help");
     WebTemplate_get_by_name(W, "help", "help.tpl");
     WebTemplate_parse(W, "DETAIL", "help");
     add_crumb(W, "admin/", NULL, "Admin");
     req = "skip";

 
  /* show a filled-out form */

  } else if (REQ("detail")) {

     char em[512];
     char *formtxt;

     add_crumb(W, "admin/", NULL, "Admin");

     if (formtxt = get_file(form_path("pending", cn),NULL)) {
        add_crumb(W, "admin/", "req=pending", "pending forms");
     } else if (formtxt = get_file(form_path("approved", cn),NULL)) {
        add_crumb(W, "admin/", "req=app", "approved forms");
     } else if (formtxt = get_file(form_path("denied", cn),NULL)) {
        add_crumb(W, "admin/", "req=deny", "denied forms");
     }
     WebTemplate_assign(W, "PAGETITLE", cn);
     WebTemplate_get_by_name(W, "detail", "detail.tpl");
     if (formtxt) {
        WebTemplate_assign(W, "TEXT", util_webify(formtxt));
        free(formtxt);
     } else {
        sprintf(em,"A form for %s was not found", cn);
        WebTemplate_assign(W, "EMSG", em);
     }
     WebTemplate_parse(W, "BODY", "detail");
     req = "skip";


  /* permit a host */

  } else if (REQ("ok")) {

     int r;
     char *rsp = NULL;
     char em[512];
     char *on = form_path("pending", cn);
     char *nn = form_path("approved", cn);
     char *ot = get_file(on, NULL);

     if (xsec_chk_token(remote_user, xseq)) {
        WebTemplate_assign(W, "PAGE", "Bad seq? try again.");
        WebTemplate_write(W, "PAGE");
        exit(1);
     }

     WebTemplate_assign(W, "CN", cn);
     if (!ot) {
        sprintf(em,"A form for %s was not found (path=%s)", cn, on);
        WebTemplate_assign(W, "EMSG", em);

     } else {

       /* do the authz */
       r = ks_cmd("permit",cn, NULL, &rsp);
       if (!r) {
         WebTemplate_assign(W, "EMSG", rsp);
       } else {
         sprintf(em, "Auth for %s approved by %s\n", cn, remote_user);
         WebTemplate_assign(W, "MSG", em);
         if (!(gets_mail=get_file(DIRROOT "/etc/gets_mail", NULL))) gets_mail = "fox@washington.edu";
         if (!(from_mail=get_file(DIRROOT "/etc/from_mail", NULL))) from_mail = "fox@washington.edu";
         send_mail(gets_mail, em, "That's all.\n", from_mail);
         if (onr=id_from_form(ot)) {
             char to[512];
             sprintf(to, "%s@u.washington.edu", onr);
             send_mail(to, em,
                 "You may retrieve pubcookie keys from that host.\n", from_mail);
         }
         sprintf(em,"Approved by:  %s\n", remote_user);
         if (put_file_2(nn, em, ot)) {
           WebTemplate_parse_dynamic(W, "page.permitted");
           write_log(LOG, remote_user, "approve", cn);
           unlink(on);
         } else WebTemplate_assign(W, "EMSG", "file rewrite failed");
       }
       free(ot);
     }
     req = "pending";

  /* deny a host - similar but without the auth part  */

  } else if (REQ("no")) {

     int r;
     char *rsp = NULL;
     char em[512];
     char *on = form_path("pending", cn);
     char *nn = form_path("denied", cn);
     char *ot = get_file(on, NULL);

     if (xsec_chk_token(remote_user, xseq)) {
        WebTemplate_assign(W, "PAGE", "Bad seq? try again.");
        WebTemplate_write(W, "PAGE");
        exit(1);
     }

     WebTemplate_assign(W, "CN", cn);
     if (!ot) {
        sprintf(em,"A form for %s was not found", cn);
        WebTemplate_assign(W, "EMSG", em);

     } else {

         sprintf(em,"Auth for %s denied by %s\n", cn, remote_user);
         WebTemplate_assign(W, "MSG", em);
         if (!(gets_mail=get_file(DIRROOT "/etc/gets_mail", NULL))) gets_mail = "fox@washington.edu";
         if (!(from_mail=get_file(DIRROOT "/etc/from_mail", NULL))) from_mail = "fox@washington.edu";
         send_mail(gets_mail, em, "That's all.\n", from_mail);
         if (onr=id_from_form(ot)) {
             char to[512];
             sprintf(to, "%s@u.washington.edu", onr);
             send_mail(to, em, "That's all.\n", from_mail);
         }
         sprintf(em,"Denied by:  %s\n", remote_user);
         if (put_file_2(nn, em, ot)) {
           WebTemplate_parse_dynamic(W, "page.denied");
           write_log(LOG, remote_user, "approve", cn);
           unlink(on);
         } else WebTemplate_assign(W, "EMSG", "file rewrite failed");
     }
     req = "pending";

  }

  /* Always send one of the admin main pages */

  if (REQ("app")) {

     add_crumb(W, "admin/", NULL, "Admin");

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Administration");
     WebTemplate_get_by_name(W, "list", "approved.tpl");

     n = scandir(DIRROOT "/approved", &list, 0, alphasort);
     if (n<0) perror("scandir");
     else {
        for (i=0;i<n;i++) {
           if (list[i]->d_name[0] == '.') continue;
           WebTemplate_assign(W, "CN", list[i]->d_name);
           WebTemplate_parse_dynamic(W, "list.cn");
           free(list[i]);
        }
        free (list);
     }
     if (n<=2) WebTemplate_parse_dynamic(W, "list.none");
     WebTemplate_parse(W, "BODY", "list");

  
  } else if (REQ("deny")) {

     add_crumb(W, "admin/", NULL, "Admin");
     WebTemplate_assign(W, "PAGETITLE", "Weblogin Administration");
     WebTemplate_get_by_name(W, "list", "denied.tpl");

     n = scandir(DIRROOT "/denied", &list, 0, alphasort);
     if (n<0) perror("scandir");
     else {
        for (i=0;i<n;i++) {
           if (list[i]->d_name[0] == '.') continue;
           WebTemplate_assign(W, "CN", list[i]->d_name);
           WebTemplate_parse_dynamic(W, "list.cn");
           free(list[i]);
        }
        free (list);
     }
     if (n<=2) WebTemplate_parse_dynamic(W, "list.none");
     WebTemplate_parse(W, "BODY", "list");

  
  } else if (REQ("pending")) {

     add_crumb(W, "admin/", NULL, "Admin");
     WebTemplate_assign(W, "PAGETITLE", "Weblogin Administration");
     WebTemplate_get_by_name(W, "list", "pending.tpl");

     n = scandir(DIRROOT "/pending",
                       &list, 0, alphasort);
     if (n<0) perror("scandir");
     else {
        for (i=0;i<n;i++) {
           char *form;
           char *id;
           if (list[i]->d_name[0] == '.') continue;
           WebTemplate_assign(W, "CN", list[i]->d_name);
           if (form=get_file(form_path("pending",list[i]->d_name),NULL)) {
              if (id=id_from_form(form)) WebTemplate_assign(W, "ID", id);
              free(form);
           }
           WebTemplate_parse_dynamic(W, "list.cn");
           free(list[i]);
        }
        free (list);
     }
     if (n<=2) WebTemplate_parse_dynamic(W, "list.none");
     WebTemplate_parse(W, "BODY", "list");

  } else if (!REQ("skip")) {

     WebTemplate_assign(W, "PAGETITLE", "Weblogin Administration");
     WebTemplate_get_by_name(W, "menu", "menu.tpl");
     WebTemplate_parse(W, "MENU", "menu");
     WebTemplate_parse_dynamic(W, "page.menu");
     WebTemplate_get_by_name(W, "home", "home.tpl");
     WebTemplate_parse(W, "BODY", "home");

  }

  WebTemplate_parse(W, "PAGE", "page");
  WebTemplate_write(W, "PAGE");

}

