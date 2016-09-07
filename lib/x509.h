/* includes for x509 tools */

X509 *ssl_txt2crt(char *crtstr);
char *ssl_get_dn(X509 *crt);
char *ssl_get_issuer(X509 *crt);
int ssl_get_sn(X509 *crt);
time_t ssl_get_issue(X509 *crt);
time_t ssl_get_expire(X509 *crt);

