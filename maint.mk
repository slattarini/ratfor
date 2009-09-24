# -*- Makefile -*-
# Contains maintainer-specific rules. Included by top-level GNUmakefile.
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

# Regenerate the source tree, and reconfigure the build tree.
.PHONY: reconfigure
reconfigure: autoreconf
	$(SHELL) $(srcdir)/configure

# By default, call autoreconf in an unconfigured tree before trying to
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

# do not assume $(MAKE) to be GNU make, since it is sometimes useful to
# ovverride it from the command line for testing purposes.
is_gnu_make := $(shell \
  $(MAKE) -f /dev/null --version nil 2>/dev/null | grep GNU \
    && echo yes || echo no)

ifeq ($(is_gnu_make),yes)
xmake = $(MAKE) --no-print-directory
else
xmake = $(MAKE)
endif

# Use this to make sure we don't run these programs when building
# from a virgin tgz file, below.
null_AM_MAKEFLAGS = \
  ACLOCAL=false \
  AUTOCONF=false \
  AUTOMAKE=false \
  AUTOHEADER=false \
  MAKEINFO=false

# Command line arguments for configure as called by `strict-distcheck'.
# Will be updated later.
strict_distcheck_configure_flags =

# Auxilary variables, needed to keep the code in target `strict-distcheck'
# understandable. Will be suitably updated later, in various places.
StrictDistcheckLoopBegin =
StrictDistcheckLoopEnd =

# C/C++ compilers to be used to by the reatfor testsuite, when doing a
# strict-distcheck.
strict_distcheck_c_compilers ?= cc c++
StrictDistcheckLoopBegin += \
  for cc in $(strict_distcheck_c_compilers); do
StrictDistcheckLoopEnd += done;
strict_distcheck_configure_flags += CC='$$cc'

# Fortran compilers to be used by the reatfor testsuite, when doing a
# strict-distcheck.
strict_distcheck_f77_compilers ?= fort77 gfortran
StrictDistcheckLoopBegin += \
  for f77 in NONE $(strict_distcheck_f77_compilers); do
StrictDistcheckLoopEnd += done;
strict_distcheck_configure_flags += F77='$$f77'

# Shells to be used to run the configure script and the tests scripts,
# when doing a strict-distcheck.  Since the rules in `make distcheck'
# run `configure' directly, we need to go through some hoops to force
# it to run with the shell we want.
strict_distcheck_shells ?= ksh bash
StrictDistcheckLoopBegin += \
  for sh_t in /bin/sh $(strict_distcheck_shells); do \
    case "$$sh_t" in \
      /*) sh=$$sh_t;; \
      */*) sh=`pwd`/$$sh_t1;; \
      *) sh=`which "$$sh_t" 2>/dev/null`;; \
    esac; \
    case "$$sh" in /*);; *) continue;; esac; \
    test -f "$$sh" && test -x "$$sh" || continue; \
    _RAT4_STRICT_DISTCHECK_CONFIG_SHELL="$$sh"; \
    export _RAT4_STRICT_DISTCHECK_CONFIG_SHELL; \
    :
StrictDistcheckLoopEnd += \
    _RAT4_STRICT_DISTCHECK_CONFIG_SHELL=''; \
    unset _RAT4_STRICT_DISTCHECK_CONFIG_SHELL || :; \
  done;
strict_distcheck_configure_flags += CONFIG_SHELL='$$sh'
# Since the rules in `make distcheck' run `configure' directly, we need
# to go through some hoops to force it to run with the shell we want.
.PHONY: config-shell-strict-distcheck-hook
distcheck-hook: config-shell-strict-distcheck-hook
config-shell-strict-distcheck-hook:
	@test -z "$${_RAT4_STRICT_DISTCHECK_CONFIG_SHELL-}" || { \
	  echo "$(ME): Correcting configure script for strict-distcheck" \
	  && tmp=configure.tmp \
	  && sh=$${_RAT4_STRICT_DISTCHECK_CONFIG_SHELL} \
	  && rm -f $$tmp \
	  && echo "#! $$sh" >>$$tmp \
	  && echo "# Correction code added by $(ME)" >>$$tmp \
	  && echo "CONFIG_SHELL='$$sh'" >>$$tmp \
	  && echo "export CONFIG_SHELL" >>$$tmp \
	  && sed '1s/^#!.*$$//' $(distdir)/configure >>$$tmp \
	  && chmod u+w $(distdir)/configure \
	  && cat $$tmp >$(distdir)/configure \
	  && chmod a-w $(distdir)/configure \
	  && echo '|----------------' \
	  && sed -e 's/^/| /' -e 40q $(distdir)/configure \
	  && echo '|----------------' \
	  && rm -f $$tmp; \
	}

.PHONY: strict-distcheck
strict-distcheck: all check distcheck
	set -e; \
	 xrun() { \
	   echo "+ $$*"; \
	   "$$@" || { \
	     echo "========================================================"; \
	     echo "$(ME): COMMAND FAILED: $$*"; \
	     echo "========================================================"; \
	     exit 1; \
	   }; \
	 }; \
	 echo " -*-*-*-"; \
	 echo "$(ME): running $@"; \
	 $(StrictDistcheckLoopBegin) \
	   echo " -*-*-*-"; \
	   xrun $(xmake) distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$(strict_distcheck_configure_flags)"; \
	   echo " -*-*-*- "; \
	   xrun $(xmake) -j8 distcheck \
	     AM_MAKEFLAGS='$(null_AM_MAKEFLAGS)' \
	     DISTCHECK_CONFIGURE_FLAGS="$(strict_distcheck_configure_flags)"; \
	 $(StrictDistcheckLoopEnd) \
	 echo " -*-*-*-"; \
	 exit $$e;

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
