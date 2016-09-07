/* ========================================================================
 * Copyright (c) 2005 The University of Washington
 *
 * $URL: https://git.s.uw.edu/ae/mailscripts/server-reg/index.cgi.c $
 * $Id: index.cgi.c | Fri Jun 6 10:38:01 2014 -0700 | Steve Hubert  $
 * $Revision: 6c9c98ab1b4fa0f07ba4714c29055c85bab2b798 $
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

/* SMTP exception request main page */

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>

#include "webtpl.h"
#include "ckcn.h"
#include "util.h"
#include "srext.h"

#define PROJ_NAME "SMTP Exception Application"

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

char *niceify(char *name)
{
   char *c;
   int m = 0;
   if (!name) return(NULL);
   for (c=name; *c; c++) {
     if (m && isupper(*c)) *c = tolower(*c);
     if (isalnum(*c)) m = 1;
     else m = 0;
   }
   return (name);
}


/* Send forms to various people */

/* These have gotten awkward, and need a different solution */

char *admin_ft_msg = "Request for SMTP exception\n\n\
  Exception type:       From/To\n\
  Requesting uwnetid:   %s\n\
  Email from address:   %s\n\
  Email to address:     %s\n\n\
//forward\n\
//requestor: %s\n\
//cc: %s\n\
//tag: type:From/To uwnetid:%s from:%s to:%s\n";

char *admin_dns_msg = "Request for SMTP exception\n\n\
  Exception type:       DNS\n\
  Requesting uwnetid:   %s\n\
  DNS name:             %s\n\
  Approving authority:  %s\n\n\
//forward\n\
//requestor: %s\n\
//cf-CustomerName: %s\n\
//cc: %s\n\
//tag: type:DNS uwnetid:%s domain:%s approver:%s\n";

char *ft_msg = "User \"%s\" has requested an exception to normal UW mail\n\
sending requirements so that mail to \"%s\"\n\
from \"%s\" can be delivered without authentication.\n\
This message was sent to the target address, though you may be reading\n\
it at another address (due to forwarding or list management).\n\
\n\
Please reply to this message to confirm this exception is desired. The \n\
exception will NOT be put into effect until confirmation is received.\n";

char *user_ft_msg = "This is a confirmation that user \"%s\" requested an\n\
exception to normal UW mail sending requirements so that mail\n\
to \"%s\" from \"%s\" can be delivered\n\
without authentication. The exception can't be put into effect\n\
until we get a reply from \"%s\" confirming\n\
that the exception is desired.\n";

char *user_dns_msg = "This is a confirmation that user \"%s\" requested an \n\
exception to normal UW mail submission requirements so that mail\n\
from \"%s\" can be delivered without authentication.\n\
The exception will be put into effect in about three to five\n\
working days unless there is an objection from \"%s\".\n";


char *dean_msg = "%s with UW NetID \"%s\" has requested\n\
an exception to normal UW mail sending requirements.\n\
Please email  \"%s@uw.edu\"\n\
if you have questions.\n\
\n\
If the exception is approved, all messages from the system at\n\
\"%s\" can be delivered without authentication.\n\
Such exceptions should not be used without considering all other\n\
options first as it does risk sending large quantities of\n\
inappropriate email if the excepted system should be compromised.\n\
\n\
%s has indicated that you have reviewed and approved\n\
this request.  You need reply to this message only if you do NOT\n\
want the requested exception to be granted.\n\
\n\
If no objection is received, then the exception will be put into\n\
effect.  It will usually take two to five working days to process.";

/* these are just the defaults */
char *gets_admin_mail = "smtp-reg@uw.edu";
char *mail_from_addr = "UW-IT <smtp-registration@uw.edu>";
char *dean_from_addr = "UW-IT <help@uw.edu>";
  
