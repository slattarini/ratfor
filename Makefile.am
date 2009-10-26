## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Top-level Makefile for the Ratfor Testsuite.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = tests

# List of extra files to be put in the distributed tarball.
EXTRA_DIST = \
  HACKING \
  GNUmakefile \
  maint/maint.mk \
  maint/strict-distcheck.mk \
  maint/release.mk \
  maint/developer.mk \
  maint/prj-post-cfg.mk

# List of files which must not be put in the distributed tarball.
no_dist_files = \
  maint/config.site \
  maint/local-cfg.mk \
  maint/personal/*

# Check that we do not erronously include in the tarball any file which is
# not meant for distribution.
dist-hook:
	@list='$(no_dist_files)'; for f in $$list; do \
	  if test -r $(distdir)/$$f; then \
	    bad_dist_files="$$bad_dist_files $$f"; \
	  fi; \
	done; \
	if test -n "$$bad_dist_files"; then \
	  echo "$@: ERROR: the following files have erroneously been put" \
	       "in the distributed tarball:" >&2; \
	  for f in $$bad_dist_files; do \
	    echo "  - $$f" >&2; \
	  done; \
	  exit 1; \
	fi; \
	exit 0

if ENABLE_MAINTAINER_MAKE_RULES
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

DISTCHECK_CONFIGURE_FLAGS = \
	'RATFOR=$(RATFOR)' \
	'RAT4_TESTSUITE_XFAIL_TESTS=$(RAT4_TESTSUITE_XFAIL_TESTS)'

# This is defined here so that it can be extended in maint/*.mk, if needed.
distcheck-hook:

# If config.status or configure are run by Zsh 3.x, they can erroneously
# leave behind some temporary files, and this causes "make distcheck" to
# fail.  So we clean up those temporary files by hand if ${SHELL} is
# determined to be Zsh 3.x
mostlyclean-local:
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
