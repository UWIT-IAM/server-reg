/* C&C pubcookie auth client definitions */

#ifndef KS_H 
#define KS_H

#define KS_MAXARG  4096

/* Location of the keyserver */
#define KS_PORT    "2222"
// #define KS_HOST    "weblogin.washington.edu"
#define KS_HOST    "webloginprodtest.cac.washington.edu"

/* cn expected in the keyserver cert */
// #define KS_CN      "weblogin.washington.edu"
#define KS_CN      "webloginprodtest.cac.washington.edu"
// need ca with comodo
#define CA_CERT    "etc/ca.crt"

/* Our cert to use */
#define CL_CERT    "etc/certs.ck"

#endif /* KS_H */
