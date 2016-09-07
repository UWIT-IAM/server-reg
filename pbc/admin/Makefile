# pbc reg makefile template

include ../../Makefile.macros

SUBDEST=$(DEST)/pbc/admin

LIB= $(SRLIB) $(COMMON_LIBS)
INC=-I. $(COMMON_INCL)

DEBUG=-g

# URLROOT and DIRROOT are base of pbc project

DEFINES=-DURLROOT=\"$(URL)/pbc\" -DDIRROOT=\"$(DEST)/pbc\"

CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB)

SUBDIRS=

PROG=index.cgi
DATA=approved.tpl  denied.tpl detail.tpl pending.tpl home.tpl menu.tpl

all-local:	$(PROG)

install:	install-prog install-data

index.cgi:	index.cgi.o $(SRDEP)
	$(CC) $(CFLAGS) -o index.cgi index.cgi.o \
	        $(LFLAGS)


