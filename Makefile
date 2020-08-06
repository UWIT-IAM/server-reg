# groups reg makefile

include Makefile.macros

SUBDEST=$(DEST)

DEBUG=-g

DEFINES=-DURLROOT=\"$(URL)\" -DDIRROOT=\"$(DEST)\"
CFLAGS= $(DEBUG) $(COMMON)INCL) $(DEFINES)
LFLAGS= $(COMMON_LIBS)

SUBDIRS=lib smtp eds2 icons

DATA=index.html 


all-local:

install:	install-sub install-data


