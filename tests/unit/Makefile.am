## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/unit subdirectory.

include $(top_srcdir)/src/SourcesAndHeaders.am

# We need access to C Ratfor headers *and* sources sor inclusion in
# test source files here.
AM_CPPFLAGS = -I $(top_srcdir)/src

EXTRA_DIST = tests-common.h ## shared by all tests

TESTS = $(test_programs)

EXTRA_PROGRAMS = $(test_programs)
## Extra programs are not cleaned automatically.
CLEANFILES = $(EXTRA_PROGRAMS)

test_programs = ## will be updated later

## Static library used by unit tests, used to expose ratfor internals.

LDADD = librat4.a
EXTRA_LIBRARIES = librat4.a
## Extra libraries are not cleaned automatically.
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

## Define tests and their dependencies.

test_programs += test-hash-collision

## Testsuite environment and setup.
## TODO

## Testsuite cleanup.
include ../blackbox/am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../blackbox/am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../blackbox/am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
