# Written by Stefano Lattarini <stefano.lattarini@gmail.com>
#
# C Ratfor: a ratfor preprocessor written in C.

ACLOCAL_AMFLAGS = -I m4 --install

SUBDIRS = src doc tests

dist_doc_DATA = README BUGS

# List of extra files to be put in the distributed tarball.
EXTRA_DIST = \
  HACKING \
  GNUmakefile

# Maintainer-specific checks.
.PHONY: maintainer-check
maintainer-check: maintainer-check-list-of-tests

# List of directories where to recurse into for testing and related
# maintainer-checks.
TEST_SUBDIRS = tests/graybox tests/blackbox

# Check that the list of tests given in the Makefile is equal to the
# list of all test scripts in the Automake testsuite.
.PHONY: maintainer-check-list-of-tests
maintainer-check-list-of-tests:
	@target='$@'; fail=''; \
	 CDPATH="$${ZSH_VERSION+.}$(PATH_SEPARATOR)"; \
	 for subdir in $(TEST_SUBDIRS); do \
	   echo "Making $$target in $$subdir"; \
	   (cd $$subdir && $(MAKE) $(AM_MAKEFLAGS) $$target) || fail=yes; \
	 done; \
	 test -z "$$fail"

## vim: ft=automake noet sw=4 ts=4
