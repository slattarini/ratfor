## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
## Copyright (C) 2009, 2010 Stefano Lattarini
## Written by Stefano Lattarini <stefano.lattarini@gmail.com>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation; either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.
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
  build-aux/gitlog-to-changelog \
  build-aux/git-version-gen \
  GNUmakefile \
  maint/maint.mk \
  maint/release.mk \
  maint/developer.mk \
  ChangeLog-old \
  ChangeLog-2009 \
  $(top_srcdir)/ChangeLog
## the use of $(top_srcdir) above is required to have non-GNU make
## implementations (e.g. FreeBSD make and Solaris make) working

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
include am/CheckNoDistFiles.am

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
include am/GitGenerateChangeLog.am

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# Rule(s) to clean temporary files left around by configure/config.status.
include am/MostlyCleanConfigureTmp.am

# Maintainer-specific checks.
.PHONY: maintainer-check
maintainer-check: maintainer-check-list-of-tests

# List of directories where to recurse for testing and related
# maintainer-checks.
TEST_SUBDIRS = tests/graybox tests/blackbox

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
