## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/unit subdirectory.

## -------------------------------- ##
##  Global initialization & setup.  ##
## -------------------------------- ##

## Get the list of C Ratfor source files.
include $(top_srcdir)/src/SourcesAndHeaders.am

CLEANFILES = ## define it here so that it can be updated later

# We need access to C Ratfor headers *and* sources sor inclusion in
# test source files here.
AM_CPPFLAGS = -I $(top_srcdir)/src

# An header file shared by all tests programs.
EXTRA_DIST = tests-common.h

## -------------------------------------------------------------------- ##
##  Static library used by unit tests, used to expose ratfor internals  ##
## -------------------------------------------------------------------- ##

# For the moment, all binary programs in this directory are test
# programs, and all of them must depend from our ratfor library.
LDADD = librat4.a

EXTRA_LIBRARIES = librat4.a
# Extra libraries are not cleaned automatically.
CLEANFILES += $(EXTRA_LIBRARIES)

librat4_a_SOURCES = $(ratfor_source_names:=_.c)

$(librat4_a_SOURCES):
	$(AM_V_at)rm -f $@ $@-t
	$(AM_V_GEN)filename=`expr x'$@' : x'\(.*\)_.c$$'` \
	  && echo "/* This file has been automatically generated. " \
	          "DO NOT EDIT BY HAND! */" > $@-t \
	  && echo "#define IS_RAT4_UNITTEST 1" >> $@-t
	  && echo "#include <$$filename.c>" >> $@-t
	$(AM_V_at)chmod a-w $@-t && mv -f $@-t $@

CLEANFILES += $(librat4_a_SOURCES)

## --------------------------------------------------- ##
##  Test binary programs tests and their dependencies  ##
## --------------------------------------------------- ##

binary_test_programs = test-hash-collision

EXTRA_PROGRAMS = $(binary_test_programs)
# Extra programs are not cleaned automatically.
CLEANFILES += $(binary_test_programs)

# Currently we only have binary test, not scripts.
TESTS = $(binary_test_programs)

## ------------------------------------ ##
##  Services offered by included files  ##
## ------------------------------------ ##

## Testsuite cleanup.
include ../blackbox/am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../blackbox/am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../blackbox/am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
