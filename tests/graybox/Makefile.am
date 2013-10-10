## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/graybox subdirectory.

## List of tests.

EXTRA_DIST = $(TESTS)

XFAIL_TESTS = no-blank-lines.test

TESTS = \
  break-badargs.test \
  next-badargs.test \
  comments-no-hang.test \
  comment-too-long-in-define-ignored.test \
  comment-too-long-in-define-no-crash.test \
  no-blank-lines.test \
  plain-define-and-comments.test \
  switch-segfault.test \
  token-too-long-no-crash.test

## Requirements and dependencies of tests.

EXTRA_DIST += too-long.sh
comment-too-long-in-define-no-crash.log: too-long.sh
comment-too-long-in-define-ignored.log: too-long.sh
token-too-long-no-crash.log: too-long.sh

## Testsuite environment and setup.
include ../t/am/TestsEnvironment.am

## Consistency checks on the list of tests.
include ../t/am/CheckListOfTests.am

## vim: ft=automake noet sw=4 ts=4
