## -*- Makefile.am -*-
## Process this file with automake to produce Makefile.in
##
# Makefile for the tests in the Ratfor Testsuite.


# -------------- #
#  MISCELLANOUS  #
# -------------- #

# Might be updated later (also in included files).  Do not remove these
# definitiond.
MOSTLYCLEANFILES = 
EXTRA_DIST =


# --------------- #
#  LIST OF TESTS  #
# --------------- #

# Generate the list of tests.
$(srcdir)/TestsList.am: $(build_aux)/gen-tests-list.sh Makefile.am
	$(AM_V_GEN): \
	  && rm -f $@ $@-t \
	  && $(SHELL) $(build_aux)/gen-tests-list.sh $(srcdir) >$@-t \
	  && chmod a-w $@-t \
	  && mv $@-t $@
MAINTAINERCLEANFILES = $(srcdir)/TestsList.am $(srcdir)/TestsList.am-t
EXTRA_DIST += $(build_aux)/gen-tests-list.sh

# Include the list of tests (definition of TESTS_ALL).
include $(srcdir)/TestsList.am

EXTRA_DIST += $(ALL_TESTS)

TEST_EXTENSIONS = .test

XFAIL_TESTS = $(RAT4_TESTSUITE_XFAIL_TESTS)

# run all tests by default
TESTS = $(ALL_TESTS)


# ---------------------------------------- #
#  REQUIREMENTS AND DEPENDENCIES OF TESTS  #
# ---------------------------------------- #

EXTRA_DIST += integers-many-bases.data
integers-many-bases.log: integers-many-bases.data
integers-many-bases_f77.log: integers-many-bases.data

EXTRA_DIST += break-next-fail.data
break-fail.test: break-next-fail.data
next-fail.test: break-next-fail.data

# Each test case depends on to-be-included scripts `defs.sh'.
check_SCRIPTS = defs.sh

# --------------------------------- #
#  TESTSUITE ENVIRONMENT AND SETUP  #
# --------------------------------- #

# Since we are using the new the new `parallel-tests' functionality
# of automake, use $(TEST_LOG_COMPILER) instead of $(TESTS_ENVIRONMENT),
# as it is the new recommended way to go, $(TESTS_ENVIRONMENT) being
# now tought as user-reserved.
TEST_LOG_COMPILER = env # because we need to pass env vars to test scripts
AM_TEST_LOG_FLAGS = # will be updated later

# To improve the overall speed of our slow testsuite, let it know that
# it is already being run with the proper $SHELL (as found by configure),
# so that the test scripts won't re-execute themselves with $SHELL
AM_TEST_LOG_FLAGS += RAT4_TESTSUITE_RUNNING_WITH_CONFIG_SHELL='yes'

# To run the test scripts with the "better" shell available, while leaving
# the possibility to the user to override the choice.
TEST_SHELL = $(SHELL) # user-overridable
AM_TEST_LOG_FLAGS += $(TEST_SHELL)


# ------------------- #
#  TESTSUITE CLEANUP  #
# ------------------- #

# Remove any directory left behind the the test scripts (this is needed
# especially if the shell used in test des not properly support the `-e'
# flag and the trap of `EXIT' signal). The `find' command is needed in
# case the test directory or any of its subdirectories was made unwritable
# by the testsuite.
clean-local-testdirs:
	: we are not interested in errors of find, just in those of rm
	find *.dir -type d '!' -perm -200 -exec chmod u+w {} ';' || :
	rm -rf *.dir
.PHONY: clean-local-testdirs
clean-local: clean-local-testdirs

## vim: ft=automake noet sw=4 ts=4
