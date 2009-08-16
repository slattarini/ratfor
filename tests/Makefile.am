## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
## Makefile for the test/ratfor subdirectory.

TESTS =	\
000sanity.test \
devnull-input.test \
empty-input.test

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = srcdir='$(srcdir)'

# Each test case depends on defs.sh and ratfor
check_SCRIPTS = ratfor defs.sh

top_ratfor = $(top_builddir)/src/ratfor$(EXEEXT)

ratfor$(EXEEXT): $(top_ratfor)
	rm -f $@ && $(LN_S) $(top_ratfor)

$(top_builddir)/src/ratfor$(EXEEXT):
	cd $(abs_top_builddir)/src \
	  && $(MAKE) $(AM_MAKEFLAGS) ratfor$(EXEEXT)

TESTS_ENVIRONMENT += TESTED_RATFOR='$(abs_builddir)/ratfor$(EXEEXT)'

clean-local-testdirs:
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ";"
	rm -rf *.dir
.PHONY: clean-local-testdirs

clean-local-ratfor:
	rm -f ratfor$(EXEEXT)
.PHONY: clean-local-ratfor

clean-local: clean-local-testdirs clean-local-ratfor

## vim: ft=automake noet sw=4 ts=4
