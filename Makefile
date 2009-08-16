# -*- Makefile -*-
# pd ratfor77 (oz)
#
# if F77 is defined, the output
# of ratfor is Fortran 77.

# TODO: targets dist, distcheck

#--------------------------------------------------------------------------

DESTDIR =
PREFIX = /usr/local
BINDIR = $(PREFIX)/bin
MANDIR = $(PREFIX)/share/man

CC = cc
INSTALL = install -c
MKDIR_P = mkdir -p

#--------------------------------------------------------------------------

prefix = $(DESTDIR)$(PREFIX)
bindir = $(prefix)/$(BINDIR)
mandir = $(prefix)/$(MANDIR)

gnuflag = -DGNU
warnflags = -Wall -Werror -ansi
ocdefines := -c -DF77 $(gnuflag)

allobjects = rat4.o lookup.o getopt.o error.o utils.o io.o main.o

#--------------------------------------------------------------------------

all: ratfor77
.PHONY: all

ratfor77: $(allobjects) Makefile
	$(CC) $(warnflags) $(CFLAGS) $(LDFLAGS) -o $@ $(allobjects)

%.o: %.c %.h ratdef.h Makefile
	$(CC) $(ocdefines) $(warnflags) $(CFLAGS) $(CCFLSGS) -o $*.o $*.c 

#XXX: temporarly disabled warnings
rat4.o: rat4.c rat4.h ratdef.h Makefile
	$(CC) $(ocdefines) $(CFLAGS) $(CCFLSGS) -o rat4.o rat4.c 

io.o: 
utils.o: lookup.h
error.o: utils.h ratcom.h
main.o: getopt.h error.h
rat4.o: utils.h error.h getopt.h ratcom.h lookup.h io.h

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
