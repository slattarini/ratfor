# configure.ac file for Public Domain Ratfor
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.64])
AC_INIT([ratfor], [1.0b], [stefano.lattarini@gmail.com])
AC_CONFIG_SRCDIR([src/rat4.c])
AC_CONFIG_HEADERS([config.h])

AC_CONFIG_AUX_DIR([build-aux])

AM_INIT_AUTOMAKE([1.11 color-tests parallel-tests foreign])
AM_SILENT_RULES([yes]) # make --enable-silent-rules the default.

AC_PROG_CC_STDC
AM_PROG_CC_C_O

AC_PROG_INSTALL
AC_PROG_MKDIR_P

for header in unistd stdlib string; do
    AC_CHECK_HEADER(
        [$header.h],
        [:],
        [AC_MSG_ERROR([header `<$header.h>' not found or broken])])
done

AC_CHECK_FUNC([getopt], [:],
              [AC_MSG_ERROR([No proper `getopt' subroutine found])])

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
])
AC_OUTPUT
