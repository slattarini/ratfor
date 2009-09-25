# -*- Makefile -*-
# Contains maintainer-specific rules to create a beta or stable release.
# Included by top-level mait.mk.

ME := release.mk

# Path or name of git(1).
GIT ?= git

# quoted-version: $(VERSION) with some metacharacters quoted, to be used
# e.g. in grep or sed commands
quoted-version := $(subst .,\.,$(VERSION))

.PHONY: vc-nodiff-check
vc-nodiff-check: git-no-diff-check git-no-diff-cached-check

.PHONY: git-no-diff-check
git-no-diff-check:
	@(unset CDPATH; cd $(srcdir) && $(GIT) diff) >$@.tmp
	@test ! -s $@.tmp || { \
	  cat $@.tmp; \
	  echo "$(ME): some local modifications not added to Git index" >&2; \
	  exit 1; \
	};
	@rm -f $@.tmp;
CLEAN_FILES += git-no-diff-check.tmp

.PHONY: git-no-diff-cached-check
git-no-diff-cached-check:
	@(unset CDPATH; cd $(srcdir) && $(GIT) diff --cached) >$@.tmp
	@test ! -s $@.tmp || { \
	  cat $@.tmp; \
	  echo "$(ME): there are uncommitted modifications to Git index" >&2; \
	  exit 1; \
	};
	@rm -f $@.tmp;
CLEAN_FILES += git-no-diff-cached-check.tmp

.PHONY: version-for-major-check
version-for-major-check:
	  @echo x'$(VERSION)' | egrep '^x[0-9]+(\.[0-9]+)+$$' >/dev/null || { \
	    echo "$(ME): invalid version string \`$(VERSION)'" >&2; \
	    exit 1; \
	  }

.PHONY: news-up-to-date-check
vc-diff-check:
	grep "^New in $(quoted-version)$$" $(srcdir)/NEWS >/dev/null || { \
	   echo "$(ME): version \`$(VERSION)' not mentioned in NEWS" >&2; \
	   exit 1; \
	}

.PHONY: alpha beta major
ALL_RECURSIVE_TARGETS += alpha beta major
alpha beta major:
	$(xmake) GIT='$(GIT)' vc-nodiff-check
	@[ x'$(VERSION)' != x ] || { \
	  echo "$(ME): $@: no version given!" >&2; \
	  exit 1; \
	}
	@if $(GIT) tag | grep '^v$(quoted-version) *$$' >/dev/null; then \
	  echo "$(ME): $@: git tag \`v$(VERSION)' already exists" >&2; \
	  exit 1; \
	fi
	if test x"$@" = x"major"; then \
	  $(xmake) version-for-major-check || exit 1; \
	fi
	$(xmake) news-up-to-date-check
	$(xmake) strict-distcheck
	$(xmake) clean
	$(xmake) dist
	@echo ====
	@echo "$(ME): creating a git tag for $@ release \`$(VERSION)'"
	$(GIT) tag -a -m '$@ release $(VERSION)' 'v$(VERSION)'

# vim: ft=make ts=4 sw=4 noet
