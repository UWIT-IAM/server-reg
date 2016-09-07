/* question analysis - recording mostly */

/* single question */
typedef struct SvRsp__ {
  char *key;
  char *rsp;
  int req;
} SvRsp_, *SvRsp;


SvRsp_ survey[] = {
  {"server", "Server name", 1},
  {"Q1", "Resources protected", 1},
  {"Q2", "Sensitivity", 0},
  {"Q3", "Present auth", 0},
  {"Q4", "Present auth method", 0},
  {"Q5", "Securid", 0},
  {"Q7", "User count", 0},
  {"Q8", "Users have uwnetids", 0},
  {"Q10", "Authz", 0},
  {"Q13", "system type", 0},
  {"Q14", "Cert issuer", 0},
  {"Q15", "MITS", 0},
  {"Q16", "Other", 1},
  {NULL,NULL,0}
};


kk
