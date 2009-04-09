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

DESTDIR =
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

CC = cc
INSTALL = install -c
MKDIR_P = mkdir -p

DIST_FILES = Makefile README BUGS ratfor.man ratfor.pdf \
             getopt.c lookup.c lookup.h rat4.c ratcom.h ratdef.h \
             test.r testw.r

prefix = $(DESTDIR)$(PREFIX)
bindir = $(prefix)/$(BINDIR)
mandir = $(prefix)/$(MANDIR)

signed_char = char
gnuflag = -DGNU
ocdefines := -c -DF77 -DS_CHAR="$(signed_char)" $(gnuflag)

all: ratfor77

ratfor77: rat4.o lookup.o getopt.o Makefile
	$(CC) -o $@ rat4.o lookup.o getopt.o

%.o: %.c Makefile
	$(CC) $(ocdefines) -o $*.o $*.c 

install:
	test -d '$(bindir)' || $(MKDIR_P) '$(bindir)'
	$(INSTALL) -m 555 ratfor77 '$(bindir)/ratfor77'
	test -d '$(mandir)man1' || $(MKDIR_P) '$(mandir)/man1'
	$(INSTALL) -m 444 ratfor.man '$(mandir)/man1/ratfor.1'

uninstall:
	rm -f '$(bindir)/ratfor77'
	rm -f '$(mandir)/man1/ratfor.1'

clean:
	rm -f ratfor77 *.o *.tmp *.tmp[0-9]
	rm -rf *.tmpdir

check test:
	@echo 'TODO!' >&2; exit 1;

.PHONY: all clean install uninstall check test 

# vim: ft=make ts=4 sw=4 et
