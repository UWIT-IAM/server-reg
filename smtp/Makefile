# smtp reg makefile template

include ../Makefile.macros

SUBDEST=$(DEST)/smtp

LIB= $(SRLIB) $(COMMON_LIBS) -lcurl
INC=-I. $(COMMON_INCL)

DEBUG=-g

# URLROOT and DIRROOT are base of smtp project

DEFINES=-DURLROOT=\"$(URL)/smtp\" -DDIRROOT=\"$(DEST)/smtp\"
CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB)

SUBDIRS=

PROG=index.cgi
DATA=reply.tpl  dns_form.tpl  ft_form.tpl  help.tpl home.tpl \
    mailform.tpl menu.tpl  


all-local:	$(PROG)

install:	install-sub install-prog install-data

index.cgi:	index.cgi.o $(SRDEP)
	$(CC) $(CFLAGS) -o index.cgi index.cgi.o \
	        $(LFLAGS)


