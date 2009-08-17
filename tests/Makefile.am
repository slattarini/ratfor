## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# $Id$
# Makefile for the test/ratfor subdirectory.

TESTS = \
000sanity.test \
devnull-input.test \
empty-input.test \
integers-many-bases.test

EXTRA_DIST = $(TESTS)

# Explicitly set the $srcdir variable for the test cases, so that the code
# in defs.sh won't be forced to calculate it again and again.
TESTS_ENVIRONMENT = srcdir='$(abs_srcdir)'

# might be updated later (esp. in included files)
MOSTLYCLEANFILES = 

# Each test case depends on defs.sh and ratfor
check_SCRIPTS = ratfor defs.sh

top_ratfor = $(top_builddir)/src/ratfor$(EXEEXT)

# Ratfor binary to be used in testing.
ratfor$(EXEEXT): $(top_ratfor)
	rm -f $@ && $(LN_S) $(top_ratfor)

$(top_ratfor):
	cd $(abs_top_builddir)/src \
	  && $(MAKE) $(AM_MAKEFLAGS) ratfor$(EXEEXT)

# Be sure to use the absolute path of the test ratfor executable, since
# in our testsuite we merryli change the curent directory a lot.
TESTS_ENVIRONMENT += TESTED_RATFOR='$(abs_builddir)/ratfor$(EXEEXT)'

# To slighty improve the overall speed of our slow testsuite, force
# consistency checks in defs.sh to be skipped by default for test cases
# run from the Makefile (in which case the tests are mostly superfluous
# anyway).
TESTS_ENVIRONMENT += RAT4_TESTSUITE_BYPASS_SANITY_CHECKS=yes

# Remove the ratfor binary used in testing
clean-local-ratfor:
	rm -f ratfor$(EXEEXT)
.PHONY: clean-local-ratfor
clean-local: local-ratfor

# Remove any directory left behind the the test scripts (this is needed
# especially if the shell used in test des not properly support the `-e'
# flag and the trap of `EXIT' signal). The `find' command is needed in
# case the test directory or any of its subdirectories was made unwritable
# by the testsuite.
clean-local-testdirs:
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ";"
	rm -rf *.dir
.PHONY: clean-local-testdirs
clean-local: clean-local-testdirs

# Consistency checks on the list of tests.
include CheckTestsList.am

## vim: ft=automake noet sw=4 ts=4