int send_smtp_ft(char *faddr, char *taddr)
{
   char mbuf[1024];
   char uaddr[64];
   
   sprintf(mbuf, admin_ft_msg, remote_user, faddr, taddr, remote_user,
           taddr, remote_user, faddr, taddr);
   send_mail(gets_admin_mail, "Request for SMTP exception", mbuf, mail_from_addr);

   /* RT handles sending these now.
   sprintf(mbuf, ft_msg, remote_user, taddr, faddr);
   send_mail(taddr, "Request for SMTP exception", mbuf, mail_from_addr);

   sprintf(mbuf, user_ft_msg, remote_user, taddr, faddr, taddr);
   sprintf(uaddr, "%s@uw.edu", remote_user);
   send_mail(uaddr, "Request for SMTP exception", mbuf, mail_from_addr);
   */
}

int send_smtp_dns(char *dns, char *dean)
{
   char mbuf[1024];
   char uaddr[64];
   char *remote_name = niceify(sr_username(remote_user));
   
   sprintf(mbuf, admin_dns_msg, remote_user, dns, dean, remote_user, remote_name,
           dean, remote_user, dns, dean);
   send_mail(gets_admin_mail, "Request for SMTP exception", mbuf, mail_from_addr);

   /* RT handles sending these now.
   sprintf(mbuf, dean_msg, remote_name, remote_user,
           remote_user, dns, remote_name);
   send_mail(dean, "Request for SMTP exception", mbuf, dean_from_addr);

   sprintf(mbuf, user_dns_msg, remote_user, dns, dean);
   sprintf(uaddr, "%s@uw.edu", remote_user);
   send_mail(uaddr, "Request for SMTP exception", mbuf, mail_from_addr);
   */
}

/* simple sanity check of address */
int ok_addr(char *a)
{
  char *s, *d;
  if (!a) return (0);
  if ((!(s=strchr(a, '@'))) || (s==a)) return (0);
  if ((!(d=strchr(a, '.'))) || (d<=s)) return (0);
  return (1);
}

void send_ft_form(WebTemplate W)
{
     add_crumb(W, NULL, NULL, PROJ_NAME);
     WebTemplate_assign(W, "PAGETITLE", "SMTP from/to exception request");
     WebTemplate_get_by_name(W, "form", "ft_form.tpl");
     WebTemplate_parse(W, "BODY", "form");
}

void send_dns_form(WebTemplate W)
{
     add_crumb(W, NULL, NULL, PROJ_NAME);
     WebTemplate_assign(W, "PAGETITLE", "SMTP DNS exception request");
     WebTemplate_get_by_name(W, "form", "dns_form.tpl");
     WebTemplate_parse(W, "BODY", "form");
}

