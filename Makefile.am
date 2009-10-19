## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Public Domain Ratfor preprocessor, written in C.
# Makefile for the top-level directory.

ACLOCAL_AMFLAGS = -I m4 --install

EXTRA_DIST = \
  xfail-tests.txt \
  HACKING \
  GNUmakefile \
  maint/maint.mk maint/strict-distcheck.mk maint/release.mk \
  maint/prj-post-cfg.mk maint/local-cfg-mk.samples

if ENABLE_MAINTAINER_MAKE_RULES
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = yes
else
  # variable used by maint.mk
  ENABLE_MAINTAINER_MAKE_RULES = no
endif

SUBDIRS = src doc blackbox-tests

dist_doc_DATA = README BUGS NEWS

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
	 xrun() { echo " $$*"; "$$@" || { e=1; return 1; } }; \
	 for subdir in 'blackbox-tests' '.'; do \
	   ocwd=`pwd`; \
	   xrun cd "$$subdir" || continue; \
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
	   xrun cd "$$ocwd" || { \
	     echo "$@: FATAL: cannot chdir back to \`$$ocwd'" >&2;  \
		 exit 1; \
	   }; \
	 done; \
	 exit $$e

## vim: ft=automake noet sw=4 ts=4
