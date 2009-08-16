## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
## Makefile for the test/ratfor subdirectory.

TESTS =	\
000sanity.test 

EXTRA_DIST = $(TESTS)

TESTS_ENVIRONMENT = srcdir='$(srcdir)'

# Each test case depends on defs.sh and ratfor
check_SCRIPTS = ratfor defs.sh

ratfor$(EXEEXT): $(top_builddir)/src/ratfor$(EXEEXT)
	rm -f $@ && $(LN_S) $(top_builddir)/src/ratfor $@

$(top_builddir)/src/ratfor$(EXEEXT):
	cd $(abs_top_builddir)/src \
	  && $(MAKE) $(AM_MAKEFLAGS) ratfor$(EXEEXT)

clean-local: clean-local-check clean-local-ratfor

clean-local-check:
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ";"
	rm -rf *.dir
.PHONY: clean-local-check

clean-local-ratfor:
	rm -f ratfor
.PHONY: clean-local-check

## vim: ft=automake noet sw=4 ts=4
