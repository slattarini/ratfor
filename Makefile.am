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
  ChangeLog \
  ChangeLog-old \
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

# Configure settings that should be preserved also in `make distcheck'
DISTCHECK_CONFIGURE_FLAGS = \
	'RATFOR=$(RATFOR)' \
	'RAT4_TESTSUITE_XFAIL_TESTS=$(RAT4_TESTSUITE_XFAIL_TESTS)'

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# Rule(s) to clean temporary files left around by configure/config.status.
include lib/MostlyCleanConfigureTmp.am
mostlyclean-local: mostlyclean-configure-tmp

# Generate ChangeLog file from git log messages.
gen_start_date = 2009-11-15
.PHONY: $(srcdir)/ChangeLog
$(srcdir)/ChangeLog: $(top_srcdir)/build-aux/gitlog-to-changelog
	$(AM_V_at)rm -f ChangeLog.tmp
	$(AM_V_GEN)$(top_srcdir)/build-aux/gitlog-to-changelog \
	              --since=$(gen_start_date) --author-date \
				  > ChangeLog.tmp
	$(AM_V_at)mv -f ChangeLog.tmp $@

# Force re-generation of ChangeLog.
$(srcdir)/ChangeLog: force-changelog-gen
.PHONY: force-changelog-gen
force-changelog-gen:

MOSTLYCLEANFILES = ChangeLog.tmp
MAINTAINERCLEANFILES = $(srcdir)/ChangeLog

# $(ENABLE_MAINTAINER_MAKE_RULES): variable used by maint.mk
if ENABLE_MAINTAINER_MAKE_RULES
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

## vim: ft=automake noet sw=4 ts=4
