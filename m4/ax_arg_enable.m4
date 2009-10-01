# -*- Autoconf -*-
# Copied from SteLib at 2009-10-01 21:17:40 +0200.  DO NOT EDIT!
# serial 1 ax_arg_enable.m4


# AX_ARG_ENABLE(ENABLE-FLAG, HELP-STRING, [ACTION-IF-GIVEN],
#               [ACTION-IF-NOT-GIVEN],[DEFAULT-ACTION])
# ------------------------------------------------------------
#  Wrapper around `AC_ARG_ENABLE'. If DEFAULT-ACTION is not given, it
# defauts to ACTION-IF-NOT-GIVEN.
AC_DEFUN([AX_ARG_ENABLE],
    [AC_ARG_ENABLE(
        [$1],
        [AC_HELP_STRING(
            [--enable-$1],
            [$2])],
        [AS_CASE(
            ["$enableval"],
            [[[yY]|[yY]es|YES|1|[tT]rue]],
                [m4_default([$3], [:])],
            [[[nN]|[nN]o|NO|0|[fF]alse]],
                [m4_default([$4], [:])],
            [ax_failmsg="Invalid value \`$enableval' specified"
             ax_failmsg="$failmsg for option \`--enable-$1'"
            AC_MSG_ERROR([$ax_failmsg])])],
        [m4_default([$5], [$4])])])

# vim: ft=m4 ts=4 sw=4 et
