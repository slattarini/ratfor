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

: # None needed presently

#
# Checks for headers.
#

AC_HEADER_STDC
if test x"$ac_cv_header_stdc" != x"yes"; then
    AC_MSG_ERROR([some system standard C headers seem broken])
fi

for header in errno unistd signal fcntl; do
    AC_CHECK_HEADER([$header.h], [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done
AS_UNSET([header])

AC_HEADER_SYS_WAIT
if test x"$ac_cv_header_sys_wait_h" != x"yes"; then
    AC_MSG_ERROR([header `<sys/wait.h>' not found or broken])
fi

AC_HEADER_STDBOOL

#
# Checks for typedefs, structures, and compiler characteristics.
#

AC_C_CONST
AC_C_INLINE

#
# Checks for library functions.
#

for func in getopt strerror alarm signal sigaction vfprintf; do
    AC_CHECK_FUNC([$func], [:],
        [AC_MSG_ERROR([No proper `$func' subroutine found])])
done
AS_UNSET([func])

#
#  Enable compiler warnings.
#

# Enable as many compiler warnings as possible (this is tailered to gcc,
# but should not cause failures of problems for other compilers).
# This code *must* be set after any check on cc, c headers, c functions
# or c libraries, as stricter handling of warnings can interfere with
# autoconf checks.
for flag in -Wall -Wextra -Wshadow -Werror -pedantic; do
    RAT4_CACHE_CHECK_CC_FLAG([$flag],[],[],[CFLAGS="$CFLAGS $flag"])
done
AS_UNSET([flag])

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
