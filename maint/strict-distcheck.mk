# -*- Makefile -*-
# Contains maintainer-specific rules to run target `strict-distcheck',
# and "on-steroids" version of distcheck. Included by top-level mait.mk.

ME := strict-distcheck.mk

# Use this to make sure we don't run these programs when building from a
# virgin tgz file in `strict-distcheck'.
strict_distcheck_null_am_makeflags ?= \
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

# C/C++ compilers to be used to by the ratfor testsuite, when doing a
# strict-distcheck.
strict_distcheck_c_compilers ?= cc c++
StrictDistcheckLoopBegin += \
  for cc in $(strict_distcheck_c_compilers); do
StrictDistcheckLoopEnd += done;
strict_distcheck_configure_flags += CC='$$cc'
strict_distcheck_configure_flags += --enable-werror-cflag

# Fortran compilers to be used by the ratfor testsuite, when doing a
# strict-distcheck.
strict_distcheck_f77_compilers ?= f77 gfortran
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
      *'[ 	]'*) \
        sh=`(set $$sh_t && shift && echo $$1)`; \
        sh_args=`(set $$sh_t && shift && shift && echo $$*)`;; \
      *) \
        sh=$$sh_t; sh_args='';; \
    esac; \
    case "$$sh" in \
      /*) ;; \
      */*) sh=`pwd`/$$sh;; \
      *) sh=`which "$$sh" 2>/dev/null`;; \
    esac; \
    case "$$sh" in /*);; *) continue;; esac; \
    test -f "$$sh" && test -x "$$sh" || continue; \
    sh="$$sh $$sh_args"; \
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
	   xrun $(xmake) -j2 distcheck \
	     AM_MAKEFLAGS='$(strict_distcheck_null_am_makeflags)' \
	     DISTCHECK_CONFIGURE_FLAGS="$(strict_distcheck_configure_flags)"; \
	 $(StrictDistcheckLoopEnd) \
	 echo " -*-*-*-"; \
	 exit $$e;

# vim: ft=make ts=4 sw=4 noet
