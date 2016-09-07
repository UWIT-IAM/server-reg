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

/* CA mail tester

   */


#include <stdio.h>
#include <signal.h>
#include <string.h>

#include "util.h"


extern int errno;


static char *form_path(char *dir, char *cn)
{
  static char path[512];
  sprintf(path, "/usr/local/server-reg/pubcookie/%s/%s", dir, cn);
  return (path);
}





main(int argc, char **argv)
{
  char *path;
  int rc;

  path = form_path("forms", "dogfood.cac.washington.edu");
  
  rc = send_mail("fox@u.washington.edu", "Mtest",  get_file(path, NULL), "fox@cac.washington.edu");

  printf("send = %d\n", rc);
}

