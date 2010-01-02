# -*- Makefile -*-
# Contains project-specific definitions, settings and overrides for
# maintainer-specific targets

#
# C compilers to be used to build ratfor, when doing a strict-distcheck.
#

# can be overridden by local-cfg.mk
strict_distcheck_c_compilers ?= cc c++

StrictDistcheckLoopBegin += \
  for cc in $(strict_distcheck_c_compilers); do
StrictDistcheckLoopEnd += done;
strict_distcheck_configure_flags += CC='$$cc'

#
# Make C compile-time warnings fatal also when checkin dist tarball
# with `strict-distcheck'
#
strict_distcheck_configure_flags += --enable-werror-cflag

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
