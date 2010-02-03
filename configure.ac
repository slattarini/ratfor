# -*- Autoconf -*-
# configure.ac file for C Ratfor
# Process this file with autoconf to produce a configure script.
#
# Copyright (C) 2009, 2010 Stefano Lattarini
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#

m4_pattern_forbid([^_RAT4_])
m4_pattern_forbid([^RAT4_])
m4_pattern_forbid([^_AX_])
m4_pattern_forbid([^AX_])

#
# Autoconf's initializations.
#

AC_PREREQ([2.64])
AC_INIT([C Ratfor],
        m4_esyscmd([build-aux/git-version-gen .tarball-version]),
        [stefano.lattarini@gmail.com])

AC_CONFIG_SRCDIR([src/rat4-common.h])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_SUBDIRS([tests/blackbox])

#
# Define and pass proper values of precious variables to the recursively
# invoked script `blackbox-tests/configure'.
#

# Ratfor program to be used in the testsuite. *Must* be an absolute path.
# NOTE: when the configure script is running, the current working
# directory (returned by `pwd') is the build directory.
RATFOR=`pwd`/src/c-ratfor
export RATFOR
AC_MSG_NOTICE([exported RATFOR='$RATFOR'])

test -f "$srcdir/xfail-tests.list" && test -r "$srcdir/xfail-tests.list" \
 || AC_MSG_ERROR([file `xfail-tests.list' not found in srcdir `$srcdir'])

m4_pattern_allow([RAT4_TESTSUITE_XFAIL_TESTS])

AC_MSG_NOTICE([reading list of test cases expected to fail])
RAT4_TESTSUITE_XFAIL_TESTS=`
  while read line; do
    AS_CASE(["$line"], ['@%:@'*|''], [], [AS_ECHO_N(" $line")])
  done <"$srcdir/xfail-tests.list"` \
 || AC_MSG_ERROR([unexpected error reading file `$srcdir/xfail-tests.list'])
export RAT4_TESTSUITE_XFAIL_TESTS

#
# Maintainer-specific initializations and defaults.
#

AX_INCLUDE_MAINT_CONFIG_SITE

#
# Automake's initializations.
#

AM_INIT_AUTOMAKE([1.11.1 foreign -Wall -Werror no-installinfo
                         no-texinfo.tex])
AM_SILENT_RULES([no]) dnl. make `silent-rules' disabled by default

#
# Checks for C compiler and support.
#

AC_PROG_CC_STDC
AM_PROG_CC_C_O

# See if $CC is a C++ compiler.
AX_CACHE_CHECK_CC_IS_CXX
# Help to support the use of C++ compilers as C compilers.
AX_AH_C_DECLS_MACROS

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

for func in getopt strerror vfprintf strdup; do
    AC_CHECK_FUNC([$func], [:],
        [AC_MSG_ERROR([No proper `$func' subroutine found])])
done
AS_UNSET([func])

#
# Enable maintainer-specific make rules, on user request.
#

# Option `--enable-maintainer-make-rules' tells that maintaner-specific
# rules should be imported in the GNUmakefile.
AX_ARG_ENABLE_MAINTAINER_MAKE_RULES

#
# Compiler warnings and strictness.
#

# Enable as many compiler warnings as possible.
AX_CC_ENABLE_ALL_COMPILER_WARNINGS

# Process configure option `--enable-werror-cflag', telling whether or
# not the `-Werror' flag should be added (when supported) to $CFLAGS.
AX_ARG_ENABLE_WERROR_CFLAG

#
# Do output.
#

AC_CONFIG_FILES([
    Makefile
    doc/Makefile
    src/Makefile
    tests/Makefile
    tests/graybox/Makefile
])

# Make GNUmakefile available also in VPATH builds.
AC_CONFIG_LINKS([GNUmakefile:GNUmakefile])

AC_OUTPUT

#
# Done.
#

AX_REPORT_IF_BETA
AS_EXIT(0)

dnl* vim: ft=config ts=4 sw=4 et
