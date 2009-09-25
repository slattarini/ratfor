# -*- Makefile -*-
# Maintainer-oriented shortcuts to regenerate, reconfigure and rebuild.
# Included by top-level mait.mk.

ME := remake.mk

# When running autoreconf, honor the `AUTORECONF' environment variable,
# regardless of the fact that make was called with or without the `-e'
# switch. We use `$(_autoreconf)' since this is the same variable used
# in top-level GNUmakefile.
_autoreconf := $(shell echo "$${AUTORECONF:-autoreconf}")

define autoreconf-rules
  cd $(srcdir) && rm -rf autom4te.cache && $(_autoreconf) -vi
endef

define reconfigure-rules
  $(SHELL) ./config.status --recheck
  $(SHELL) ./config.status
endef

# Call autoreconf the source tree, to rebuild autotools-generated files.
.PHONY: autoreconf
autoreconf:
	$(autoreconf-rules)

# Regenerate the source tree, and reccheck the build tree with the same
# conditions of the old configuration (if any).
.PHONY: reconfigure
reconfigure: autoreconf
	$(reconfigure-rules)

# Regenerate the source tree, and reconfigure the build tree, and rebuild
# everything.
.PHONY: remake
remake:
	$(MAKE) $(AM_MAKEFLAGS) clean
	$(autoreconf-rules)
	$(reconfigure-rules)
	$(MAKE) $(AM_MAKEFLAGS) all

# vim: ft=make ts=4 sw=4 noet
