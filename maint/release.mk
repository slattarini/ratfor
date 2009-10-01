# -*- Makefile -*-
# Copied from SteLib at 2009-09-30 22:25:33 +0200.  DO NOT EDIT!
# Contains maintainer-specific rules to create a beta or stable release.
# Included by top-level maint.mk.

# Internal variables/macros/targets follow the naming scheme `_rls_*'

_rls_ME := release.mk
_rls_make := $(MAKE) --no-print-directory

# Path or name of git(1).
GIT ?= git

# _rls_version: $(VERSION) with some metacharacters quoted, to be used
# e.g. in grep or sed commands
_rls_version := $(subst .,\.,$(VERSION))

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

.PHONY: version-for-major-check
version-ok-for-major-check:
	  @echo x'$(VERSION)' | egrep '^x[0-9]+(\.[0-9]+)+$$' >/dev/null || { \
	    echo "$(_rls_ME): invalid version string for major release:" \
		     			 "$(VERSION)" >&2; \
	    exit 1; \
	  }

.PHONY: news-up-to-date-check
news-up-to-date-check:
	@grep "^New in $(_rls_version)$$" $(srcdir)/NEWS >/dev/null || { \
	   echo "$(_rls_ME): version \`$(VERSION)' not mentioned in NEWS" >&2;\
	   exit 1; \
	 }

.PHONY: version-git-tag-is-new-check
version-git-tag-is-new-check:
	@if $(GIT) tag | grep '^v$(_rls_version) *$$' >/dev/null; then \
	  echo "$(_rls_ME): git tag \`v$(VERSION)' already exists" >&2; \
	  exit 1; \
	fi

.PHONY: alpha beta major
ALL_RECURSIVE_TARGETS += alpha beta major
alpha beta major:
	$(_rls_make) GIT='$(GIT)' vc-nodiff-check
	@[ x'$(VERSION)' != x ] || { \
	  echo "$(_rls_ME): $@: no version given!" >&2; \
	  exit 1; \
	}
	$(_rls_make) version-git-tag-is-new-check
	@if test x"$@" = x"major"; then \
	  echo "$(_rls_make) version-ok-for-major-check"; \
	  $(_rls_make) version-ok-for-major-check || exit 1; \
	fi
	$(_rls_make) news-up-to-date-check
	$(_rls_make) strict-distcheck
	$(_rls_make) clean
	$(_rls_make) dist
	@echo ====
	@echo "$(_rls_ME): creating a git tag for $@ release \`$(VERSION)'"
	$(GIT) tag -a -m '$@ release $(VERSION)' 'v$(VERSION)'

# vim: ft=make ts=4 sw=4 noet
