# -*- Autoconf -*-
# Copied from SteLib at 2010-02-03 16:07:08 +0100.  DO NOT EDIT!
# serial 1 ax_cc_is_cxx_support.m4

#
# Autoconf macros which provide (minimal) help in compiling C code
# with a C++ compiler.
#
# Copyright (C) 2010 Lattarini Stefano <stefano.lattarini@gmail.com>
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_CACHE_CHECK_CC_IS_CXX([ACTION-IF-TRUE], [ACTION-IF-FALSE])
# ------------------------------------------------------------------
# If $CC is a C++ compiler, run ACTION-IF-TRUE, else run ACTION-IF-FALSE.
# Also, set the `$ax_cv_check_cc_is_cxx' accordingly.
AC_DEFUN([AX_CACHE_CHECK_CC_IS_CXX],
    [AC_CACHE_CHECK(
        [wheter the C compiler is a C++ compiler],
        [ax_cv_check_cc_is_cxx],
        [AC_COMPILE_IFELSE(
            [AC_LANG_PROGRAM([[
#ifdef __cplusplus
  choke me
#endif]])],
            [ax_cv_check_cc_is_cxx=no],
            [ax_cv_check_cc_is_cxx=yes])])
    AS_IF([test x"$ax_cv_check_cc_is_cxx" = x"yes"], [$1], [$2])])

# AX_AH_C_DECLS_MACROS
# --------------------
# Add some boileplate code to config.h to make it easier to
# support the use of C++ compilers to compile you C code.
AC_DEFUN([AX_AH_C_DECLS_MACROS],
    [AH_TOP([/* Support for C++ compilers */
#ifdef __cplusplus
#  define C_DECL extern "C"
#  define BEGIN_C_DECLS extern "C" {
#  define END_C_DECLS }
#else
#  define C_DECL
#  define BEGIN_C_DECLS
#  define END_C_DECLS
#endif])])

# vim: ft=m4 ts=4 sw=4 et
