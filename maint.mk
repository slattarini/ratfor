# -*- Makefile -*-
# Loosely inspired to `maintainer-makefile' module.

ME := maint.mk

# Prevent programs like 'sort' from considering distinct strings to be equal.
# Doing it here saves us from having to set LC_ALL elsewhere in this file.
export LC_ALL = C

# When running autoreconf, honor the `AUTORECONF' environment variable,
# regardless of the fact that make was called with or without the `-e'
# switch.
_autoreconf := $(shell echo "$${AUTORECONF:-autoreconf}")

# Call autoreconf the source tree, to rebuild autotools-generated files.
.PHONY: autoreconf
autoreconf:
	cd $(srcdir) && rm -rf autom4te.cache && $(_autoreconf) -vi

# Regenrate the source tree, and reconfigure the build tree.
.PHONY: reconfigure
reconfigure: autoreconf
	$(SHELL) $(srcdir)/configure

# By default, call autoreconf in an uncofigured tree before trying to
# make the requsted targets.
ifeq ($(.DEFAULT_GOAL),abort-due-to-no-makefile)
ifeq ($(MAKECMDGOALS),)
.DEFAULT_GOAL := _remake
.PHONY: _remake
_remake: reconfigure
	$(MAKE) $(AM_MAKEFLAGS) all
else
.DEFAULT_GOAL :=
$(MAKECMDGOALS): _reconfigure_and_redo
.PHONY: _reconfigure_and_redo
_reconfigure_and_redo: reconfigure
	$(MAKE) $(MAKECMDGOALS)
endif
endif

# Do not save the original name or timestamp in the .tar.gz file.
# Use --rsyncable if available.
gzip_rsyncable := \
  $(shell gzip --help 2>/dev/null | grep rsyncable >/dev/null && \
  	    echo x--rsyncable | sed 's/^x//')
GZIP_ENV = '--no-name --best $(gzip_rsyncable)'

# quoted-version: $(VERSION) with some metacharacters quoted, to be used
# e.g. in grep or sed commands
quoted-version = $(subst .,\.,$(VERSION))

GIT = git

# we assume $(MAKE) to be GNU make, of course, so this is legitimate
xmake = $(MAKE) --no-print-directory

# Use this to make sure we don't run these programs when building
# from a virgin tgz file, below.
null_AM_MAKEFLAGS = \
  ACLOCAL=false \
  AUTOCONF=false \
  AUTOMAKE=false \
  AUTOHEADER=false \
  MAKEINFO=false

# Compilers to be used by the reatfor testsuite, when doing a
# strict-distcheck.
strict_distcheck_f77_compilers ?= fort77 gfortran

# Command line arguments for configure as called by `strict-distcheck'.
strict_distcheck_configure_flags = \
	CFLAGS='$(CFLAGS) -Wall -Wshadow -pedantic' \
	FFLAGS='$(FFLAGS) -Wall'
#XXX: use this once the rat4 source code will be fixed
#	CFLAGS='$(CFLAGS) -Wall -Werror -Wshadow -pedantic' \
#	FFLAGS='$(FFLAGS) -Wall -Werror'

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
	  cat $@.tmp;	\
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

.PHONY: strict-distcheck
strict-distcheck:
	@set -e; \
	 xrun() { \
	   echo "+ $$*"; \
	   "$$@" || { echo "$(ME): COMMAND FAILED: $$*"; exit 1; }; \
	 }; \
	 echo " -*-*-*-"; \
	 echo "$(ME): running strict-distcheck"; \
	 for f77 in NONE $(strict_distcheck_f77_compilers); do \
	   configure_flags="$(strict_distcheck_configure_flags) F77='$$f77'"; \
	   echo " -*-*-*-"; \
	   xrun $(xmake) distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$$configure_flags"; \
	   echo "  -*-*-*- "; \
	   xrun $(xmake) -j8 distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$$configure_flags"; \
	 done; \
	 echo " -*-*-*-"; \
	 exit $$e;

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
