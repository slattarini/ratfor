# -*- Autoconf -*-
# Copied from SteLib at 2010-10-27 14:50:53 +0200.  DO NOT EDIT!
# serial 4 ax_arg_enable_werror_cflag.m4

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
# not the `-Werror' flag should be added (when supported) to compiler
# calls.
# If the option was given, set the shell variable `ax_use_cc_werror_flag'
# to "yes", and define the AC_SUBST'd variable `WERROR_CFLAGS' to
# "-Werror" (if supported) or to empty (if not).   If the option was not
# given, just set the shell variable `ax_use_cc_werror_flag' to "no" and
# define `WERROR_CFLAGS' to empty.
AC_DEFUN([AX_ARG_ENABLE_WERROR_CFLAG],
    [AX_ARG_ENABLE(
        [werror-cflag],
        [enable the `-Werror' C compiler flag, if supported],
        [ax_use_cc_werror_flag=yes],
        [ax_use_cc_werror_flag=no])
    AS_VAR_IF([ax_use_cc_werror_flag], [yes],
        [AX_CACHE_CHECK_CC_FLAG(
            [-Werror], [], [],
            [WERROR_CFLAGS='-Werror'],
            [WERROR_CFLAGS=''])])
    AC_SUBST([WERROR_CFLAGS])])

# vim: ft=m4 ts=4 sw=4 et
