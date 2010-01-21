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
include ../blackbox/lib/CheckListOfTests.am

# Some versions of FreeBSD make do not return a proper exit status on
# testsuite failures if more test scripts were being run in parallel.
# This could cause false negatives in the list of failed test scripts,
# which is a Very Bad Thing.  This hack offers a workaround.
check-workaround-freebsd-make-bug: check-TESTS
	@test -n "$(TEST_SUITE_LOG)" || { \
	   echo "$@: \`\$$(TEST_SUITE_LOG)' is empty or unset." >&2; \
	   echo "$@: Don't know which file is used as testsuite log" >&2; \
	   exit 1; \
	 }; \
	 test -f "$(TEST_SUITE_LOG)" && test -r "$(TEST_SUITE_LOG)" || { \
	   echo "$@: Can't find testsuite log \`$(TEST_SUITE_LOG)'" >&2; \
	   exit 1; \
	 }; \
## not all grep implementations support patterna alternation with `\|',
## so it's better to use two distinct grep invocations
	 if grep '^FAIL:' "$(TEST_SUITE_LOG)" >/dev/null || \
	    grep '^XPASS:' "$(TEST_SUITE_LOG)" >/dev/null; then \
	   exit 1; \
	 else \
	   exit 0; \
	 fi
.PHONY: check-workaround-freebsd-make-bug
check-local: check-workaround-freebsd-make-bug

## vim: ft=automake noet sw=4 ts=4
