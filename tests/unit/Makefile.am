## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/unit subdirectory.

AM_CPPFLAGS = -I $(top_srcdir)/src

## List of tests.

EXTRA_DIST = $(TESTS)

TESTS = ## TODO

## Requirements and dependencies of tests.
## TODO

## Testsuite environment and setup.
## TODO

## Testsuite cleanup.
include ../blackbox/am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../blackbox/am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../blackbox/am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
