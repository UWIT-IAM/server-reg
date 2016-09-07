# server-reg lib makefile 

include ../Makefile.macros

SUBDEST=$(DEST)/lib

LIB= $(SRLIB) $(COMMON_LIBS) -L/usr/local/pgversion/lib -Wl,-rpath=/usr/local/pgversion/lib  -lcurl
INC=-I. -I/usr/local/pgversion/include $(COMMON_INCL) -Iwebtpl

NETACT=netact.o

LIBOBJ=ckcn.o util.o srext.o uwldap.o srgr.o kscmd.o x509.o fsdb.o pgdb.o webtpl-1.16/.libs/webtpl.o

# DEBUG=-g -DMOD_UWA_DEBUG -DUWA_LDAP_DEBUG_LEVEL=9
DEBUG=-g
# LDAP_DEBUG=-DMOD_UWA_DEBUG -DUWA_LDAP_DEBUG_LEVEL=9
LDAP_DEBUG=

# URLROOT and DIRROOT are base of project

DEFINES=-DURLROOT=\"$(URL)\" -DDIRROOT=\"$(DEST)\" $(LDAP_DEBUG)

CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB) -lecpg -lcurl

SUBDIRS=

PROG=ldaptest

DATA=

all-local:	libsrtools.a $(PROG) 

install:	

libsrtools.a:   $(LIBOBJ) $(NETACT)
	@ rm -f libsrtools.a
	ar cru libsrtools.a $(LIBOBJ) $(NETACT)

ldaptest:	ldaptest.o libsrtools.a
	cc $(CFLAGS) -o ldaptest ldaptest.o $(LFLAGS) 

eds_recon:	eds_recon.o libsrtools.a
	cc $(CFLAGS) -o eds_recon eds_recon.o $(LFLAGS) 

grtest:	grtest.o libsrtools.a
	cc $(CFLAGS) -o grtest grtest.o $(LFLAGS) 

dntest:	dntest.o libsrtools.a
	cc $(CFLAGS) -o dntest dntest.o $(LFLAGS) 

mtest:	mtest.o 
	cc $(CFLAGS) -o mtest mtest.o $(LFLAGS) 

pgtest:	pgtest.o 
	cc $(CFLAGS) -o pgtest pgtest.o $(LFLAGS) 

srtest:	srtest.o 
	cc $(CFLAGS) -o srtest srtest.o $(LFLAGS) 

ksclient:	ksclient.o 
	cc $(CFLAGS) -o ksclient ksclient.o $(LFLAGS) 

