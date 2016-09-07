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

/* CA statistics */


#include <string.h>
#include <stdio.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "ezp.h"
#include "db.h"
#include "webtpl.h"


/* Start of the CA was Sat Mar  1 2003 */

#define START_OF_CA 1046505600
#define SPD (60*60*24)

int debug = 0;

int *cc = NULL;  /* concurrent cert counts */

/* plot parameters */

int cc_base = START_OF_CA;

V *newV(int steps)
{
   V *v = (V*) malloc(sizeof(V)*steps);
   memset (v, 0, sizeof(V)*steps);
   return (v);
}


int ncrt = 0;
int nrev = 0;

/* By domain counter */

typedef struct DomCnt__ {
  struct DomCnt__ *next;
  char *dom;
  int cnt;
} DomCnt_, *DomCnt;

DomCnt_ domroot_ = {NULL, "", 0};
DomCnt domroot = &domroot_;

void step_dom(char *dom) {
  DomCnt D, L;
  for (D=domroot,L=NULL;D;L=D,D=D->next) if (!strcmp(D->dom,dom)) break;
  if (D) D->cnt++;
  else {
    D = (DomCnt) malloc(sizeof(DomCnt_));
    D->next = NULL;
    D->dom = strdup(dom);
    D->cnt = 1;
    L->next = D;
  }
}

char *getdom(char *addr) {
  char *d = strdup(addr);
  char *p;
  for (p=d;*p;p++);
  for (p--;(p>d)&&(*p!='.');p--);
  for (p--;(p>d)&&(*p!='.');p--);
  for (p--;(p>d)&&(*p!='.');p--);
  return (p+1);
}

/* scan all db records and make the plot */

int mk_cc_plot(int wd, int ht, char *title, char *pf) {

   SvrReq R = (SvrReq) malloc(sizeof(SvrReq_));
   SvrCert S = (SvrCert) malloc(sizeof(SvrCert_));

   time_t now = time(NULL);
   int s;
   int ncc;
   int id, ed;
   V *cct; /* total */
   V *ccc; /* ua */
   V *cco; /* other */
   V *ccd;
   int i;

   EZP_Image ei;
   EZP_Graph g1;
   EZP_Color bgc, fgc;
   EZP_Color bcct, bccc, bcco;
   EZP_Style sct, scc, sco;

   /* accumulate data */

   ncc = (now-cc_base) / SPD;   /* one per day */
   if (debug) fprintf(stderr, "base = %d\n ncc = %d\n", cc_base, ncc);

   cct = newV(ncc+1);
   ccc = newV(ncc+1);
   cco = newV(ncc+1);
   ccd = newV(ncc+1);
   for (i=0;i<=ncc;i++) ccd[i].i = cc_base + (i*SPD);

   db_connect();
   db_select_req(0, NULL, 0, 0);
   while (db_get_req(R)) {
      if (!R->crtno) continue;
      db_select_crt(R->crtno, 0, 0);
      if (db_get_crt(S)) {
        int ua = 0;
        if (S->issue_date<cc_base) S->issue_date = cc_base;
        id = (S->issue_date - cc_base) / SPD;
        ed = (S->expire_date - cc_base) / SPD;
        if (strstr(R->dn,".cac.")) ua = 1;
        if (strstr(R->dn,".u.")) ua = 1;
        for (i=id; i<ed && i<ncc; i++) {
           if (ua) ccc[i].i++;
           else cco[i].i++;
           cct[i].i++;
        }
        step_dom(getdom(R->dn));
        ncrt++;
        if (S->revoke_date && (S->expire_date>now)) nrev++;
      }
      db_free_crt();
   }

   /* Plot the results */

 if (pf) {
   ezp_verbose = debug;

   fprintf(stderr,"# certs:  %d \n", ncrt);

   ei = ezp_create_image (wd, ht);
   bgc = ezp_allocate_rgbcolor(ei, 255,245,235);  /* bg color */
   fgc = ezp_allocate_rgbcolor(ei, 0,0,0);        /* fg (text) color */
   bcct = ezp_allocate_rgbcolor(ei, 0,0,255);     /* total color */
   bccc = ezp_allocate_rgbcolor(ei, 34,139,34);   /* ua color */
   bcco = ezp_allocate_rgbcolor(ei, 255,0,0);     /* other color */

   if (title) ezp_image_title(ei, title, fgc);
   
   g1 = ezp_create_graph(ei, EZP_TYPE_TIME, EZP_TYPE_INT);

   ezp_graph_captions(g1, "Date", "# certificates", fgc);

   sct = ezp_create_style(EZP_STYLE_LINE, 3, 3, bcct);
   scc = ezp_create_style(EZP_STYLE_LINE, 3, 3, bccc);
   sco = ezp_create_style(EZP_STYLE_LINE, 3, 3, bcco);
   
   ezp_plot_xy(g1, ccd, cct, ncc, sct, "Total");
   ezp_plot_xy(g1, ccd, ccc, ncc, scc, "UA (cac | u)");
   ezp_plot_xy(g1, ccd, cco, ncc, sco, "other");

   ezp_save_image(ei,  EZP_IMAGE_PNG, pf);
 }

}

