## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile for the test/ratfor subdirectory.

TEST_EXTENSIONS = .test

# --------------- #
#  LIST OF TESTS  #
# --------------- #

XFAIL_TESTS = # updated later

# Currently, messages about syntax errors are *not* dispalyed if the error
# occur in an included file. Sigh. To be fixed ASAP.
XFAIL_TESTS += \
  synerr-include-lineno.test \
  include-fail.test \
  include-synerr.test
  
# Currently, ratfor output strings with the same quoting they are input
# with; this should be changed, and only single-quoted strings (e.g. 'foo')
# should be outputted.
XFAIL_TESTS += no-double-quotes.test

# Currently, ratfor builtin `define' is case-insensible w.r.t. symbol names
XFAIL_TESTS += define-case-sensibility.test

# We need this auxiliary variable to keep the list of *all* test scripts
# so that the user can override the $(TESTS) variable without clashing
# with consistency checks done by  the `check-list-of-tests' target defined
# in `CheckTestList.am'.
ALL_TESTS = \
  000sanity.test \
  define-balanced-paren.test \
  define-balanced-paren_f77.test \
  define-basic.test \
  define-basic_f77.test \
  define-case-sensibility.test \
  define-fail.test \
  define-multiline.test \
  define-multiline_f77.test \
  define-nested.test \
  define-nested_f77.test \
  define-redefine.test \
  define-semicolon.test \
  define-semicolon_f77.test \
  define-whitespaces.test \
  devnull-input.test \
  empty-input.test \
  include-abspath.test \
  include-basic.test \
  include-defined.test \
  include-fail.test \
  include-nested-deep.test \
  include-nested.test \
  include-stress.test \
  include-synerr.test \
  integers-many-bases.test \
  integers-many-bases_f77.test \
  keepcomment.test \
  keepcomment_f77.test \
  long-infile.test \
  long-infile_f77.test \
  no-curly-brackets.test \
  no-double-quotes.test \
  no-semicolons.test \
  no-tabs.test \
  rel-transl.test \
  startlabel-default.test \
  startlabel-user-choice.test \
  stress1_f77.test \
  stress2_f77.test \
  synerr-basic.test \
  synerr-include-lineno.test \
  synerr-lineno.test \
  synerr-nonfatal.test \
  synerr-nonfatal_f77.test

EXTRA_DIST = $(TESTS)

# run all tests by default
TESTS = $(ALL_TESTS)

# ----------------------- #
#  REQUIREMENTS OF TESTS  #
# ----------------------- #

EXTRA_DIST += integers-many-bases.data
integers-many-bases.log: integers-many-bases.data
integers-many-bases_f77.log: integers-many-bases.data

# --------------------------------- #
#  TESTSUITE ENVIRONMENT AND SETUP  #
# --------------------------------- #

# Since we are using the new the new `parallel-tests' functionality
# of automake, use $(TEST_LOG_COMPILER) instead of $(TESTS_ENVIRONMENT),
# as it is the new recommended way to go, $(TESTS_ENVIRONMENT) being
# now tought as user-reserved.
TEST_LOG_COMPILER = env # because we need to pass env vars to test scripts
AM_TEST_LOG_FLAGS = # will be updated later

# Explicitly set the $srcdir variable for the test cases.
#  (Q.1) Why do we need this definition?
#  (A.1) So that the code in defs.sh won't be forced to calculate it
#        again and again.
#  (Q.2) Why we can't rely on automake feature of exporting
#        `srcdir=$(srcdir)' in the environment of every test case?
#  (A.2) We prefer to export `srcdir=$(abs_srcdir)' instead, since
#        otherwise, whenever $(srcdir) is not an absolute path (which
#        is the usual case), defs.sh will be forced to make it an
#        absolute path (with e.g. srcdir=`pwd`/$srcdir), thereby
#        slowing the execution of the testsuite.
AM_TEST_LOG_FLAGS += srcdir='$(abs_srcdir)'

# might be updated later (esp. in included files)
MOSTLYCLEANFILES = 

# Each test case depends on defs.sh and ratfor
check_SCRIPTS = ratfor$(EXEEXT) defs.sh

# Real ratfor binary.
top_ratfor = $(top_builddir)/src/ratfor$(EXEEXT)

# Ratfor binary to be used in testing. It depends on the real ratfor
# binary (at the moment, it's just a link to it).
ratfor$(EXEEXT): $(top_ratfor)
	rm -f $@ && $(LN_S) $(top_ratfor) $@

# Tell how to rebuild the real ratfor binary -- aha, the woes of recursive
# make...
$(top_ratfor): _force
	cd $(abs_top_builddir)/src && $(MAKE) $(AM_MAKEFLAGS) ratfor$(EXEEXT)
_force:
.PHONY: _force

# Be sure to use the absolute path of the test ratfor executable, since
# in our testsuite we merryli change the curent directory a lot.
AM_TEST_LOG_FLAGS += TESTED_RATFOR='$(abs_builddir)/ratfor$(EXEEXT)'

# To improve the overall speed of our slow testsuite, let it know that
# it is already being run with the proper $SHELL (as found by configure),
# so that the test scripts won't re-execute themselves with $SHELL
AM_TEST_LOG_FLAGS += RAT4_TESTSUITE_RUNNING_WITH_CONFIG_SHELL='yes'

# To slighty improve the overall speed of our slow testsuite, force
# consistency checks in defs.sh to be skipped by default for test cases
# run from the Makefile (in which case the tests are mostly superfluous
# anyway).
AM_TEST_LOG_FLAGS += RAT4_TESTSUITE_BYPASS_SANITY_CHECKS='yes'

# To run the test scripts with the "better" shell available, while leaving
# the possibility to the user to override the choice.
TEST_SHELL = $(SHELL) # user-overridable
AM_TEST_LOG_FLAGS += $(TEST_SHELL)

# ------------------- #
#  TESTSUITE CLEANUP  #
# ------------------- #

# Remove the ratfor binary used in testing
clean-local-ratfor:
	rm -f ratfor$(EXEEXT)
.PHONY: clean-local-ratfor
clean-local: clean-local-ratfor

# Remove any directory left behind the the test scripts (this is needed
# especially if the shell used in test des not properly support the `-e'
# flag and the trap of `EXIT' signal). The `find' command is needed in
# case the test directory or any of its subdirectories was made unwritable
# by the testsuite.
clean-local-testdirs:
	: we are not interested in errors of find, just in those of rm
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ";" || :
	rm -rf *.dir
.PHONY: clean-local-testdirs
clean-local: clean-local-testdirs

# ----------------------------------------- #
#  CONSISTENCY CHECKS ON THE LIST OF TESTS  #
# ----------------------------------------- #

include CheckTestsList.am

## vim: ft=automake noet sw=4 ts=4
