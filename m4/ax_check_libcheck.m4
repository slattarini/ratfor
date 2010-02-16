# -*- Autoconf -*-
# Copied from SteLib at 2010-02-16 11:55:49 +0100.  DO NOT EDIT!
# serial 1 ax_check_libcheck.m4

#
# Copyright (C) 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
# This file is part of C Ratfor.
#
# Define macro `AX_CHECK_LIBCHECK' which looks for the `check' library
# (unit testing library for C) and related header files.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#


#######################
##  Internal macros  ##
#######################

AC_DEFUN([_AX_ARG_WITH_LIBCHECK],
    [AC_ARG_WITH(
        [libcheck],
        [AS_HELP_STRING([--with-libcheck],
                        [use `check' library for unit testing])],
        [:], [with_libcheck=check])])

AC_DEFUN([_AX_HEADER_CHECK_H],
    [# check for <check.h> header
    AC_CHECK_HEADER([check.h],
                     [ax_have_check_h=yes],
                     [ax_have_check_h=no])])

AC_DEFUN([_AX_CHECK_LIBCHECK],
    [m4_pushdef([$0_testfunc], [suite_create])
    # check for libcheck (no tongue-in-cheek)
    AC_SEARCH_LIBS($0_testfunc, [check libcheck])
    AS_CASE([$ac_cv_search_]$0_testfunc,
        [none*required],
            [ax_have_libcheck=yes
             ax_libcheck_ldflags=''],
        [no],
            [ax_have_libcheck=no
             ax_libcheck_ldflags=''],
        dnl* default
            [ax_have_libcheck=yes
             ax_libcheck_ldflags=$ac_cv_search_]$0_testfunc)
    m4_popdef([$0_testfunc])])

AC_DEFUN([_AX_LIBCHECK_NOTFOUND_ERROR],
    [AC_MSG_ERROR([m4_do([library `check' not found or not usable, ],
                         [but the user explicitly required it])])])

AC_DEFUN([_AX_LIBCHECK_NOTFOUND_WARN],
    [AC_MSG_WARN([m4_do([library `check' not found or not usable; ],
                        [unit tests will be skipped])])])

#######################
##  Public macro(s)  ##
#######################

#
# AX_CHECK_LIBCHECK
# -------------------
# Look for the `check' library (unit testing library for C) and related
# header files.  If the library is accessible, define (with AC_SUBST)
# `LIBCHECK_LDFLAGS' to the compiler flag(s) necessary to link to it, and
# define (with AC_DEFINE) the preprocessor symbol `HAVE_LIBCHECK' to `1'.
# Otherwise,  define (with AC_SUBST) `LIBCHECK_LDFLAGS' to the empty string
# and define (with AC_DEFINE) the preprocessor symbol `HAVE_LIBCHECK' to
# `0'.  Note that if `HAVE_LIBCHECK' is set to `1', the user code can
# assume to be able to `#include <check.h>' without any problem.
# The search for this library will honour the `--with-libcheck' configure
# option.
AC_DEFUN_ONCE([AX_CHECK_LIBCHECK],
    [_AX_ARG_WITH_LIBCHECK dnl* add `--with-libcheck' option to configure
    ax_have_libcheck=no; ax_libcheck_ldflags='';
    if test x"$with_libcheck" != x"no"; then
        dnl* check for <check.h> header, anf it it's found check for the
        dnl* `check' library itself
        _AX_HEADER_CHECK_H
        AS_IF([test x"$ax_have_check_h" = x"yes"], [_AX_CHECK_LIBCHECK])
    fi
    if test x"$ax_have_libcheck" != x"yes"; then
        AS_CASE([$with_libcheck],
                [yes], [_AX_LIBCHECK_NOTFOUND_ERROR],
                 [no], [:],
                  [*], [_AX_LIBCHECK_NOTFOUND_WARN])
    fi
    AC_SUBST([LIBCHECK_LDFLAGS], [$ax_libcheck_ldflags])
    AS_IF([test x"$ax_have_libcheck" = x"yes"],
          [AC_DEFINE([HAVE_LIBCHECK], [1],
                     [Define to 1 if the `check' library is available])])])

# vim: ft=m4 ts=4 sw=4 et
