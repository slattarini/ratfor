## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/graybox subdirectory.

AUTOMAKE_OPTIONS = color-tests parallel-tests

## Some *.am files include later might require these special variables
## to be initialized, even if only with an empty value.
MOSTLYCLEANFILES =
EXTRA_DIST =

## List of tests.

EXTRA_DIST += $(TESTS)

# This fails, probably for a bug in cascode() or swend() in codegen.c
XFAIL_TESTS = switch-segfault.test

TESTS = \
  switch-segfault.test \
  token-too-long.test

## Testsuite environment and setup.
include ../blackbox/lib/TestsEnvironment.am

## Testsuite cleanup.
include ../blackbox/lib/LocalCleanup.am

## Consistency checks on the list of tests.
include ../CheckListOfTests.am

## vim: ft=automake noet sw=4 ts=4
