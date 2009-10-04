# configure.ac file for Ratfor Testsuite.
# Process this file with autoconf to produce a configure script.

m4_pattern_forbid([^_AX_])
m4_pattern_forbid([^AX_])
m4_pattern_forbid([^_RAT4_])
m4_pattern_forbid([^RAT4_])

# This apparently weird quote is needed to avoid that vim picks up
# and intepretes the following line as a vim modeline for this file.
m4_define([__vim_modeline__], [@%:@ v][im: ft=sh ts=8 sw=8 noet])

#
# Autoconf's initializations.
#

AC_PREREQ([2.64])
AC_INIT([Ratfor Testsuite], [0.9a], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([tests/ifelse-ambiguity_f77.test])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])

#
# Maintainer-specific initializations and defaults.
#

AX_INCLUDE_MAINT_CONFIG_SITE

#
# Automake's initializations.
#

AM_INIT_AUTOMAKE([1.11 foreign -Wall -Werror -Wno-override color-tests
                       parallel-tests no-installinfo no-texinfo.tex])
AM_SILENT_RULES([yes]) # make `--enable-silent-rules' the default

#
# Option `--enable-maintainer-make-rules' tells that maintaner-specific
# rules should be imported in the GNUmakefile.
#

AX_ARG_ENABLE(
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

#
# Check for a ratfor program to be used by the test scripts.
#

AC_PATH_PROGS([RATFOR], [ratfor ratfor77], [:])
if test x"$RATFOR" = x":"; then
    AC_MSG_ERROR([No ratfor program found])
fi
AC_ARG_VAR([RATFOR], [Ratfor preprocessor to be used by the testsuite])

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
RAT4_PROG_F77

# Look for a diff command that can produce better human-readable output.
AX_PROG_DIFF_U

#
# Do output.
#

# Make GNUmakefile available also in VPATH builds.
AC_CONFIG_LINKS([GNUmakefile:GNUmakefile])

AC_CONFIG_FILES([Makefile bin-aux/Makefile tests/Makefile])

# Be sure to make the generated files readonly, to avoid that absent-minded
# developers modify them instead of the files they are generated from.
# It has already happened too many times.
AC_CONFIG_FILES([tests/defs.sh:tests/defs.in],
                [chmod a-w tests/defs.sh])
AC_CONFIG_FILES([bin-aux/timer:bin-aux/timer.sh],
                [chmod a-w,a+x bin-aux/timer])

AC_OUTPUT

#
# Done.
#

AX_REPORT_IF_BETA
AS_EXIT(0)

__vim_modeline__
dnl*
dnl* vim: ft=config ts=4 sw=4 et
