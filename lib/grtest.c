/* CA mail tester

   */


#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "srext.h"
#include "srgr.h"


extern int errno;

main(int argc, char **argv)
{
  int rc;
  char *rsp = NULL;
  char *host;

  host = "loss.cac.washington.edu";
  rc = sr_is_member(host, GR_GROUP);
  printf("%s: rc=%d, rsp=%s\n", host, rc, rsp?rsp:"-null-");

  host = "losz.cac.washington.edu";
  rc = sr_is_member(host, GR_GROUP);
  printf("%s: rc=%d, rsp=%s\n", host, rc, rsp?rsp:"-null-");

  rc = gr_add("admin.courses.read", "u302.cac.washington.edu", &rsp);
  
  printf("rc=%d, rsp=%s\n", rc, rsp?rsp:"-null-");

  
}

