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

#
# ChangeLog generation, from Git commit messages.
#

# Commits before this date shouldn't be put in the ChangeLog
changelog_start_date = 2009-11-14

# Use the commit dates of authors, not of commiters.
gitlog_to_changelog_extra_options = --author-date

dist-hook: dist-hook-generate-changelog
.PHONY: dist-hook-generate-changelog
dist-hook-generate-changelog:
	$(AM_V_GEN)(set -u) >/dev/null 2>&1 && set -u; set -e; \
	case '$(FORCE_CHANGELOG_GENERATION)' in \
	  y|Y|yes|YES|true|1) force_chlog_gen=yes;; \
	                   *) force_chlog_gen=no;; \
	esac; \
	if test ! -d '$(top_srcdir)/.git'; then \
## probably the srcdir is from an extracted tarball
	  if test x"$$force_chlog_gen" = x"yes"; then \
## we must (re)generate the ChangeLog from git history, but no
## git repository is available: give up
	    echo "No .git directory found in top_srcdir \`$(top_srcdir)'" >&2; \
	    echo "Cannot generate ChangeLog." >&2; \
	    exit 1; \
	  elif test ! -f '$(top_srcdir)/ChangeLog'; then \
## no way to generate a new ChangeLog, nor to reuse the old one or an
## hand-written one: give up
	    echo "No .git directory and no ChangeLog file found in" >&2; \
	    echo "top_srcdir \`$(top_srcdir)'" >&2; \
	    echo "Cannot distribute ChangeLog." >&2; \
	    exit 1; \
	  else \
## assume that either the existing ChangeLog is up-to-date, or that the
## user properly updated it by hand
	    cp -f '$(top_srcdir)/ChangeLog' $(distdir)/cl.tmp; \
	  fi; \
	else \
## the srcdir is from a (maybe cloned) git repository, so we should be
## able to generate the ChangeLog file from git history
	  if test -n '$(gitlog_to_changelog_command)'; then \
	    gitlog2chlog='$(gitlog_to_changelog_command)'; \
	  else \
	    gitlog2chlog='$(top_srcdir)/build-aux/gitlog-to-changelog'; \
	  fi; \
	  if test -n '$(changelog_start_date)'; then \
	    changelog_start_date='$(changelog_start_date)'; \
	  else \
	    changelog_start_date='1970-01-01 UTC'; \
	  fi; \
	  GIT_DIR='$(abs_top_srcdir)/.git'; \
	  GIT_WORK_TREE='$(abs_top_builddir)'; \
	  export GIT_DIR GIT_WORK_TREE; \
	  $$gitlog2chlog \
	    --since="$$changelog_start_date" \
	    $(gitlog_to_changelog_extra_options) >$(distdir)/cl.tmp; \
	fi; \
## if all went well, move the temporary ChangeLog to its final location
	mv -f $(distdir)/cl.tmp $(distdir)/ChangeLog

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

## vim: ft=automake noet sw=4 ts=4
