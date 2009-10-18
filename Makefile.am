## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Top-level Makefile for the Ratfor Testsuite.

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = \
  GNUmakefile \
  maint.mk maint/strict-distcheck.mk maint/release.mk \
  HACKING

if ENABLE_MAINTAINER_MAKE_RULES
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

SUBDIRS = bin-aux tests

DISTCHECK_CONFIGURE_FLAGS = \
	'RATFOR=$(RATFOR)' \
	'RAT4_TESTSUITE_XFAIL_TESTS=$(RAT4_TESTSUITE_XFAIL_TESTS)'

# We don't support install targets.
RAT4_INSTALL_TARGETS = \
  install install-data install-exec \
  install-dvi install-html install-info install-ps install-pdf \
  uninstall
$(RAT4_INSTALL_TARGETS):
	@(echo ==========================================================; \
	  echo '$(PACKAGE_NAME) dot not support "install-like" targets.';  \
	  echo "The target \`$@' is therefore a no-op.";                   \
	  echo ==========================================================) \
	 | sed 's/^/WARNING: /' >&2


# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# If config.status or configure are run by Zsh 3.x, they can erroneously
# leave behind some temporary files, and this causes "make distcheck" to
# fail.  So we clean up those temporary files by hand if ${SHELL} is
# determined to be Zsh 3.x
distclean-local:
	@case `$(SHELL) -c 'echo "$$ZSH_VERSION"'` in \
	   3.*) ;; \
	     *) exit 0;; \
	 esac; \
	 e=0; \
	 xrun() { echo " $$*"; "$$@" || e=1; }; \
	 xrun rm -f conftest*; \
	 xrun rm -f confdefs.h; \
	 : temporary directories left by config.status are tricky to find; \
	 for d in conf*; do \
	    if test -f "$$d/subs.awk" \
	         && $(FGREP) "PACKAGE_NAME" "$$d/subs.awk" >/dev/null \
	         && $(FGREP) '$(PACKAGE_NAME)' "$$d/subs.awk" >/dev/null; \
	    then \
	       xrun rm -f "$$d"/*; \
	       xrun rmdir "$$d"; \
	    fi; \
	 done; \
	 exit $$e

## vim: ft=automake noet sw=4 ts=4
