## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/graybox subdirectory.

## ------------------------ ##
##  LOCAL AUTOMAKE OPTIONS  ##
## ------------------------ ##

AUTOMAKE_OPTIONS = color-tests parallel-tests

## --------------- ##
##  MISCELLANEOUS  ##
## --------------- ##

# Might be updated later (also in included files).  Do not remove these
# definitions.
MOSTLYCLEANFILES =
EXTRA_DIST =

## --------------- ##
##  LIST OF TESTS  ##
## --------------- ##

EXTRA_DIST += $(TESTS)

# This fails, probably for a bug in cascode() or swend() in codegen.c
XFAIL_TESTS = switch-segfault.test

TESTS = \
  switch-segfault.test \
  token-too-long.test

## --------------------------------- ##
##  TESTSUITE ENVIRONMENT AND SETUP  ##
## --------------------------------- ##

include ../blackbox/lib/TestsEnvironment.am

## ------------------- ##
##  TESTSUITE CLEANUP  ##
## ------------------- ##

include ../blackbox/lib/LocalCleanup.am

## ----------------------------------------- ##
##  CONSISTENCY CHECKS ON THE LIST OF TESTS  ##
## ----------------------------------------- ##

include ../CheckListOfTests.am

## vim: ft=automake noet sw=4 ts=4
