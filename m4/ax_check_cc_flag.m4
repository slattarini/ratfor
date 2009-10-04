# -*- Autoconf -*-
# Copied from SteLib at 2009-10-04 20:49:22 +0200.  DO NOT EDIT!
# serial 1 ax_check_cc_flag.m4


# _AX4_CHECK_CC_FLAG(FLAG-TO-CHECK,[PROLOGUE],[BODY],
#                    [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# ------------------------------------------------------------
# For internal usage only.
AC_DEFUN([_AX_CHECK_CC_FLAG],
    [AC_LANG_PUSH([C])
    ax_check_cc_flag_save_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $1"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([$2],[$3])], [$4], [$5])
    CFLAGS="$ax_check_cc_flag_save_CFLAGS"
    AS_UNSET([ax_check_cc_flag_save_CFLAGS])
    AC_LANG_POP()])

# AX_CACHE_CHECK_CC_FLAG(FLAG-TO-CHECK, [PROLOGUE], [BODY], 
#                        [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# ----------------------------------------------------------------
#  This macro check if the C compiler supports the flag FLAG-TO-CHECK.
#  Execute ACTION-IF-SUCCESS if succesfull, ACTION-IF-FAILURE otherwise.
#  PROLOGUE and BODY are optional, and should be used as in they are
#  in the AC_LANG_PROGRAM macro.
#  The check is cached, and the cache variable is named:
#   `ax_cv_cc_check_flag_FLAG'
#  where FLAG is obtained normalizing `FLAG-TO-CHECK' (i.e. by substituting
#  non-alphanumeric characters in FLAG-TO-CHECK with underscores `_').
AC_DEFUN([AX_CACHE_CHECK_CC_FLAG],
    [ax_cc_flag_norm=`echo x"$1" | sed -e 's/^x//' -e 's/[[^A-Za-z0-9_]]/_/g'`
    AC_CACHE_CHECK(
        [whether the C compiler accepts the $1 flag],
        [ax_cv_check_cc_flag_${ax_cc_flag_norm}],
        [_AX_CHECK_CC_FLAG(
            [$1], [$2], [$3],
            [eval "ax_cv_check_cc_flag_${ax_cc_flag_norm}=yes"],
            [eval "ax_cv_check_cc_flag_${ax_cc_flag_norm}=no"])])
    
    AS_IF(
        [eval test \"\$ax_cv_check_cc_flag_${ax4_cc_flag_norm}\" = yes],
        [$4],
        [$5])
    AS_UNSET([ax_cc_flag_norm])])

# vim: ft=m4 ts=4 sw=4 et
