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

EXTRA_DIST += $(ALL_TESTS)

# Defined at configure time.
XFAIL_TESTS = $(RAT4_TESTSUITE_XFAIL_TESTS)

# We need this auxiliary variable to keep the list of *all* test scripts
# so that the user can override the $(TESTS) variable without clashing
# with consistency checks done by  the `check-list-of-tests' target defined
# in `CheckTestList.am'.
ALL_TESTS = \
  run_F77.test \
  run_RATFOR.test \
  run_command.test \
  timer-cc.test \
  timer.test

# Run all tests by default.
TESTS = $(ALL_TESTS)

## --------------------------------- ##
##  TESTSUITE ENVIRONMENT AND SETUP  ##
## --------------------------------- ##

include ../TestsEnvironment.am

## ------------------- ##
##  TESTSUITE CLEANUP  ##
## ------------------- ##

include ../LocalCleanup.am

## ----------------------------------------- #à
##  CONSISTENCY CHECKS ON THE LIST OF TESTS  ##
## ----------------------------------------- ##

include ../CheckTestsList.am
check-local: check-list-of-tests

## vim: ft=automake noet sw=4 ts=4
