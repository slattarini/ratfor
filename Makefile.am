## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Top-level Makefile for the Ratfor Testsuite.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src-aux tests

DISTCHECK_CONFIGURE_FLAGS = 'RATFOR=$(RATFOR)'

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

## vim: ft=automake noet sw=4 ts=4
