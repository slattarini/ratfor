# -*- Makefile -*-
# Contains maintainer-specific rules. Included by top-level GNUmakefile.

# Directory containing most maintainer-specific stuff
maintdir = $(srcdir)/maint
abs_maintdir = $(abs_srcdir)/maint

# When running autoreconf, honor the `AUTORECONF' environment variable,
# regardless of the fact that make was called with or without the `-e'
# switch. We use `$(_autoreconf)' since this is the same variable used
# in top-level GNUmakefile.
_autoreconf := $(shell echo "$${AUTORECONF:-autoreconf}")

#
# The rest of this makefile is to be considered only if the user requested
# the import of maintainer-specific make rules.
#
ifeq ($(RAT4_ENABLE_MAINTAINER_MAKE_RULES),yes)

#--------------------------------------------------------------------------

# Prevent programs like 'sort' from considering distinct strings to be
# equal. Doing it here saves us from having to set LC_ALL elsewhere in
# this makefile, or in makefiles included by it.
export LC_ALL = C

# Do not save the original name or timestamp in the .tar.gz file.
# Use --rsyncable if available.
gzip_rsyncable := \
  $(shell gzip --help 2>/dev/null | grep rsyncable >/dev/null && \
      echo x--rsyncable | sed 's/^x//')
GZIP_ENV = '--no-name --best $(gzip_rsyncable)'

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

#
# Read local definition and overrides, if any.
#
-include $(maintdir)/local-cfg.mk

#
# Include maintainer rules, subdivided in sub-makefiles for clarity
# and modularity. We reset `$(ME)' since it can be overriden in
# sub-makefiles.
#
ME := maint.mk
include $(maintdir)/strict-distcheck.mk
ME := maint.mk
include $(maintdir)/release.mk

#--------------------------------------------------------------------------

endif # $(RAT4_ENABLE_MAINTAINER_MAKE_RULES) == yes

# vim: ft=make ts=4 sw=4 noet
