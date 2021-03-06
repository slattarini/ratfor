#-*- Autoconf -*-
# Copied from SteLib at 2010-02-17 00:50:49 +0100.  DO NOT EDIT!
# serial 4 ax_arg_enable.m4

#
# Copyright (C) 2008-2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_ARG_ENABLE(ENABLE-FLAG, HELP-STRING, [ACTION-IF-GIVEN],
#               [ACTION-IF-NOT-GIVEN], [DEFAULT-ACTION])
# ------------------------------------------------------------
#  Wrapper around `AC_ARG_ENABLE'. If DEFAULT-ACTION is not given, it
#  defauts to ACTION-IF-NOT-GIVEN.
AC_DEFUN([AX_ARG_ENABLE],
    [AC_ARG_ENABLE(
        [$1],
        [AC_HELP_STRING(
            [--enable-$1],
            [$2])],
        [AS_CASE(
            ["$enableval"],
            [yes|true|1],
                [m4_default([$3], [:])],
            [no|false|0],
                [m4_default([$4], [:])],
            [AC_MSG_ERROR(m4_do([invalid value \`$enableval' specified],
                                [ for option \`--enable-$1']))])],
        [m4_default([$5], [$4])])])

# vim: ft=m4 ts=4 sw=4 et
