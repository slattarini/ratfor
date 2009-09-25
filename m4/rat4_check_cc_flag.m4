# serial 2 rat4_check_cc_flag.m4

# _RAT4_CHECK_CC_FLAG(FLAG-TO-CHECK,[PROLOGUE],[BODY],
#                     [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# -------------------------------------------------------------
# For internal usage only.
AC_DEFUN([_RAT4_CHECK_CC_FLAG],
    [AC_LANG_PUSH([C])
    rat4_save_CFLAGS="$CFLAGS"
    CFLAGS="$CFLAGS $1"
    AC_COMPILE_IFELSE([AC_LANG_PROGRAM([$2],[$3])], [$4], [$5])
    CFLAGS="$rat4_save_CFLAGS"
    AS_UNSET([rat4_save_CFLAGS])
    AC_LANG_POP()])

# RAT4_CACHE_CHECK_CC_FLAG(FLAG-TO-CHECK, [PROLOGUE], [BODY], 
#                          [ACTION-IF-SUCCESS], [ACTION-IF-FAILURE])
# ------------------------------------------------------------------
#  This macro check if the C compiler supports the flag FLAG-TO-CHECK.
#  Execute ACTION-IF-SUCCESS if succesfull, ACTION-IF-FAILURE otherwise.
#  PROLOGUE and BODY are optional, and should be used as in they are
#  in the AC_LANG_PROGRAM macro.
#  The check is cached, and the cache variable is named:
#   `rat4_cv_cc_check_flag_FLAG'
#  where FLAG is obtained normalizing `FLAG-TO-CHECK' (i.e. by substituting
#  non-alphanumeric characters in FLAG-TO-CHECK with underscores `_').
AC_DEFUN([RAT4_CACHE_CHECK_CC_FLAG],
    [rat4_cc_flag_norm=`echo x"$1" | sed -e 's/^x//' -e 's/[[^A-Za-z0-9_]]/_/g'`
    AC_CACHE_CHECK(
        [whether the C compiler accepts the $1 flag],
        [rat4_cv_check_cc_flag_$rat4_cc_flag_norm],
        [_RAT4_CHECK_CC_FLAG(
            [$1], [$2], [$3],
            [eval "rat4_cv_check_cc_flag_$rat4_cc_flag_norm=yes"],
            [eval "rat4_cv_check_cc_flag_$rat4_cc_flag_norm=no"])])
    
    AS_IF(
        [eval test \"\$rat4_cv_check_cc_flag_$rat4_cc_flag_norm\" = yes],
        [$4],
        [$5])
    AS_UNSET([rat4_cc_flag_norm])])

# vim: ft=m4 ts=4 sw=4 et
