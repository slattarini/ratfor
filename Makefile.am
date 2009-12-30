## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# C Ratfor: a ratfor preprocessor written in C.
# Makefile for the top-level directory.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src doc tests

dist_doc_DATA = README BUGS NEWS

# List of extra files to be put in the distributed tarball.
EXTRA_DIST = \
  xfail-tests.list \
  HACKING \
  ChangeLog \
  ChangeLog-old \
  build-aux/gitlog-to-changelog \
  build-aux/git-version-gen \
  GNUmakefile \
  maint/maint.mk \
  maint/strict-distcheck.mk \
  maint/release.mk \
  maint/developer.mk \
  maint/prj-post-cfg.mk

# List of files which must not be put in the distributed tarball.
no_dist_files = maint/config.site maint/local-cfg.mk maint/personal/*

# Check that we do not erreonously include in the tarball any file which
# is not meant for distribution.
dist-hook: dist-hook-check-no-dist-files
.PHONY: dist-hook-check-no-dist-files
dist-hook-check-no-dist-files:
	@list='$(no_dist_files)'; for f in $$list; do \
	  if test -r $(distdir)/$$f; then \
	    bad_dist_files="$$bad_dist_files $$f"; \
	  fi; \
	done; \
	if test -n "$$bad_dist_files"; then \
	  echo "$@: ERROR: the following files have erroneously been put" \
	       "in the distributed tarball:" >&2; \
	  for f in $$bad_dist_files; do \
	    echo "  - $$f" >&2; \
	  done; \
	  exit 1; \
	fi; \
	exit 0

# Version string management for distributed tarballs.
dist-hook: dist-hook-create-tarball-version
.PHONY: dist-hook-create-tarball-version
dist-hook-create-tarball-version:
	echo '$(VERSION)' > $(distdir)/.tarball-version

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# Rule(s) to clean temporary files left around by configure/config.status.
include MostlyCleanConfigureTmp.am
mostlyclean-local: mostlyclean-configure-tmp

# Maintainer-specific checks.
.PHONY: maintainer-check
maintainer-check: maintainer-check-list-of-tests

# List of directories where to recurse for testing and related
# maintainer-checks.
# XXX: will soon be extended also to `tests/blackbox'
TEST_SUBDIRS = tests/graybox

# Check that the list of tests given in the Makefile is equal to the
# list of all test scripts in the Automake testsuite.
.PHONY: maintainer-check-list-of-tests
maintainer-check-list-of-tests:
	@target='$@'; fail=''; \
	 CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)"; \
	 for subdir in $(TEST_SUBDIRS); do \
	   echo "Making $$target in $$subdir"; \
	   (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$target) || fail=yes; \
	 done; \
	 test -z "$$fail"

# $(ENABLE_MAINTAINER_MAKE_RULES): variable used by maint.mk
if ENABLE_MAINTAINER_MAKE_RULES
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

## vim: ft=automake noet sw=4 ts=4
