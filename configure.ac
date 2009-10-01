# configure.ac file for Public Domain Ratfor
# Process this file with autoconf to produce a configure script.

m4_pattern_forbid([^_?RAT4_])
# This apparently weird quote is needed to avoid that vim picks up
# and intepretes the following line as a vim modeline for this file.
m4_define([__vim_modeline__], [@%:@ v][im: ft=sh ts=8 sw=8 noet])

#
# Autoconf's initializations.
#

AC_PREREQ([2.64])
AC_INIT([Public Domain Ratfor], [1.0k], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4-common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
# the testsuite is kept in a separate subpackage.
AC_CONFIG_SUBDIRS([blackbox-tests])

# Since `RATFOR' is declared in `blackbox-tests/configure.ac' as a
# precious variable, this will pass the proper value of $RATFOR to
# the recusrively invoked script `blackbox-tests/configure'.
# NOTE: when the configure script is running, the current working
# directory (returned by `pwd') is the build directory.
RATFOR=`pwd`/src/ratfor; export RATFOR
AC_MSG_NOTICE([exported RATFOR='$RATFOR'])

#
# Maintainer-specific initializations and defaults.
#

RAT4_INCLUDE_MAINT_CONFIG_SITE

#
# Automake's initializations.
#

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror no-installinfo no-texinfo.tex])
AM_SILENT_RULES([yes]) # make `--enable-silent-rules' the default

#
# Checks for C compiler and support.
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

for header in errno unistd; do
    AC_CHECK_HEADER([$header.h], [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done
AS_UNSET([header])

#
# Check for booleans' support.
#

AC_HEADER_STDBOOL

#
# Checks for typedefs, structures, and compiler characteristics.
#

AC_C_CONST
AC_C_INLINE

#
# Checks for library functions.
#

for func in getopt strerror vfprintf; do
    AC_CHECK_FUNC([$func], [:],
        [AC_MSG_ERROR([No proper `$func' subroutine found])])
done
AS_UNSET([func])

#
#  Enable compiler warnings.
#

# Option `--enable-werror-cflag' tells whether or not the `-Werror' flag
# should be added (when supported) to $CFLAGS.
RAT4_ARG_ENABLE(
    [werror-cflag],
    [enable the `-Werror' C compiler flag, if supported],dnl '`# font-lock
    [rat4_maybe_werror=-Werror],
    [rat4_maybe_werror=''])

# Option `--enable-maintainer-make-rules' tells that maintaner-specific
# rules should be imported in the GNUmakefile.
RAT4_ARG_ENABLE(
    [maintainer-make-rules],
    [Import maintaner-specific rules in the GNUmakefile. Use it only if
      you have GNU make, and you know what you are doing. Also, note that
      this option has *NOTHING* to do with the infamous automake's
      "maintainer-mode".],dnl '# font-lock
    [rat4_enable_maintainer_make_rules=yes],
    [rat4_enable_maintainer_make_rules=no])

m4_pattern_allow([ENABLE_MAINTAINER_MAKE_RULES])
AM_CONDITIONAL([ENABLE_MAINTAINER_MAKE_RULES],
               [test x"$rat4_enable_maintainer_make_rules" = x"yes"])

# Enable as many compiler warnings as possible (this is tailered to gcc,
# but should not cause failures of problems for other compilers).
# This code *must* be set after any check on cc, c headers, c functions
# or c libraries, as stricter handling of warnings can interfere with
# autoconf checks.
for flag in -Wall -Wextra -Wshadow -pedantic $rat4_maybe_werror; do
    RAT4_CACHE_CHECK_CC_FLAG([$flag],[],[],[CFLAGS="$CFLAGS $flag"])
done
AS_UNSET([flag])

#
# Hack to support C++ compilers.
#

AH_TOP([/* Support for C++ compilers */
#ifdef __cplusplus
#  define C_DECL extern "C"
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
# else
#  define C_DECL
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif])

#
# Do output.
#

AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile])
# Make GNUmakefile available also in VPATH builds.
AC_CONFIG_LINKS([GNUmakefile:GNUmakefile])

AC_OUTPUT

#
# Done.
#

AS_EXIT(0)

__vim_modeline__
dnl*
dnl* vim: ft=config ts=4 sw=4 et