char *prog;
void usage()
{
   fprintf(stderr,"usage: %s [-d] [-p plotfile]\n", prog);
   exit(1);
}


#define GETARG if (++argv,--argc<0) usage()
#define ARG argv[0]

main(argc, argv)
int	argc;
char	*argv[];
{
   int	i, j, c;

   char *sum_file = NULL;

   char *plot_title = "UW Services CA certificates in use";
   char *plot_file = NULL;
   int plot_width = 500;
   int plot_height = 300;

/* parse arguments */

   while (--argc > 0) {
      argv++;
      if (argv[0][0]=='-') {
        switch (argv[0][1]) {

          case 'd':
             debug = 1;
             break;

          case 'p':
             switch (argv[0][2]) {
              case 'w':
                 GETARG;
                 plot_width = atoi(ARG);
                 if (plot_width<50) usage();
                 break;
              case 'h':
                 GETARG;
                 plot_height = atoi(ARG);
                 if (plot_height<50) usage();
                 break;
              case 'f':
                 GETARG;
                 plot_file = strdup(ARG);
                 break;
              case 't':
                 GETARG;
                 plot_title = strdup(ARG);
                 break;
              default: usage();
             }
             break;

          case 's':
             GETARG;
             sum_file = strdup(ARG);
             break;

          case '?':
             usage();
             exit(1);
          }
       }
   }

   mk_cc_plot(plot_width, plot_height, plot_title, plot_file);

   if (sum_file) {
      WebTemplate W = newWebTemplate();
      time_t now = time(NULL);
      char ibuf[12];
      int sumfd = open(sum_file,O_CREAT|O_TRUNC|O_RDWR);
      DomCnt D;

      if (!sumfd) {
         perror(sum_file);
         exit (1);
      }

      WebTemplate_get_by_name(W, "sum", "/usr/local/ca/stats/stats.tpl");
      WebTemplate_assign(W, "DATE", ctime(&now));
      sprintf(ibuf, "%d", ncrt);
      WebTemplate_assign(W, "NCRT", ibuf);
      sprintf(ibuf, "%d", nrev);
      WebTemplate_assign(W, "NREV", ibuf);
      for (D=domroot;D;D=D->next) {
         if (D->cnt==0) continue;
         WebTemplate_assign(W, "DNAME", D->dom);
         sprintf(ibuf, "%d", D->cnt);
         WebTemplate_assign(W, "DCNT", ibuf);
         WebTemplate_parse_dynamic(W, "sum.dom");
      }
      WebTemplate_parse(W, "SUM", "sum");
      WebTemplate_set_output(W, sumfd);
      WebTemplate_set_noheader(W);
      WebTemplate_write(W, "SUM");
      close(sumfd);
   }
}


