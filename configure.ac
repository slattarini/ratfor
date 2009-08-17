# configure.ac file for Public Domain Ratfor
# Process this file with autoconf to produce a configure script.
# $Id$

m4_pattern_forbid([^_?RAT4_])
m4_define([__vim_modeline__], [@%:@ v][im: ft=sh ts=8 sw=8 noet])

#
# Autoconf's and Automake's initializations
#

AC_PREREQ([2.64])
AC_INIT([ratfor], [1.0d], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_MACRO_DIR([m4])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 -Wall -Werror color-tests parallel-tests foreign])
AM_SILENT_RULES([yes]) # make --enable-silent-rules the default.

#
# Check for C support
#

AC_PROG_CC_STDC
AM_PROG_CC_C_O

#
# Check for programs
#

AC_PROG_INSTALL
AC_PROG_MKDIR_P

#
# Check for headers
#

for header in unistd stdlib string; do
    AC_CHECK_HEADER(
        [$header.h],
        [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done

#
# Check for functions
#

AC_CHECK_FUNC([getopt], [:],
              [AC_MSG_ERROR([No proper `getopt' subroutine found])])

#
# Check for things needed by the test suite.
#

# Common utilities.
AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_SED
AC_PROG_AWK
AC_PROG_LN_S

# Get a list of available fortran compilers, to be used in testing.
RAT4_PROG_F77_FOR_TESTSUITE

# Look for a diff command that can produce better human-readable output.
RAT4_PROG_DIFF_U

# Check if the $SHELL supports `trap "..." EXIT' and `set -e' that play
# well togheter.
RAT4_CHECK_SH_ERREXIT_WORKS
if test x"$rat4_cv_sh_errexit_works" = x"no"; then
  AC_MSG_WARN([`${MAKE-make} check' will leave leftover directories tests/*.dir])
  AC_MSG_WARN([you can clean them up manually using `${MAKE-make} clean' or])
  AC_MSG_WARN([`cd tests && ${MAKE-make} clean-local-testdirs'])
fi

#
# Do output.
#

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
    tests/defs.sh:tests/defs.in
])
AC_OUTPUT

#
# Done.
#

AS_EXIT(0)

__vim_modeline__
dnl*
dnl* vim: ft=config ts=4 sw=4 et
