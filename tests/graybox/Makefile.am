## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/graybox subdirectory.

AUTOMAKE_OPTIONS = color-tests parallel-tests

## List of tests.

EXTRA_DIST = $(TESTS)

TESTS = \
  comments-no-hang.test \
  comment-too-long-in-define-ignored.test \
  comment-too-long-in-define-no-crash.test \
  plain-define-and-comments.test \
  switch-segfault.test \
  tokens-after-until.test \
  tokens-after-until_f77.test \
  token-too-long-no-crash.test

## Requirements and dependencies of tests.

EXTRA_DIST += too-long.sh
comment-too-long-in-define-no-crash.log: too-long.sh
comment-too-long-in-define-ignored.log: too-long.sh
token-too-long-no-crash.log: too-long.sh

## Testsuite environment and setup.
include ../blackbox/am/TestsEnvironment.am

## Testsuite cleanup.
include ../blackbox/am/LocalCleanup.am

## Consistency checks on the list of tests.
include ../blackbox/am/CheckListOfTests.am

## Work around a bug of FreeBSD make w.r.t. parallel tests.
include ../blackbox/am/WorkaroundParallelTests.am

## vim: ft=automake noet sw=4 ts=4
