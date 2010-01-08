# -*- Makefile -*-
# Copied from SteLib at 2010-01-08 23:12:51 +0100.  DO NOT EDIT!
#
#  Contains maintainer-specific rules to run target `strict-distcheck',
#  and "on-steroids" version of distcheck.  Included by top-level maint.mk.
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

# Public variables/macros follow the naming scheme `$(strict_distcheck_*)'
# or $(StrictDistcheck*).
# Internal macros, shell variables and make targets follow the naming
# scheme `_SD_*'.

_SD_me := strict-distcheck.mk

# Use this to make sure we don't run these programs when building from a
# virgin tgz file in `strict-distcheck'.
strict_distcheck_null_am_makeflags ?= \
  ACLOCAL=false \
  AUTOCONF=false \
  AUTOMAKE=false \
  AUTOHEADER=false \
  PERL=false \
  MAKEINFO=false

# Command line arguments for configure as called by `strict-distcheck'.
# Will be updated later.
strict_distcheck_configure_flags =

# Auxilary variables, needed to keep the code in target `strict-distcheck'
# They will be suitably extended in the rest of this file.
# Note thatt projects using strict-distcheck can extend or override these,
# if they need to.
StrictDistcheckLoopBegin =
StrictDistcheckLoopEnd =

# Shells to be used to run the configure script and the tests scripts,
# when doing a strict-distcheck.  Since the rules in `make distcheck'
# run `configure' directly, we need to go through some hoops to force
# it to run with the shell we want.
strict_distcheck_shells ?= /bin/sh ksh
StrictDistcheckLoopBegin += \
  for _SD_sh in $(strict_distcheck_shells); do \
    case "$${_SD_sh}" in \
      /*) sh=$${_SD_sh};; \
      */*) sh=`pwd`/$${_SD_sh};; \
      *) sh=`which "$${_SD_sh}"` || \
	       fatal "shell \`$${_SD_sh}' not found in PATH";; \
    esac; \
    case "$$sh" in \
	  /*);; \
	  *) fatal "shell \`$$sh': not an absolute path";; \
	esac; \
    test -f "$$sh" || fatal "shell \`$$sh': doesn't exist"; \
    test -x "$$sh" || fatal "shell \`$$sh': not executable"; \
    _SD_STRICT_DISTCHECK_CONFIG_SHELL="$$sh"; \
    export _SD_STRICT_DISTCHECK_CONFIG_SHELL; \
    set +x;
StrictDistcheckLoopEnd += \
    _SD_STRICT_DISTCHECK_CONFIG_SHELL=''; \
    unset _SD_STRICT_DISTCHECK_CONFIG_SHELL || :; \
  done;
strict_distcheck_configure_flags += CONFIG_SHELL='$$sh'
# Since the rules in `make distcheck' run `configure' directly, we need
# to go through some hoops to force it to run with the shell we want.
.PHONY: _SD_distcheck-config-shell-distcheck-hook
distcheck-hook: _SD_config-shell-strict-distcheck-hook
_SD_config-shell-strict-distcheck-hook:
	@test -z "$${_SD_STRICT_DISTCHECK_CONFIG_SHELL-}" || { \
	  echo "$(_SD_me): Correcting configure script for strict-distcheck" \
	    && tmp=configure.tmp \
	    && sh=$${_SD_STRICT_DISTCHECK_CONFIG_SHELL} \
	    && rm -f $$tmp \
	    && echo "#! $$sh" >>$$tmp \
	    && echo "# Correction code added by $(_SD_me)" >>$$tmp \
	    && echo "CONFIG_SHELL='$$sh'" >>$$tmp \
	    && echo "export CONFIG_SHELL" >>$$tmp \
	    && sed '1s/^#!.*$$//' $(distdir)/configure >>$$tmp \
	    && chmod u+w $(distdir)/configure \
	    && cat $$tmp >$(distdir)/configure \
	    && chmod a-w $(distdir)/configure \
	    && echo '|----------------' \
	    && sed -e 's/^/| /' -e 11q $(distdir)/configure \
	    && echo '|----------------' \
	    && rm -f $$tmp; \
	}

# internal macro
define _SD_DisplayFlags
  eval ' \
    for x in : '"$(call $(1))"'; do \
      test x"$$x" = x":" && continue; \
      echo "$(2)    $$x"; \
    done'
endef

# internal macro
define _SD_StrictDistcheckDisplay
 { \
  echo "$(1)==========================================================="; \
  echo "$(1)"; \
  echo "$(1)  "$(2); \
  echo "$(1)"; \
  echo "$(1)  AM_MAKEFLAGS:"; \
  $(call _SD_DisplayFlags,strict_distcheck_null_am_makeflags,$(1)); \
  echo "$(1)"; \
  echo "$(1)  DISTCHECK_CONFIGURE_FLAGS:"; \
  $(call _SD_DisplayFlags,strict_distcheck_configure_flags,$(1)); \
  echo "$(1)"; \
  echo "$(1)==========================================================="; \
 }
endef

.PHONY: strict-distcheck
strict-distcheck: all check distcheck
	set -e; \
	fatal() { \
	  echo "$(_SD_me): $$*" >&2; \
	  exit 1; \
	}; \
	$(StrictDistcheckLoopBegin) \
	  echo; echo ' -*-*-*-'; echo; \
	  $(call _SD_StrictDistcheckDisplay,@@,'$(_SD_me): running $@'); \
	  e=0; \
	  $(MAKE) -j2 distcheck \
	      AM_MAKEFLAGS='$(strict_distcheck_null_am_makeflags)' \
	      DISTCHECK_CONFIGURE_FLAGS="$(strict_distcheck_configure_flags)" \
	    || e=1; \
	  if test $$e -eq 0; then \
	    $(call _SD_StrictDistcheckDisplay,..,'$(_SD_me): $@ OK'); \
	  else \
	    $(call _SD_StrictDistcheckDisplay,!!,'$(_SD_me): $@ FAILED'); \
	    exit 1; \
	  fi; \
	$(StrictDistcheckLoopEnd) \
	echo; \
	echo ' -*-*-*-'; \
	echo;

# vim: ft=make ts=4 sw=4 noet