void send_reply(WebTemplate W, char *sub)
{
     add_crumb(W, NULL, NULL, PROJ_NAME);
     WebTemplate_assign(W, "PAGETITLE", "");
     WebTemplate_get_by_name(W, "reply", "reply.tpl");
     WebTemplate_parse_dynamic(W, sub);
     WebTemplate_parse(W, "BODY", "reply");
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
  char *dns;
  int alp;
  char *onr;
  char *path;
  char mailsub[512];
  char *rsp = NULL;
  char *m;
 
  signal(SIGPIPE, catch_pipe);

  WebTemplate_set_comments(W, "#", NULL);
  WebTemplate_get_by_name(W, "page", "../page.tpl");
  WebTemplate_header(W);

  /* Get arguments */

  WebTemplate_get_args(W);
  if (m=strchr(remote_user, '@')) *m = '\0';

  if (strlen(remote_user)>12) remote_user[11] = '\0';
  seqstr = WebTemplate_get_arg(W, "seq");
  if (seqstr) seq = atoi(seqstr);
  req = WebTemplate_get_arg(W, "req");
  xseq = WebTemplate_get_arg(W, "xseq");


  WebTemplate_assign(W, "TITLE",
             "SMTP Exception Request");
  WebTemplate_assign(W, "CURPAGE", "SMTP");
  WebTemplate_assign(W, "XSEQ", xsec_gen_token(remote_user));
  WebTemplate_assign(W, "ROOT", doc_root);
  WebTemplate_assign(W, "REQ", req);

  crumb_home(W);
  /* add_crumb(W, "?req=form", "Register a server"); */

  /* add_crumb(W, "?req=help", "Help"); */

  /* rest of page depends on request */

  /* send the 'services:about' page */

  if (REQ("help")) {

     WebTemplate_assign(W, "PAGETITLE", "SMTP Help");
     WebTemplate_get_by_name(W, "help", "help.tpl");
     WebTemplate_parse(W, "BODY", "help");
     add_crumb(W, NULL, NULL, PROJ_NAME);
 

  /* send a form */

  } else if (REQ("ft")) {

     send_ft_form(W);

  } else if (REQ("dns")) {

     send_dns_form(W);

  /* receive a form */

  } else if (REQ("ftsub")) {

     char *faddr, *taddr;

     if (xsec_chk_token(remote_user, xseq)) {
        WebTemplate_assign(W, "PAGE", "Bad seq? try again.");
        WebTemplate_write(W, "PAGE");
        exit(1);
     }

     faddr = WebTemplate_get_arg(W, "fromaddr");
     taddr = WebTemplate_get_arg(W, "toaddr");
     
     if (ok_addr(faddr) && ok_addr(taddr)) {

        /* OK, send mail */
        send_smtp_ft(faddr, taddr);
        if (taddr) WebTemplate_assign(W, "TO", taddr);
        send_reply(W, "reply.thanks_ft");

     } else {
        WebTemplate_assign(W, "EMSG", "We need two valid addresses.");
        if (faddr) WebTemplate_assign(W, "FROMADDR", faddr);
        if (taddr) WebTemplate_assign(W, "TOADDR", taddr);
        send_ft_form(W);
     }

  } else if (REQ("dnssub")) {

     char *dns, *dean, *meets;
     if (xsec_chk_token(remote_user, xseq)) {
        WebTemplate_assign(W, "PAGE", "Bad seq? try again.");
        WebTemplate_write(W, "PAGE");
        exit(1);
     }

     dns = WebTemplate_get_arg(W, "dns");
     dean = WebTemplate_get_arg(W, "dean");
     meets = WebTemplate_get_arg(W, "meets");
     

     if (dns && ok_addr(dean) && meets) {


        if (onr=chk_dns(dns, 0)) {

            /* no such host */
            WebTemplate_assign(W, "CN", dns);
            WebTemplate_assign(W, "EMSG", onr);
            WebTemplate_get_by_name(W, "reply", "reply.tpl");
            WebTemplate_parse_dynamic(W, "reply.goback");
            WebTemplate_parse(W, "BODY", "reply");

        } else if (onr=chk_netacts(dns, strlen(dns), remote_user)) {

            /* not owner */
            WebTemplate_assign(W, "CN", dns);
            WebTemplate_assign(W, "EMSG", "Not owner");
            WebTemplate_get_by_name(W, "reply", "reply.tpl");
            show_dn_help(W, dns);
            WebTemplate_parse_dynamic(W, "reply.goback");
            WebTemplate_parse(W, "BODY", "reply");

        } else {

            /* OK, send mail */
            send_smtp_dns(dns, dean);
            if (dean) WebTemplate_assign(W, "DEAN", dean);
            send_reply(W, "reply.thanks_dns");
        }

     } else {
        if (dns) WebTemplate_assign(W, "DNSNAME", dns);
        else WebTemplate_assign(W, "EMSG", "You must provide a DNS name");
        if (ok_addr(dean)) WebTemplate_assign(W, "DEANADDR", dean);
        else WebTemplate_assign(W, "EMSG", "You must provide an approver's address");
        if (!meets) WebTemplate_assign(W, "EMSG", "You must assert that you meet all the requirements ");
        send_dns_form(W);
     }

  /* check a domain */

  } else if (REQ("dntest")) {

     char *dns, *dean, *meets;
     dns = WebTemplate_get_arg(W, "dns");
     dean = WebTemplate_get_arg(W, "dean");
     WebTemplate_get_by_name(W, "reply", "reply.tpl");
     if (dns && *dns) {
       WebTemplate_assign(W, "CN", dns);
       if (chk_netacts(dns, strlen(dns), remote_user)) show_dn_help(W, dns);
       else WebTemplate_parse_dynamic(W, "reply.ok");
     } else {
       WebTemplate_parse_dynamic(W, "reply.nodn");
     }
     WebTemplate_parse_dynamic(W, "reply.goback");
     WebTemplate_parse(W, "BODY", "reply");

  } else {    /* real home page */

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

