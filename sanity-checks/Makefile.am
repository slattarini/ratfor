## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile to run sanity checks (or "self-tests") on the Ratfor
# Testsuite.

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

TESTS = \
  run_F77.test \
  run_RATFOR.test \
  run_command.test \
  timer-cc.test \
  timer.test

## --------------------------------- ##
##  TESTSUITE ENVIRONMENT AND SETUP  ##
## --------------------------------- ##

include ../lib/TestsEnvironment.am

## ------------------- ##
##  TESTSUITE CLEANUP  ##
## ------------------- ##

include ../lib/LocalCleanup.am

## ----------------------------------------- ##
##  CONSISTENCY CHECKS ON THE LIST OF TESTS  ##
## ----------------------------------------- ##

include ../lib/CheckTestsList.am

## vim: ft=automake noet sw=4 ts=4
