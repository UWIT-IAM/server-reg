/* server-reg ldap interface */


/* These printf most useful when httpd run with -X */
#define PRINTF if (0) printf

#define MAX_STRING_LEN 8192

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "uwldap.h"

#include <ldap.h>
#include <lber.h>
#include <sasl/sasl.h>


static int ldap_chk_in_group(LDAP *ld, char *base, char *group, char *member, int dpth, int mdpth);
static int ldap_adjust_group(LDAP *ld, char *group_dn, char *member, int add);
// static int ldap_get_attr_by_name(LDAP *ld, char *netid, char *regid, char *attr, char ***values);
static LDAPMessage *ldap_sel_attrs_by_id(LDAP *ld, char *netid, char *regid, char **attrs);
static LDAPMessage *ldap_sel_attr_by_id(LDAP *ld, char *netid, char *regid, char *attr);
static char *ldap_get_first_attr_value(LDAP *ld, char *attr, LDAPMessage *result);
static char *ldap_get_attr_value_by_id(LDAP *ld, char *netid, char *regid, char *attr);

/**
#define UWA_LDAP_DEBUG_LEVEL 15 
 **/

/* ----- Tools -----------*/

/* malloc and copy a string, l is strlen */
static char *newstr(char *src, int l)
{
   char *s = (char*) malloc(l+1);
   strncpy(s,src,l);
   s[l] = '\0';
   return (s);
}

/* free a list of strings */
static void free_str_list(char **list)
{
   char **p;
   for (p=list; *p; p++) free(*p);
   free(list);
}

/* ----- Connection procedures  -----------*/

/* A host name, e.g. author.u.washington.edu, may actually be
   the name of a cluster.  We want the individual hosts returned
   for that cluster.  For simple auth it is just convenient;
   for kerberos auth it is necessary. 

   In addition, we allow the ldap library convention that a 
   list of hosts may be submitted by separating the names
   with spaces. */

#define MAX_LDAP_HOSTS 10  /* much more than needed? */
static int get_hosts(char **hosts, char *hostnames, int *hl, int *ht)
{
    struct hostent *ldh;
    int h;
    int nh = 0;
    char **hp;
    char *host;
    char *p;
       
    for (;;) {
       while (*hostnames && isspace(*hostnames)) hostnames++;
       if (!*hostnames) break;
       for (p=hostnames;*p && !isspace(*p); p++);
       host = newstr(hostnames, p-hostnames);

       /* get the host, may be a list */

       ldh = gethostbyname(host);
       if (!ldh) {
          perror("gethostbyname");
          continue;
       }
       *hl = ldh->h_length;
       *ht = ldh->h_addrtype;
       for (hp=ldh->h_addr_list;*hp && nh<MAX_LDAP_HOSTS;nh++,hp++) {
          char *hx = (char *)malloc(*hl);
          memcpy(hx,*hp,*hl);
          hosts[nh] = hx;
       }
       hostnames = p;
    }
    return (nh);
}
        
static void set_ldap_debug()
{
#if defined(UWA_LDAP_DEBUG_LEVEL)
  int debug = UWA_LDAP_DEBUG_LEVEL;
  if( ber_set_option( NULL, LBER_OPT_DEBUG_LEVEL, &debug )
          != LBER_OPT_SUCCESS ) {
      fprintf( stderr, "Could not set LBER_OPT_DEBUG_LEVEL %d\n", debug );
  }
  if( ldap_set_option( NULL, LDAP_OPT_DEBUG_LEVEL, &debug )
         != LDAP_OPT_SUCCESS ) {
      fprintf( stderr, "Could not set LDAP_OPT_DEBUG_LEVEL %d\n", debug );
  }
#endif /* debug */
}


/* sasl auth callback */

static int
tsasl_interact(LDAP *ld, unsigned flags, void *defaults, void *in)
{
        sasl_interact_t *interact = in;
        /* Should loop through, ++interact, for full SASL stuff. */
        if (interact->id != SASL_CB_LIST_END) {
                interact->result = (char *) interact->defresult;
                if (interact->defresult)
                        interact->len = strlen(interact->defresult);
                else
                        interact->len = 0;
        }
        return LDAP_SUCCESS;
}

