# -*- Makefile -*-
# Copied from SteLib at 2010-02-03 17:49:01 +0100.  DO NOT EDIT!
#
#  Contains maintainer-specific rules to create a beta or stable release.
#  Included by top-level maint.mk.
#
#  Copyright (C) 2009, 2010 Stefano Lattarini.
#  Written by Stefano Lattarini <stefano.lattarini@gmail.com>
#
#  This program is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation; either version 3 of the License, or
#  (at your option) any later version.
#
#  This program is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

# Internal variables/macros/targets follow the naming scheme `_rls_*'

_rls_ME := release.mk
_rls_make := $(MAKE) --no-print-directory

# _rls_version: $(VERSION) with some metacharacters quoted, to be used
# e.g. in grep or sed commands.
_rls_version := $(subst .,\.,$(VERSION))

# Regular expressions (*not anchored*) for acceptable version strings.
_rls_stable_version_rx := [0-9]\+\(\.[0-9]\+\)\+
_rls_alpha_version_rx := $(_rls_stable_version_rx)[a-z]\+
_rls_beta_version_rx := $(_rls_alpha_version_rx)

# Path or name of git(1).
GIT ?= git

.PHONY: vc-nodiff-check
vc-nodiff-check: git-no-diff-check git-no-diff-cached-check

.PHONY: git-no-diff-check
git-no-diff-check:
	@(unset CDPATH; cd $(srcdir) && $(GIT) diff) >$@.tmp
	@test ! -s $@.tmp || { \
	  cat $@.tmp; \
	  echo "$(_rls_ME): some local modifications not added to" \
	       "Git index" >&2; \
	  exit 1; \
	};
	@rm -f $@.tmp;
CLEAN_FILES += git-no-diff-check.tmp

.PHONY: git-no-diff-cached-check
git-no-diff-cached-check:
	@(unset CDPATH; cd $(srcdir) && $(GIT) diff --cached) >$@.tmp
	@test ! -s $@.tmp || { \
	  cat $@.tmp; \
	  echo "$(_rls_ME): there are uncommitted modifications to" \
	       "Git index" >&2; \
	  exit 1; \
	};
	@rm -f $@.tmp;
CLEAN_FILES += git-no-diff-cached-check.tmp

.PHONY: check-version-alpha check-version-beta check-version-stable
check-version-for-alpha check-version-beta check-version-stable:
	@(set -u) >/dev/null 2>&1 && set -u; \
	 reltype=`echo $@ | sed -e 's/^.*-//'`; \
	 stable_version_rx='$(_rls_stable_version_rx)'; \
	 alpha_version_rx='$(_rls_alpha_version_rx)'; \
	 beta_version_rx='$(_rls_beta_version_rx)'; \
	 eval "version_rx=\$${$${reltype}_version_rx}"; \
	 echo x'$(VERSION)' | grep "^x$${version_rx}$$" >/dev/null || { \
	   echo "$(_rls_ME): invalid version string for a $$reltype release:" \
	                    "\`$(VERSION)'" >&2; \
	   exit 1; \
	 }

.PHONY: news-up-to-date-check
news-up-to-date-check:
	@grep "^New in $(_rls_version)$$" $(srcdir)/NEWS >/dev/null || { \
	  echo "$(_rls_ME): version \`$(VERSION)' not mentioned in NEWS" >&2; \
	  exit 1; \
	 }

# FIXME: The code used to recover the tag message/annotation below is
# FIXME: awkward and hackish; find a better way to do that.
.PHONY: check-git-tag-alpha check-git-tag-beta check-git-tag-stable
check-git-tag-alpha check-git-tag-beta check-git-tag-stable:
	@reltype=`echo $@ | sed -e 's/^.*-//'`; \
	 tag=`$(GIT) tag -l 'v$(VERSION)'`; \
	 test -n "$$tag" || { \
	   echo "$(_rls_ME): git tag \`v$(VERSION)' not found" >&2; \
	   exit 1; \
	 }; \
	 exp_msg="$$reltype release $(VERSION)"; \
	 tag_msg=`$(GIT) show --format=oneline "$$tag" | grep . | \
	            sed -n -e 2p -e 2q`; \
	 if test x"$$tag_msg" != x"$$exp_msg"; then \
	   echo "$(_rls_ME): Git tag \`$$tag' has bad message" \
	                    "\`$$tag_msg'">&2; \
	   echo "$(_rls_ME): It should have message \`$$exp_msg'">&2; \
	   exit 1; \
	 fi;

.PHONY: alpha beta stable
ALL_RECURSIVE_TARGETS += alpha beta stable
alpha beta stable:
	@[ x'$(VERSION)' != x ] || { \
	  echo '$(_rls_ME): $@: $$(VERSION) is empty!' >&2; \
	  exit 1; \
	}
	$(_rls_make) GIT='$(GIT)' vc-nodiff-check
	$(_rls_make) check-version-$@
	$(_rls_make) check-git-tag-$@
	$(_rls_make) news-up-to-date-check
	$(_rls_make) distcheck

# vim: ft=make ts=4 sw=4 noet
