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

# Test for things needed by the test suite.

AC_PROG_GREP
AC_PROG_EGREP
AC_PROG_FGREP
AC_PROG_SED
AC_PROG_LN_S

# TODO: proper check & AC_SUBST for fortran77 compiler

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
  AC_MSG_WARN([`cd tests && ${MAKE-make} clean-local-check'])
fi
AC_SUBST([sh_errexit_works], [$rat4_cv_sh_errexit_works])

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

# Do output.

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
    tests/defs.sh:tests/defs.in
])
AC_OUTPUT
