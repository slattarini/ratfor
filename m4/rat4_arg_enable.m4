# serial 1 rat4_arg_enable.m4

# RAT4_ARG_ENABLE(ENABLE-FLAG, HELP-STRING, [ACTION-IF-GIVEN],
#                 [ACTION-IF-NOT-GIVEN],[DEFAULT-ACTION])
# ------------------------------------------------------------
#  Wrapper around `AC_ARG_ENABLE'. If DEFAULT-ACTION is not given, it
# defauts to ACTION-IF-NOT-GIVEN.
AC_DEFUN([RAT4_ARG_ENABLE],
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
            [rat4_failmsg="Invalid value \`$enableval' specified"
             rat4_failmsg="$failmsg for option \`--enable-$1'"
            AC_MSG_ERROR([$rat4_failmsg])])],
        [m4_default([$5], [$3])])])

# vim: ft=m4 ts=4 sw=4 et
