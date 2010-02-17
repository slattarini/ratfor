# -*- Autoconf -*-
# Copied from SteLib at 2010-02-17 01:09:10 +0100.  DO NOT EDIT!
# serial 3 ax_arg_enable_werror_cflag.m4

#
# Copyright (C) 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_ARG_ENABLE_WERROR_CFLAG
# --------------------------
# Add to configure an option `--enable-werror-cflag', telling whether or
# not the `-Werror' flag should be added (when supported) to $CFLAGS.
# If the option was given, set the shell variable `ax_use_cc_werror_flag'
# to "yes", and add `-Werror' to $CFLAGS (if it's supported); otherwise,
# just set the shell variable `ax_use_cc_werror_flag' to "no".
AC_DEFUN([AX_ARG_ENABLE_WERROR_CFLAG],
    [AX_ARG_ENABLE(
        [werror-cflag],
        [enable the `-Werror' C compiler flag, if supported],
        [ax_use_cc_werror_flag=yes],
        [ax_use_cc_werror_flag=no])
    AS_VAR_IF([ax_use_cc_werror_flag], [yes],
        [AX_CACHE_CHECK_CC_FLAG(
            [-Werror], [], [],
            [AS_VAR_APPEND([CFLAGS], [" -Werror"])])])])

# vim: ft=m4 ts=4 sw=4 et