/* Initialize the library.
   Connects to ldap and authenticates.
   Returns the LDAP pointer (opaque)
 */

void *uwldap_connect(char *host, char *bindcert, char *bindkey, char *ca)
{
   
    LDAP  *ld = NULL;
    LDAPMessage    *result, *e;
    int        i;
    struct hostent *ldh;
    char *hosts[MAX_LDAP_HOSTS];
    int h, nh;
    int hl, ht;
    char **hp;
    int protocol = LDAP_VERSION3;
    int port;

    PRINTF( ".. uwldap_connect\n");

    if (!bindkey)  bindkey = bindcert;
    // nh = get_hosts(hosts, host, &hl, &ht);
        
       int rc;
       // ldh = gethostbyaddr(hosts[h], hl, ht);
/**
       ldh = gethostbyname(host);
       if (!ldh) {
          perror( "ldap_init" );
          return (NULL);
       }
 **/

       port = UWLDAP_PORT;
       PRINTF( "Trying connect to %s\n", host);

       /* Get a handle to the server */

          set_ldap_debug();

          if ((ld=ldap_init(host, UWLDAP_PORT))==NULL) {
              perror( "ldap_init" );
              return (NULL);
          }

          rc = LDAP_VERSION3;
          if ( ldap_set_option( NULL, LDAP_OPT_PROTOCOL_VERSION, &rc ) != LDAP_SUCCESS ) {
              ldap_perror( ld, "ldap_set_option LDAPv3" );
              return (NULL);
          }

          rc = LDAP_OPT_X_TLS_DEMAND;
          if ( ldap_set_option( NULL, LDAP_OPT_X_TLS, &rc ) != LDAP_SUCCESS ) {
              ldap_perror( ld, "ldap_set_option LDAP_OPT_X_TLS" );
              return (NULL);
          }

          if ( ldap_set_option( NULL, LDAP_OPT_X_TLS_CACERTFILE, ca ) != LDAP_SUCCESS ) {
              ldap_perror( ld, "ldap_set_option cacert" );
              return (NULL);
          }

          PRINTF(".. ldap tls initialize ok\n");


       /* bind */

          rc = LDAP_VERSION3;
          if ( ldap_set_option( ld, LDAP_OPT_PROTOCOL_VERSION, &rc ) != LDAP_SUCCESS ) {
              ldap_perror( ld, "ldap_set_option LDAPv3" );
              return (NULL);
          }

          /* cert */
          PRINTF("using cert from %s\n", bindcert);
          if ( ldap_set_option( NULL, LDAP_OPT_X_TLS_CERTFILE, bindcert ) < 0 ) {
              ldap_perror( ld, "ldap_set_option bindcrt" );
              return (NULL);
          }

          if ( ldap_set_option( NULL, LDAP_OPT_X_TLS_KEYFILE, bindkey ) < 0 ) {
              ldap_perror( ld, "ldap_set_option bindkey" );
              return (NULL);
          }

         if ( ldap_start_tls_s(ld, NULL,NULL) != LDAP_SUCCESS) {
             ldap_perror(ld, "ldap_sasl_interactive_bind_s");
              return (NULL);
          }

         if ( ldap_sasl_interactive_bind_s(ld, NULL, "EXTERNAL", 0, 0,
                LDAP_SASL_AUTOMATIC|LDAP_SASL_QUIET, tsasl_interact, 0) != LDAP_SUCCESS) {
             ldap_perror(ld, "ldap_sasl_interactive_bind_s");
              return (NULL);
          }


       PRINTF(".. ldap start bind ok\n");

       /* use LDAPv3 */
       i = LDAP_VERSION3;
       if ( ldap_set_option( ld, LDAP_OPT_PROTOCOL_VERSION, &i ) < 0 ) {
           ldap_perror( ld, "ldap_set_option LDAPv3" );
           ldap_unbind( ld );
              return (NULL);
       }

    PRINTF(".. ldap connect OK.\n");
    return ((void*)ld);
}

/* Disconnect from the ldap server. 
 */

void uwldap_disconnect(void *ldv)
{
    ldap_unbind_s( (LDAP *)ldv );
}



