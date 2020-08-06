# eds reg makefile template

include ../Makefile.macros

PROJECT=eds2
SUBDEST=$(DEST)/$(PROJECT)

LIB= $(SRLIB) $(COMMON_LIBS) -L/usr/local/pgversion/lib -Wl,-rpath=/usr/local/pgversion/lib -lcurl
INC=-I. $(COMMON_INCL)

DEBUG=-g

# URLROOT and DIRROOT are base of project

DEFINES=-DPROJECT=\"$(PROJECT)\" -DURLROOT=\"$(URL)/$(PROJECT)\" -DDIRROOT=\"$(DEST)/$(PROJECT)\"

CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB) -lecpg

SUBDIRS=admin

PROG=index.cgi
DATA=help.tpl home.tpl menu.tpl \
     index.tpl mailform.tpl nop_adm.tpl \
     req_1.tpl req_2.tpl req_reply.tpl \
     rag_1.tpl rag_2.tpl \
     req_log.tpl mailer.tpl .htaccess


all-local:	$(PROG)

install:	install-sub install-prog install-data

index.cgi:	index.cgi.o $(SRDEP)
	$(CC) $(CFLAGS) -o index.cgi index.cgi.o \
	        $(LFLAGS)


