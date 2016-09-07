# groups reg makefile

include Makefile.macros

DEBUG=-g

DEFINES=-DURLROOT=\"$(URL)\" -DDIRROOT=\"$(DEST)\"
CFLAGS= $(DEBUG) $(COMMON)INCL) $(DEFINES)
LFLAGS= $(COMMON_LIBS)

SUBDIRS=lib smtp eds2 pbc

DATA=index.html 


all-local:

install:	install-sub install-data