/* -------- member procedures ------------------ */


/* get name by netid */

char *uwldap_get_username(void *ldv, char *netid)
{
   LDAP *ld = (LDAP*) ldv;
   char *ret = ldap_get_attr_value_by_id(ld, netid, NULL, "displayName");
   return (ret);
}


/* get regid by netid */

char *uwldap_get_regid(void *ldv, char *netid)
{
   LDAP *ld = (LDAP*) ldv;
   char *ret = ldap_get_attr_value_by_id(ld, netid, NULL, "uwRegID");
   return (ret);
}

/* get netid by regid */

char *uwldap_get_netid(void *ldv, char *regid)
{
   LDAP *ld = (LDAP*) ldv;
   char *ret = ldap_get_attr_value_by_id(ld, NULL, regid, "uwNetID");
   return (ret);
}

/* get wpinfo by netid */

static char *wpattrs[] = {
  "uwEWPName",
  "uwEWPDept1",
  "uwEWPTitle1",
  "uwEWPPhone1",
  "uwEWPEmail1",
  "uwEWPPublish",
  NULL
};

WPInfo *uwldap_get_wp_netid(void *ldv, char *netid)
{
   LDAP *ld = (LDAP*) ldv;
   char *n, *c;
   WPInfo *ret = (WPInfo*) malloc(sizeof(WPInfo));
   LDAPMessage *ldm = ldap_sel_attrs_by_id(ld, netid, NULL, wpattrs);

   if (!ldm) return (NULL);

   // name gets 'fixed'
   n = ldap_get_first_attr_value(ld, wpattrs[0], ldm);
   if (n && (c=strchr(n, ','))) {
      ret->name = (char*) malloc(strlen(n)+2);
      *c++ = '\0';
      while (*c && *c==' ') c++;
      strcpy(ret->name, c);
      strcat(ret->name, " ");
      strcat(ret->name, n);
      free(n);
   } else ret->name = n;
   ret->dept = ldap_get_first_attr_value(ld, wpattrs[1], ldm);
   ret->title = ldap_get_first_attr_value(ld, wpattrs[2], ldm);
   ret->phone = ldap_get_first_attr_value(ld, wpattrs[3], ldm);
   ret->email = ldap_get_first_attr_value(ld, wpattrs[4], ldm);
   ret->publish = ldap_get_first_attr_value(ld, wpattrs[5], ldm);
   ldap_msgfree(ldm);
   return (ret);
}


/* Test if an id is in an auth group. 
   Returns 1 if in, 0 if not.
 */

static int dotcount(char *str)
{
   int n = 0;
   for (;*str;str++) if (*str=='.') n++;
   return (n);
}
   
int uwldap_is_member(void *ldv, char *id, char *group) 
{
    int ret;

    LDAP  *ld = (LDAP*) ldv;

    char *um = (char*) malloc(strlen(id)+32);

    /* make the member attr attribute value to seek */
    if (strchr(id, '@')) sprintf(um,"eduPersonPrincipalName=%s", id);
    else if (dotcount(id)>1) sprintf(um,"cn=%s",id);
    else sprintf(um,"uwnetid=%s",id);
    ret = ldap_chk_in_group(ld, UWLDAP_GDS_BASE, group, um, 0, 10);
    free (um);
    return ret;
}
       

/* Test if a cn is in an eds auth group. 
   Returns 1 if in, 0 if not.
 */

int uwldap_is_eds_member(void *ldv, char *cn, char *group) 
{
    int ret;

    LDAP  *ld = (LDAP*) ldv;

    char *um = (char*) malloc(strlen(cn)+12);

    sprintf(um,"cn=%s",cn);
    ret = ldap_chk_in_group(ld, UWLDAP_EDS_BASE, group, um, 0, 0);
    free (um);
    return ret;
}
       

/* Add a cn tp an eds auth group. 
   Returns 1 if ok, 0 if failed.
 */

int uwldap_add_eds_member(void *ldv, char *cn, char *group) 
{
    int ret;

    LDAP  *ld = (LDAP*) ldv;

    char *um = (char*) malloc(strlen(cn)+12);
    char *dn = (char*) malloc(strlen(UWLDAP_EDS_BASE)+strlen(group)+12);

    sprintf(um,"cn=%s",cn);
    sprintf(dn,"cn=%s,%s",group, UWLDAP_EDS_BASE);
    ret = ldap_adjust_group(ld, dn, um, 1);
    free (um);
    free (dn);
    return ret;
}
       

