#ifndef SRLIB_UTIL_H
#define SRLIB_UTIL_H

#include <time.h>
#include "webtpl.h"

/* browser and system type */

#define BR_XXX      0
#define BR_IE       1
#define BR_NS       2
#define BR_OPERA    3
#define BR_CHIMERA  4
#define BR_OMNIWEB  5
#define BR_SAFARI   6
#define BR_GECKO    7

#define BS_XXX      0
#define BS_LINUX    1
#define BS_MAC      2
#define BS_WIN      3

typedef struct Browser__ {
  int type;
  int version;
  char *full_name;
  int system;
} Browser_, *Browser;


typedef struct SRConfig__ {
  char *eds_db;
  char *eds_user;
  char *eds_pw;
  char *eds_req_to;
  char *eds_app_to;
  char *eds_app_pds_to;
  char *eds_app_gds_to;
  char *eds_from;
} SRConfig_, *SRConfig;
  
extern SRConfig sr_config;

#define XSEC_FORM_TIMEOUT 3600
#define XSEC_KEY_FILE "etc/xsec_key2"

#define DB_CONFIG "etc/db.conf"
#define EDS_CONFIG "etc/eds.conf"
#define FORM_DIR "forms"
#define PEND_DIR "pending"
#define LOG  "log/request.log"


/* prototypes */
Browser get_browser();
int get_config(char *file);
char *get_file(char *file, int *len);
int put_file(char *file, char *str);
int put_file_2(char *file, char *s1, char *s2);
char *put_tmp_file(char *str, char *pfx);
char *date2mdy(time_t date);
char *int2dec(int i);
char *int2hex(int i);
char *util_webify(char *str);
char *util_paramify(char *txt);


char *xsec_gen_token(char *key);
int xsec_chk_token(char *key, char *token);
char *xsec_gen_token2(char *k2);
int xsec_chk_token2(char *k2, char *token);
void xsec_set_valid_form(WebTemplate W);
int xsec_chk_valid_form(WebTemplate W);
char *fix_cert_for_html(char *cert);

int send_mail(char *to, char *sub, char *msg, char *from);
int send_mail_cc(char *to, char *cc, char *sub, char *msg, char *from);
int write_log(char *name, char *user, char *action, char *cn);

long fnv_hash_32(char *str);
long long fnv_hash_64(char *str);

char *lower_case(char *str);
char *path_name(char *file);
char *base_path_name(char *file);

void add_crumb(WebTemplate W, char *path, char *args, char *name);
void crumb_home(WebTemplate W);
void add_crumb_help(WebTemplate W, char *link);

#endif

