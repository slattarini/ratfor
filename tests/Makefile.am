## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
## Makefile for the test/ratfor subdirectory.

TESTS =	\
000sanity.test \
devnull-input.test

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = srcdir='$(srcdir)'

# might be updated later
MOSTLYCLEANFILES = 

# Each test case depends on defs.sh and ratfor
check_SCRIPTS = ratfor defs.sh

top_ratfor = $(top_builddir)/src/ratfor$(EXEEXT)

ratfor$(EXEEXT): $(top_ratfor)
	rm -f $@ && $(LN_S) $(top_ratfor)

$(top_builddir)/src/ratfor$(EXEEXT):
	cd $(abs_top_builddir)/src \
	  && $(MAKE) $(AM_MAKEFLAGS) ratfor$(EXEEXT)

TESTS_ENVIRONMENT += TESTED_RATFOR='$(abs_builddir)/ratfor$(EXEEXT)'

# Check that the list of tests as given by $(TEST) is equal to the
# list of all *.test scripts
tmk = tests-in-makefile-list.tmp
tfs = tests-in-makefile-list.tmp
tdiffs = diff-in-tests-lists.tmp 
check-list-of-tests:
	@echo "****--------------------------------------------"
	@echo "** Checking consistency of list of tests..."
	@for t in $(TESTS); do \
	    echo "$$t"; \
	 done > $(tmk)
	@(cd $(srcdir) && for t in *.tests; do \
	    echo "$$t"; \
	 done) > $(tfs)
	@if $(DIFF_U) $(tmk) $(tfs) > $(tdiffs); then \
	    : OK, no diffs; \
	 else \
	    echo "*** ERROR ***" >&2; \
	    echo " List of tests in Makefile an on filesystem differ" >&2; \
	    echo " + $(DIFF_U) in-makefile on-filesystem" >&2; \
	    cat $(tdiffs) >&2; \
		echo "****--------------------------------------------"; \
	    exit 1; \
	 fi;
	@echo "** OK"
	@echo "****--------------------------------------------"
MOSTLYCLEANFILES += $(tmk) $(tfs) $(tdiffs)
.PHONY: check-list-of-tests

check-local: check-list-of-tests

clean-local-testdirs:
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ";"
	rm -rf *.dir
.PHONY: clean-local-testdirs

clean-local-ratfor:
	rm -f ratfor$(EXEEXT)
.PHONY: clean-local-ratfor

clean-local: clean-local-testdirs clean-local-ratfor

## vim: ft=automake noet sw=4 ts=4
