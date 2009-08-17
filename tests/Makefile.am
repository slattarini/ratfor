## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
## Makefile for the test/ratfor subdirectory.

TESTS =	\
000sanity.test \
devnull-input.test \
empty-input.test

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = srcdir='$(srcdir)'

# might be updated later
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

# Check that the list of tests as given by $(TESTS) is equal to the
# list of all *.test scripts
tmk = tests-in-makefile-list.tmp
tfs = tests-on-filesystem-list.tmp
tdiffs = diff-in-tests-lists.tmp 
check-list-of-tests:
	@set -e; \
	 export LC_ALL=C; \
	 echo "****--------------------------------------------"; \
	 echo "** Checking consistency of list of tests..."; \
	 for t in $(TESTS); do \
	    echo "$$t"; \
	 done > $(tmk); \
	 (cd $(srcdir) && for t in *.test; do \
	    echo "$$t"; \
	 done) > $(tfs); \
	 if $(DIFF_U) $(tmk) $(tfs) > $(tdiffs); then \
		echo "** OK"; : No diffs; \
		e=0; \
	 else \
	    echo "*** ERROR ***" >&2; \
	    echo " List of tests in Makefile an on filesystem differ" >&2; \
	    echo " + $(DIFF_U) in-makefile on-filesystem" >&2; \
	    cat $(tdiffs) >&2; \
	    e=1; \
	 fi; \
	 echo "****--------------------------------------------"; \
	 rm -f $(tmk) $(tfs) $(tdiffs); \
	 exit $$e;
MOSTLYCLEANFILES += $(tmk) $(tfs) $(tdiffs)
.PHONY: check-list-of-tests

check-local: check-list-of-tests

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

# Remove the ratfor binary used in testing
clean-local-ratfor:
	rm -f ratfor$(EXEEXT)
.PHONY: clean-local-ratfor
clean-local: clean-local-ratfor

## vim: ft=automake noet sw=4 ts=4
