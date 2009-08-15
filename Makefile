# -*- Makefile -*-
# pd ratfor77 (oz)
#
# if F77 is defined, the output
# of ratfor is Fortran 77.
#
#   On sun4,            use S_CHAR="char"
#   On RS6000,          use S_CHAR="signed char"
#   On DEC3100, maybe   use S_CHAR="signed char"
#   On CRAY,            use S_CHAR="char"
#   On GNU,             use S_CHAR="char"
#

# TODO: targets dist, distcheck

#--------------------------------------------------------------------------

DESTDIR =
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

CC = cc
INSTALL = install -c
MKDIR_P = mkdir -p

<<<<<<< HEAD
DIST_FILES = Makefile README BUGS ratfor.man \
             getopt.c lookup.c lookup.h rat4.c ratcom.h ratdef.h \
             test.r testw.r
=======
#--------------------------------------------------------------------------
>>>>>>> 2307ca8... ## Starting a BIG REARGANIZATION of the source code to make it more

prefix = $(DESTDIR)$(PREFIX)
bindir = $(prefix)/$(BINDIR)
mandir = $(prefix)/$(MANDIR)

signed_char = char
gnuflag = -DGNU
ocdefines := -c -DF77 -DS_CHAR="$(signed_char)" $(gnuflag)

allobjects = rat4.o lookup.o getopt.o error.o utils.o bulk.o

#--------------------------------------------------------------------------

all: ratfor77
.PHONY: all

ratfor77: $(allobjects) Makefile
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(allobjects)

%.o: %.c %.h ratdef.h Makefile
	$(CC) $(ocdefines) $(CFLAGS) $(CCFLSGS) -o $*.o $*.c 

utils.o: lookup.h
error.o: utils.h ratcom.h
rat4.o: getopt.h ratcom.h
bulk.o: utils.h error.h getopt.h ratcom.h keywords.h

#--------------------------------------------------------------------------

install:
	test -d '$(bindir)' || $(MKDIR_P) '$(bindir)'
	$(INSTALL) -m 555 ratfor77 '$(bindir)/ratfor77'
	test -d '$(mandir)man1' || $(MKDIR_P) '$(mandir)/man1'
	$(INSTALL) -m 444 ratfor.man '$(mandir)/man1/ratfor.1'

uninstall:
	rm -f '$(bindir)/ratfor77'
	rm -f '$(mandir)/man1/ratfor.1'

.PHONY: install uninstall

#--------------------------------------------------------------------------

clean:
	rm -f ratfor77 *.o *.tmp *.tmp[0-9]
	rm -rf *.tmpdir

.PHONY: clean

#--------------------------------------------------------------------------

check test:
	@echo 'TODO!' >&2; exit 1;

.PHONY: check test 

#--------------------------------------------------------------------------

# vim: ft=make ts=4 sw=4 et
