## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Top-level Makefile for the Ratfor Testsuite.

ACLOCAL_AMFLAGS = -I m4 --install

# The order *is* significant here!
SUBDIRS = lib sanity-checks tests

# List of extra files to be put in the distributed tarball.
EXTRA_DIST = \
  HACKING \
  GNUmakefile \
  ChangeLog-old \
  ChangeLog-2009 \
  build-aux/gitlog-to-changelog \
  build-aux/git-version-gen \
  maint/maint.mk \
  maint/strict-distcheck.mk \
  maint/release.mk \
  maint/developer.mk \
  maint/prj-post-cfg.mk

# List of files which must not be put in the distributed tarball.
no_dist_files = \
  maint/config.site \
  maint/local-cfg.mk \
  maint/personal/*

# Enabling of maintainer rules.
if ENABLE_MAINTAINER_MAKE_RULES
  ENABLE_MAINTAINER_MAKE_RULES = yes ## used by maint.mk
  FORCE_CHANGELOG_GENERATION = yes
else
  ENABLE_MAINTAINER_MAKE_RULES = no ## used by maint.mk
  FORCE_CHANGELOG_GENERATION = no
endif

# Check that we do not erreonously include in the tarball any file which
# is not meant for distribution.
include lib/CheckNoDistFiles.am

# Version string management for distributed tarballs.

dist-hook: dist-hook-create-tarball-version
.PHONY: dist-hook-create-tarball-version
dist-hook-create-tarball-version:
	echo '$(VERSION)' > $(distdir)/.tarball-version

# ChangeLog generation, from Git commit messages.

# Commits before this date shouldn't be put in the ChangeLog.
changelog_start_date = 2010-01-01
# Use the commit dates of authors, not of commiters.
gitlog_to_changelog_extra_options = --author-date
# Include the proper dist-hook to generate te ChangeLog.
include lib/GitGenerateChangeLog.am

# Configure settings that should be preserved also in `make distcheck'
DISTCHECK_CONFIGURE_FLAGS = \
	'RATFOR=$(RATFOR)' \
	'RAT4_TESTSUITE_XFAIL_TESTS=$(RAT4_TESTSUITE_XFAIL_TESTS)'

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# Rule(s) to clean temporary files left around by configure/config.status.
include lib/MostlyCleanConfigureTmp.am

# Maintainer-specific checks.
.PHONY: maintainer-check
maintainer-check: maintainer-check-list-of-tests

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

## vim: ft=automake noet sw=4 ts=4
