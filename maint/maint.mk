# -*- Makefile -*-
# Copied from SteLib at 2009-11-13 16:50:05 +0100.  DO NOT EDIT!
# Contains maintainer-specific rules. Included by top-level GNUmakefile.

# Do not save the original name or timestamp in the .tar.gz file.
# Use --rsyncable if available.
gzip_rsyncable := \
  $(shell gzip --help 2>/dev/null | grep rsyncable >/dev/null && \
      echo x--rsyncable | sed 's/^x//')
GZIP_ENV = '--no-name --best $(gzip_rsyncable)'

#
# The rest of this makefile is to be considered only if the user requested
# the import of maintainer-specific make rules.
#
ifeq ($(ENABLE_MAINTAINER_MAKE_RULES),yes)

#==========================================================================

# Directory containing most maintainer-specific stuff.
maintdir = $(srcdir)/maint
abs_maintdir = $(abs_srcdir)/maint

# When running autoreconf, honor the `AUTORECONF' environment variable,
# regardless of the fact that make was called with or without the `-e'
# switch. We use `$(_autoreconf)' since this is the same variable used
# in top-level GNUmakefile.
_autoreconf := $(shell echo "$${AUTORECONF:-autoreconf -vi}")

# Prevent programs like 'sort' from considering distinct strings to be
# equal. Doing it here saves us from having to set LC_ALL elsewhere in
# this makefile, or in makefiles included by it.
export LC_ALL = C

#--------------------------------------------------------------------------

#
# Helper & utility macros.
#

define _maint_normalize-boolean-sh-code
  x=`echo '$(1)' | sed -e 's/^[ 	]*//' -e 's/[ 	]*$$//'`;
  case "$$x" in
    [yY]|[yY]es|YES|[tT]rue|1) echo yes;;
    *) echo no;;
  esac;
endef

# don't leave leading spaces in the body of the macro
define normalize-boolean
$(shell $(call _maint_normalize-boolean-sh-code,$(1)))
endef

#--------------------------------------------------------------------------

#
# Read local definitions and overrides, if any.
#
-include $(maintdir)/local-cfg.mk

#
# Read early project-specific definitions and overrides, if any.
#
-include $(maintdir)/prj-pre-cfg.mk

#
# Include maintainer rules, subdivided in sub-makefiles for clarity
# and modularity.
#
include $(maintdir)/developer.mk
include $(maintdir)/strict-distcheck.mk
include $(maintdir)/release.mk

#
# Read late project-specific definitions, overrides and extensions, if any.
#
-include $(maintdir)/prj-post-cfg.mk

#==========================================================================

endif # $(ENABLE_MAINTAINER_MAKE_RULES) == yes

# vim: ft=make ts=4 sw=4 noet
