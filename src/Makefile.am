## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile for the src/ratfor subdirectory.

bin_PROGRAMS = ratfor

## we should not need to use $(BUILT_SOURCES) here
lexer.c: mklexer.sh Makefile
	$(AM_V_GEN): \
	  && rm -f $@ $@-t \
	  && $(srcdir)/mklexer.sh >$@-t \
	  && chmod a-w $@-t \
	  && mv $@-t $@
EXTRA_DIST = mklexer.sh
MOSTLYCLEANFILES = lexer.c lexer.c-t

ratfor_SOURCES = \
    rat4-global.h \
	rat4-common.h \
	lex-symbols.h \
	main.c \
	utils.c utils.h \
	error.c error.h \
	io.c io.h \
	xopen.c xopen.h \
	lookup.c lookup.h \
	init.c init.h \
	tokenizer.c tokenizer.h \
	parser.c parser.h \
	labgen.c labgen.h \
	codegen.c codegen.h

ratfor_SOURCES += lexer.h 
nodist_ratfor_SOURCES = lexer.c

## vim: ft=automake noet sw=4 ts=4
