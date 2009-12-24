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

EXTRA_DIST += $(ALL_TESTS)

# This fails, probably for a bug in cascode() or swend() in codegen.c
XFAIL_TESTS = switch-segfault.test

# We need this auxiliary variable to keep the list of *all* test scripts
# so that the user can override the $(TESTS) variable without clashing
# with consistency checks done by  the `check-list-of-tests' target defined
# in `CheckTestList.am'.
ALL_TESTS = \
  switch-segfault.test \
  token-too-long.test

# Run all tests by default.
TESTS = $(ALL_TESTS)

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

include ../blackbox/lib/CheckTestsList.am
check-local: check-list-of-tests

## vim: ft=automake noet sw=4 ts=4
