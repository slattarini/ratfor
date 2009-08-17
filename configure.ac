# configure.ac file for Public Domain Ratfor
# Process this file with autoconf to produce a configure script.

# Autoconf's and Automake's initializations

AC_PREREQ([2.64])
AC_INIT([ratfor], [1.0c], [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 -Wall -Werror color-tests parallel-tests foreign])
AM_SILENT_RULES([yes]) # make --enable-silent-rules the default.

# Check for C support

AC_PROG_CC_STDC
AM_PROG_CC_C_O

# Test for programs

AC_PROG_INSTALL
AC_PROG_MKDIR_P

# Check for headers

for header in unistd stdlib string; do
    AC_CHECK_HEADER(
        [$header.h],
        [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done

# Check for functions

AC_CHECK_FUNC([getopt], [:],
              [AC_MSG_ERROR([No proper `getopt' subroutine found])])

# Test for things needed by the test suite.

AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_SED
AC_PROG_LN_S

# Get a list of available fortran compilers, to be used in testing
dnl ---
AC_ARG_VAR([RAT4_F77_LIST],
[List of Fortran 77 compilers to be used in testing (should all be given
as absolute paths!). If empty, configure will scan the system looking for
a working Fortran 77 compiler.])
dnl ---
AC_MSG_CHECKING([a list of Fortran 77 compilers to be used in testing])
if test -n "$RAT4_F77_LIST"; then
    : # do nothing: user choiche override our checks
    AC_MSG_RESULT([got a user-provided list!])
else
    AC_MSG_RESULT([no default list available])
    AC_MSG_NOTICE([Now will scan the system to find a Fortran 77 compiler])
    AC_PROG_F77
    RAT4_F77_LIST=$F77
fi
if test -n "$RAT4_F77_LIST"; then
    AC_MSG_NOTICE(
        [Fortran 77 compilers to be used in testing: $RAT4_F77_LIST])
else
    AC_MSG_WARN([No Fortran 77 compiler found to be used in testing])
    AC_MSG_WARN([Some parts of the testsuite will be skipped])
fi

# TODO: check that all compilers in $RAT4_F77_LIST work, bailout on error

# Look for a diff command that can produce better human-readable output
AC_CACHE_CHECK(
    dnl# IMPORTANT NOTE: the cache variable *must* have the format
    dnl# `ac_cv_*' to work; using e.g. `rat4_cv_path_UDIFF' seems
    dnl# tempting, but does not work.
    [for a diff command producing human-readable output],
    [ac_cv_path_UDIFF],
    [AC_PATH_PROGS_FEATURE_CHECK(
        [UDIFF],
        [diff gdiff],
        [echo x >confdf1 && echo x >confdf2 \
           && $ac_path_UDIFF -u confdf1 confdf2 >/dev/null 2>&1 \
           && ac_path_UDIFF_found=: \
           && ac_cv_path_UDIFF=$ac_path_UDIFF \
           && DIFF_U="$ac_path_UDIFF -u"
         rm -f confdf1 confdf2],
        [AC_MSG_RESULT([None found.])
         DIFF_U=diff])])
AC_SUBST([DIFF_U])

# Check if the $SHELL supports `trap "..." EXIT' and `set -e' that play
# well togheter.
AC_CACHE_CHECK(
    [whether \$SHELL=$SHELL has working 'set -e' with exit trap],
    [rat4_cv_sh_errexit_works],
[if $SHELL -c "set -e; trap 'exit \$?' 0; (exit 77); exit 77"; test $? = 77
then
    rat4_cv_sh_errexit_works=yes
else
    rat4_cv_sh_errexit_works=no
fi])
if test x"$rat4_cv_sh_errexit_works" = x"no"; then
  AC_MSG_WARN([`${MAKE-make} check' will leave leftover directories tests/*.dir])
  AC_MSG_WARN([you can clean them up manually using `${MAKE-make} clean' or])
  AC_MSG_WARN([`cd tests && ${MAKE-make} clean-local-testdirs'])
fi
AC_SUBST([sh_errexit_works], [$rat4_cv_sh_errexit_works])

# Do output.

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
    tests/defs.sh:tests/defs.in
])
AC_OUTPUT
