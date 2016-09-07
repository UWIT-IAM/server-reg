# eds reg makefile template

include ../../Makefile.macros

PROJECT=eds2
SUBDEST=$(DEST)/$(PROJECT)/admin

LIB= $(SRLIB) $(COMMON_LIBS) -L/usr/local/pgversion/lib -Wl,-rpath=/usr/local/pgversion/lib
INC=-I. $(COMMON_INCL)

DEBUG=-g

# URLROOT and DIRROOT are base of project

DEFINES=-DPROJECT=\"$(PROJECT)\" -DURLROOT=\"$(URL)/$(PROJECT)\" -DDIRROOT=\"$(DEST)/$(PROJECT)\"

CFLAGS= $(DEBUG) $(INC) $(DEFINES)
LFLAGS= $(LIB) -lecpg -lcurl

SUBDIRS=

PROG=index.cgi 
DATA=requests.tpl detail.tpl home.tpl menu.tpl actions.tpl \
    mailer.tpl 

all-local:	$(PROG)

install:	install-prog install-data

index.cgi:	index.cgi.o $(SRDEP)
	$(CC) $(CFLAGS) -o index.cgi index.cgi.o \
	        $(LFLAGS)