/* Remove a cn from an eds auth group. 
   Returns 1 if ok, 0 if failed.
 */

int uwldap_remove_eds_member(void *ldv, char *cn, char *group) 
{
    int ret;

    LDAP  *ld = (LDAP*) ldv;

    char *um = (char*) malloc(strlen(cn)+12);
    char *dn = (char*) malloc(strlen(UWLDAP_EDS_BASE)+strlen(group)+12);

    sprintf(um,"cn=%s",cn);
    sprintf(dn,"cn=%s,%s",group, UWLDAP_EDS_BASE);
    ret = ldap_adjust_group(ld, dn, um, 0);
    free (um);
    free (dn);
    return ret;
}
       


/* ---- multi-level group membership tools ----- */

/* This list holds member groups that we need to look at */

typedef struct List__ {
   struct List__ *next;
   char *text;
} List_, *List;

static void listadd(List *root, char *text) 
{
   List l = (List) malloc(sizeof(List_));
   l->next = *root;
   *root = l;
   l->text = (char*) malloc(strlen(text)+1);
   strcpy(l->text, text);
}

static void listfree(List l) 
{
   List a,b;
   for (a=l;a;a=b) {
      b = a->next;
      free (a->text);
      free (a);
   }
}


/* Test if a member is in the group */

static int ldap_chk_in_group(LDAP *ld, char *base, char *group, char *member, int dpth, int mdpth) 
{
    LDAPMessage    *result, *e;
    BerElement    *ber;
    char        *a, *dn;
    char        **vals;
    int        i;
    int idok = 0;

    char *filter;
 
    List grp_mbrs = NULL;
    List gmb;
    char *attrs[8];

    if (dpth>mdpth) return (0);  /* to prevent looping */
    PRINTF("%s chk_in_group(%d): %s in %s\n", dpth, member, group);

    /* Look for the exact member match  */

    filter = (char*) malloc(strlen(group)+strlen(member)+32);
    sprintf(filter,"(&(cn=%s)(member=%s))", group, member);
       
    attrs[0] = "cn";
    attrs[1] = NULL;

    PRINTF("filter: %s\n", filter);
    if ( ldap_search_s( ld, base, LDAP_SCOPE_SUBTREE,
        filter, attrs, 0, &result ) != LDAP_SUCCESS ) {
        ldap_perror( ld, "ldap_search_s" );
        if ( result == NULL ) {
            ldap_unbind( ld );
            free(filter);
            return( 0 );
        }
    }

    for ( e = ldap_first_entry( ld, result ); e!=NULL && !idok;
          e = ldap_next_entry( ld, e ) ) {
       if ((vals = ldap_get_values( ld, e, "cn")) != NULL ) {
          /* this just for debug */
          for ( i = 0; vals[i]!=NULL && !idok; i++ ) {
             PRINTF( "cn=%s\n", vals[i] );
          }
          ldap_value_free( vals );
          idok = 1;
       }
    }
    ldap_msgfree( result );
    free (filter);

    if (idok) return (1);  /* OK, in group */

    if (mdpth==0) return (0);

    /* Next, look for any group members. */ 

    PRINTF( " not yet, check groups\n");
    filter = (char*) malloc(strlen(group)+64);
    sprintf(filter,"(&(cn=%s)(memberGroup=*))", group, member);
       
    attrs[0] = "memberGroup";
    attrs[1] = NULL;

    PRINTF("filter: %s\n", filter);
    if ( ldap_search_s( ld, UWLDAP_GDS_BASE, LDAP_SCOPE_SUBTREE,
        filter, attrs, 0, &result ) != LDAP_SUCCESS ) {
        ldap_perror( ld, "ldap_search_s" );
        if ( result == NULL ) {
            ldap_unbind( ld );
            free(filter);
            return( 0 );
        }
    }

    for ( e = ldap_first_entry( ld, result ); e!=NULL && !idok;
          e = ldap_next_entry( ld, e ) ) {
       if ((vals = ldap_get_values( ld, e, "memberGroup")) != NULL ) {
          for ( i = 0; vals[i]!=NULL && !idok; i++ ) {
             PRINTF( "memberGroup=%s\n", vals[i] );
             listadd(&grp_mbrs, vals[i]+strlen("cn")+1);
          }
          ldap_value_free( vals );
       }
    }
    ldap_msgfree( result );
    free (filter);

    /* Look for the id in the subgroups */ 

    for (gmb=grp_mbrs; gmb && !idok; gmb=gmb->next) {
       idok = ldap_chk_in_group(ld, base, gmb->text, member, dpth+1, mdpth);
    }
    listfree(grp_mbrs);

    return( idok );
}


