# -*-Makefile-*-
# $Id$
# Loosely inspired to `maintainer-makefile' module.

# This Makefile fragment tries to be general-purpose enough to be
# used by many projects via the gnulib maintainer-makefile module.

ME := maint.mk

# Prevent programs like 'sort' from considering distinct strings to be equal.
# Doing it here saves us from having to set LC_ALL elsewhere in this file.
export LC_ALL = C

# Do not save the original name or timestamp in the .tar.gz file.
# Use --rsyncable if available.
gzip_rsyncable := \
  $(shell gzip --help 2>/dev/null | grep rsyncable >/dev/null && \
  	    echo x--rsyncable | sed 's/^x//')
GZIP_ENV = '--no-name --best $(gzip_rsyncable)'

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
vc-nodiff-check:
	@rm -f vc-diffs.tmp
	@(unset CDPATH; cd $(srcdir) && $(GIT) diff) >vc-diffs.tmp
	@test ! -s vc-diffs.tmp || { \
	  cat vc-diffs.tmp;	\
	  echo "$(ME): Some files are locally modified:" >&2; \
	  exit 1; \
	};
	@rm -f vc-diffs.tmp;

.PHONY: version-for-major-check
version-for-major-check: 
	  @echo x'$(VERSION)' | egrep '^x[0-9]+(\.[0-9]+)+$$' >/dev/null || { \
	    echo "$(ME): invalid version string \`$(VERSION)'" >&2; \
	    exit 1; \
	  }

.PHONY: news-up-to-date-check
vc-diff-check:
	@version=`echo '$(VERSION)' | sed 's/\./\\\\./'`; \
	 grep "^New in $$version$$" $(srcdir)/NEWS >/dev/null || { \
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
	 echo "  ===== "; \
	 echo "$(ME): running strict-distcheck"; \
	 for f77 in NONE $(strict_distcheck_f77_compilers); do \
	   configure_flags="$(strict_distcheck_configure_flags) F77='$$f77'"; \
	   echo "  ===== "; \
	   xrun $(xmake) distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$$configure_flags"; \
	   echo "  ===== "; \
	   xrun $(xmake) -j8 distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$$configure_flags"; \
	 done; \
	 echo "  ===== "; \
	 exit $$e;

.PHONY: alpha beta major
ALL_RECURSIVE_TARGETS += alpha beta major
alpha beta major:
	$(xmake) GIT='$(GIT)' vc-nodiff-check 
	[ x'$(VERSION)' != x ] || { \
	  echo "$(ME): $@: no version given." >&2; \
	  exit 1; \
	}
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
