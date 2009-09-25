## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# ratfor: public domain ratfor preprocessor, written in C.
# Makefile for the top-level directory of ratfor.

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = GNUmakefile maint.mk HACKING
EXTRA_DIST += maint/strict-distcheck.mk maint/release.mk maint/remake.mk

if RAT4_ENABLE_MAINTAINER_MAKE_RULES
  # used by maint.mk
  RAT4_ENABLE_MAINTAINER_MAKE_RULES = yes
else
  # used by maint.mk
  RAT4_ENABLE_MAINTAINER_MAKE_RULES = no
endif

SUBDIRS = src doc tests 

dist_doc_DATA = README BUGS NEWS

# This is defined here so that it can be extended in maint.mk, if needed.
distcheck-hook:

## vim: ft=automake noet sw=4 ts=4
