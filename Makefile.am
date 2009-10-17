## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Public Domain Ratfor preprocessor, written in C.
# Makefile for the top-level directory.

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = \
  xfail-tests.txt \
  GNUmakefile \
  maint.mk maint/strict-distcheck.mk maint/release.mk \
  HACKING

if ENABLE_MAINTAINER_MAKE_RULES
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

SUBDIRS = src doc blackbox-tests

dist_doc_DATA = README BUGS NEWS

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

## vim: ft=automake noet sw=4 ts=4
