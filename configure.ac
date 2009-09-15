# configure.ac file for Public Domain Ratfor
# Process this file with autoconf to produce a configure script.

m4_pattern_forbid([^_?RAT4_])
# This apparently weird quote is needed to avoid that vim picks up
# and intepretes the following line as a vim modeline for this file.
m4_define([__vim_modeline__], [@%:@ v][im: ft=sh ts=8 sw=8 noet])

#
# Autoconf's and Automake's initializations.
#

AC_PREREQ([2.64])
AC_INIT([ratfor], [1.0i], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4-common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror color-tests parallel-tests
                       no-installinfo no-texinfo.tex])
AM_SILENT_RULES([yes]) # make `--enable-silent-rules' the default

#
# Checks for C support.
#

AC_PROG_CC_STDC
AM_PROG_CC_C_O

#
# Checks for programs.
#

AC_PROG_INSTALL
AC_PROG_MKDIR_P

#
# Checks for headers.
#

for header in errno unistd stdlib string; do
    AC_CHECK_HEADER(
        [$header.h],
        [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done

AC_HEADER_STDBOOL

#
# Checks for typedefs, structures, and compiler characteristics.
#

AC_C_CONST
AC_C_INLINE

#
# Checks for library functions.
#

for func in getopt strerror; do
    AC_CHECK_FUNC(
        [$func],
        [:],
        [AC_MSG_ERROR([No proper `$func' subroutine found])])
done

# Sigh! Checks for vfprintf don't work with GCC if `-Werror' is active.
RAT4_CHECK_FUNC_NO_WERROR(
    [vfprintf],
    [:],
    [AC_MSG_ERROR([No proper `vfprintf' subroutine found])])

#
# Checks for things needed by the test suite.
#

# Common utilities.
AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_SED
AC_PROG_AWK
AC_PROG_LN_S

# Get an available fortran compiler, to be used in testing.
RAT4_PROG_F77_FOR_TESTSUITE

# Look for a diff command that can produce better human-readable output.
RAT4_PROG_DIFF_U

#
# Do output.
#

# Make GNUmakefile available also in VPATH builds.
AC_CONFIG_LINKS([GNUmakefile:GNUmakefile])

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
])
# Be sure to make the generated file `tests/defs.sh' readonly, to avoid
# that absent-minded developers modify it instead of `tests/defs.in'.
# It already happened too many times.
AC_CONFIG_FILES([tests/defs.sh:tests/defs.in], [chmod a-w tests/defs.sh])

AC_OUTPUT

#
# Done.
#

AS_EXIT(0)

__vim_modeline__
dnl*
dnl* vim: ft=config ts=4 sw=4 et
