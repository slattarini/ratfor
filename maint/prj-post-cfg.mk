# -*- Makefile -*-
# Contains project-specific definitions, settings and overrides for
# maintainer-specific targets

#
# Testcases to be skipped, even when doing a strict-distcheck (presently
# these are exactly the same the use passed to the configure script).
#

strict_distcheck_configure_flags += \
  RAT4_TESTSUITE_XFAIL_TESTS='$(RAT4_TESTSUITE_XFAIL_TESTS)'

#
# Fortran compilers to be used by the ratfor testsuite, when doing a
# strict-distcheck.
#

# can be overridden by local-cfg.mk
strict_distcheck_f77_compilers ?= NONE f77

StrictDistcheckLoopBegin += \
  for f77 in $(strict_distcheck_f77_compilers); do
StrictDistcheckLoopEnd += done;

strict_distcheck_configure_flags += F77='$$f77'

# vim: ft=make ts=4 sw=4 noet
