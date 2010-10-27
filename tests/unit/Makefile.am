## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/unit subdirectory.

AM_CPPFLAGS = -I $(top_srcdir)/src

EXTRA_DIST = tests-common.h ## shared by all tests

TESTS = $(test_programs)
EXTRA_PROGRAMS = $(test_programs)

CLEANFILES = $(EXTRA_PROGRAMS) ## extra programs are cleaned automatically

test_programs = ## will be updated later

## Define tests and their dependencies.

test_programs += test-hash-collision
test_hash_collision_SOURCES = test-hash-collision.c rat4-hash_.c error_.c

## Testsuite environment and setup.
## TODO

## Testsuite cleanup.
include ../blackbox/am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../blackbox/am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../blackbox/am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
