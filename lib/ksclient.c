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

/* C&C keyser command-line client.
   This is mostly a diagnostic tool.  
*/


#include <stdio.h>
#include <string.h>

#include "ks.h"

extern int  kscmd_debug;

extern char *caport;
extern char *cahost;

/* ------------------------------------------------- */


char *prog;

void usage()
{
   fprintf(stderr, "usage: %s [-c permit|deny -n host_name] [options]\n", prog);
   fprintf(stderr, "          [-G file] [options]\n", prog);
   fprintf(stderr, "          [-p port] [-h host]\n\n");
   fprintf(stderr, "          [-t]  <test only>\n");
   fprintf(stderr, "          [-v]  <verbose>\n");
   fprintf(stderr, "          [-d]  <debug>\n");
   fprintf(stderr, "          [-x]  <no cert cn check>\n");
   
   exit(-1);
}



extern int do_cert_cn_check;

int  main(int argc, char *argv[])
{
   char   *p;
   char *arg;
   char *s;
   char *cmd = NULL;
   char *cn = NULL;
   char *test = "";
   int ret;
   char *retarg = "-null-";
   char *gfile = NULL;
  

   /* parse command line options */

   prog = argv[0];
   while (--argc) {
      arg = (++argv)[0];

      if (!strcmp(arg,"-v")) {
            kscmd_debug = 1;

      } else if (!strcmp(arg,"-d")) {
            kscmd_debug = 2;

      } else if (!strcmp(arg,"-t")) {
            test = "test";

      } else if (!strcmp(arg,"-x")) {
            do_cert_cn_check = 0;

      } else if (!strcmp(arg,"-c")) {
            if (--argc<0) usage();
            cmd = ((++argv)[0]);

      } else if (!strcmp(arg,"-n")) {
            if (--argc<0) usage();
            cn = ((++argv)[0]);

      } else if (!strcmp(arg,"-h")) {
            if (--argc<0) usage();
            cahost = ((++argv)[0]);

      } else if (!strcmp(arg,"-p")) {
            if (--argc<0) usage();
            caport = ((++argv)[0]);
            if (atoi(caport)<=0) usage();

      } else if (!strcmp(arg,"-c")) {
            if (--argc<0) usage();
            cmd = ((++argv)[0]);

      } else if (!strcmp(arg,"-G")) {
            if (--argc<0) usage();
            gfile = ((++argv)[0]);
            cmd = "getgc";

      } else usage();
   }

   if (!(cmd && (cn||gfile))) usage();


     ret = ks_cmd(cmd, cn, test, &retarg);

     if (ret) {
        fprintf(stderr, "Request successful.\n");
        if (gfile && retarg) {
           FILE *g = fopen(gfile, "w");
           if (g) {
              char *s = strstr(retarg, "OK ");
              if (s) fputs(s+3, g);
              else fprintf(stderr, "Invalid response: %s\n", retarg);
              fclose(g);
           } else {
              perror(gfile);
           }
        }
     } else {
        fprintf(stderr, "Request failed.\n");
        if (retarg) {
           fprintf(stderr,"server says: %s\n", retarg);
        }
     }

}
