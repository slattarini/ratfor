## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the `src/' subdirectory.

bin_PROGRAMS = c-ratfor

# Enable as many compiler warnings as possible.  Also, make compiler
# warnings fatal, if the user aked so.
AM_CFLAGS = $(WARN_CFLAGS) $(WERROR_CFLAGS)

c_ratfor_SOURCES = \
    rat4-global.h \
	rat4-common.h \
	tok-types.h \
	lex-types.h \
	main.c \
	error.c error.h \
	io.c io.h \
	xopen.c xopen.h \
	rat4-hash.c rat4-hash.h \
	init.c init.h \
	tokenizer.c tokenizer.h \
	define.c define.h \
	include.c include.h \
	parser.c parser.h \
	labgen.c labgen.h \
	codegen.c codegen.h

## vim: ft=automake noet sw=4 ts=4