/* Add a member to a group */

static int ldap_adjust_group(LDAP *ld, char *group_dn, char *member, int add) 
{
    LDAPMessage    *result, *e;
    BerElement    *ber;
    char        *a, *dn;
    char        **vals;
    int        i;
    int idok = 0;
    char *filter;
    char *attrs[8];
    LDAPMod mods;
    LDAPMod *modsptr[2];
    char *modval[2];

    modsptr[0] = &mods;
    modsptr[1] = NULL;
    modval[0] = member;
    modval[1] = NULL;
 
 
    PRINTF("adjust_group: %s in %s (%d)\n", member, group_dn, add);

    mods.mod_op = add?LDAP_MOD_ADD:LDAP_MOD_DELETE;
    mods.mod_type = "member";
    mods.mod_values = modval;
    i = ldap_modify_s(ld, group_dn, modsptr);
    if (i) {
       ldap_perror( ld, "ldap group" );
       return (0);
    }
    return(1);
}


/* Select attributes by regid or netid.  attrs: null terminated list  */

static LDAPMessage *ldap_sel_attrs_by_id(LDAP *ld, char *netid, char *regid, char **attrs)
{
    LDAPMessage    *result, *e;
    BerElement    *ber;
    char        *dn;
    char        **vals;
    int        i;
    int nv = 0;
    int nva = 0;
    char **rv = NULL;
    char filter[128];

    if (netid) {
       PRINTF(" sel attrs for netid (%s)\n", netid);
       sprintf(filter, "(|(uwNetID=%s)(uwPriorNetID=%s))", netid, netid);
    } else if (regid) {
       PRINTF(" sel attrs for regid (%s)\n", regid);
       sprintf(filter, "(|(uwRegID=%s)(uwPriorRegID=%s))", regid, regid);
    } else return (NULL);

    if ( ldap_search_s( ld, UWLDAP_USR_BASE, LDAP_SCOPE_SUBTREE,
        filter, attrs, 0, &result ) != LDAP_SUCCESS ) {
        ldap_perror( ld, "ldap_search_s" );
        if ( result == NULL ) {
            ldap_unbind( ld );
            return(NULL);
        }
    }
    return (result);
}

static LDAPMessage *ldap_sel_attr_by_id(LDAP *ld, char *netid, char *regid, char *attr)
{
   char *attrs[2];
   attrs[0] = attr;
   attrs[1] = NULL;
   return (ldap_sel_attrs_by_id(ld, netid, regid, attrs));
}


/* get the first value of an atribute */
static char *ldap_get_first_attr_value(LDAP *ld, char *attr, LDAPMessage *result) 
{
    LDAPMessage  *e;
    char        *dn;
    char        **vals;
    int        i;
    char *ret = NULL;

    if (e=ldap_first_entry( ld, result )) { 
        if ((vals = ldap_get_values( ld, e, attr)) != NULL ) {
            ret = strdup(vals[0]);
            ldap_value_free(vals);
        }
    }
    return (ret);
}

/* get a single value for a single attribute bu id */

static char *ldap_get_attr_value_by_id(LDAP *ld, char *netid, char *regid, char *attr)
{
   LDAPMessage *msg = ldap_sel_attr_by_id(ld, netid, regid, attr);
   char *ret = NULL;
   if (msg) ret = ldap_get_first_attr_value(ld, attr, msg);
   ldap_msgfree(msg);
   return (ret);
}
