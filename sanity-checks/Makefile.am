## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile to run sanity checks (or "self-tests") on the Ratfor
# Testsuite.

## Some *.am files include later might require these special variables
## to be initialized, even if only with an empty value.
MOSTLYCLEANFILES =
EXTRA_DIST =

## List of tests.

EXTRA_DIST += $(TESTS)

TESTS = \
  run_F77.test \
  run_RATFOR.test \
  run_command.test \
  timer-cc.test \
  timer.test

## Testsuite environment and setup.
include ../lib/TestsEnvironment.am

## Testsuite cleanup.
include ../lib/LocalCleanup.am

## Consistency checks on the list of tests.
include ../lib/CheckListOfTests.am

## vim: ft=automake noet sw=4 ts=4
