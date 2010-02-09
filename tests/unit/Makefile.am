## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the tests/unit subdirectory.

AUTOMAKE_OPTIONS = color-tests parallel-tests

## FIXME required by some included *.am files, hopefully it will soon be
## FIXME possible to remove this
MOSTLYCLEANFILES = ## dummy init

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
