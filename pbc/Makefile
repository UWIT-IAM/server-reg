# pbc reg makefile template

include ../Makefile.macros

SUBDEST=$(DEST)/pbc

LIB= $(SRLIB) $(COMMON_LIBS) -lcurl
INC=-I. $(COMMON_INCL)

DEBUG=-g

# URLROOT and DIRROOT are base of pbc project

DEFINES=-DURLROOT=\"$(URL)/pbc\" -DDIRROOT=\"$(DEST)/pbc\"

CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB)

SUBDIRS=admin

PROG=index.cgi
DATA=gcert.tpl grant.crt help.tpl home.tpl menu.tpl \
     index.tpl mailform.tpl nop_adm.tpl \
     req_form.tpl req_reply.tpl saveform.tpl

all-local:	$(PROG)

install:	install-sub install-prog install-data

index.cgi:	index.cgi.o $(SRDEP)
	$(CC) $(CFLAGS) -o index.cgi index.cgi.o \
	        $(LFLAGS)


