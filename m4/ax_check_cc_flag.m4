#-*- Autoconf -*-
# Copied from SteLib at 2010-02-17 01:09:10 +0100.  DO NOT EDIT!
# serial 3 ax_check_cc_flag.m4

#
# Copyright (C) 2009, 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# _AX_CHECK_CC_FLAG(FLAG-TO-CHECK,[PROLOGUE],[BODY],
#                   [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# ------------------------------------------------------------
# For internal usage only.
AC_DEFUN([_AX_CHECK_CC_FLAG],
    [AC_LANG_PUSH([C])
    # Save original value of $CFLAGS.
    AS_VAR_COPY([ax_check_cc_flag_save_CFLAGS], [CFLAGS])
    # Temporarly append the flag to be checke to $CFLAGS.
    AS_VAR_APPEND([CFLAGS], [" $1"])
    # Check the flag in combination of the original $CFLAGS.
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([$2], [$3])], [$4], [$5])
    # Restore original value of $CFLAGS.
    AS_VAR_COPY([CFLAGS], [ax_check_cc_flag_save_CFLAGS])
    AS_UNSET([ax_check_cc_flag_save_CFLAGS])
    AC_LANG_POP()])

# AX_CACHE_CHECK_CC_FLAG(FLAG-TO-CHECK, [PROLOGUE], [BODY], 
#                        [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# ----------------------------------------------------------------
#  This macro checks if the C compiler supports the flag FLAG-TO-CHECK.
#  Execute ACTION-IF-SUCCESS if succesful, ACTION-IF-FAILURE otherwise.
#  PROLOGUE and BODY are optional, and should be used as in they are
#  in the AC_LANG_PROGRAM macro.
#  The check is cached, and the cache variable is named:
#   `ax_cv_cc_check_flag_FLAG'
#  where FLAG is obtained normalizing `FLAG-TO-CHECK' as through the
#  use of m4sh macro `AS_TR_SH' (which basically should just substite
#  non-alphanumeric characters in FLAG-TO-CHECK with underscores `_').
AC_DEFUN([AX_CACHE_CHECK_CC_FLAG],
    [AS_VAR_PUSHDEF([_$0_var], [ax_cv_check_cc_flag_$1])dnl
    AC_CACHE_CHECK(
        [whether the C compiler accepts the $1 flag],
        [_$0_var],
        [_AX_CHECK_CC_FLAG(
            [$1], [$2], [$3],
            [AS_VAR_SET([_$0_var], [yes])],
            [AS_VAR_SET([_$0_var], [no])])])
    AS_VAR_IF([_$0_var], [yes], [$4], [$5])
    AS_UNSET([_$0_var])
    AS_VAR_POPDEF([_$0_var])])

# vim: ft=m4 ts=4 sw=4 et
