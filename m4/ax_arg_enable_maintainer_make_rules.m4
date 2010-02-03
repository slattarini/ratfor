# -*- Autoconf -*-
# Copied from SteLib at 2010-02-03 16:07:07 +0100.  DO NOT EDIT!
# serial 1 ax_arg_enable_maintainer_make_rules.m4

#
# Copyright (C) 2010 Lattarini Stefano <stefano.lattarini@gmail.com>
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

#
# AX_ARG_ENABLE_MAINTAINER_MAKE_RULES
# -----------------------------------
# Enable maintainer-specific make rules, on user request through the option
# `--enable-maintainer-make-rules'.
# For this macro to be used meaningfully, the package developer must setup
# proper maintainer-specific rules by itself, to be placed in a GNUmakefile
# wrapping the package's top-level Makefile, and to be actived only when
# the Automake conditional `ENABLE_MAINTAINER_MAKE_RULES' is set to "true".
#
AC_DEFUN([AX_ARG_ENABLE_MAINTAINER_MAKE_RULES],
    [AX_ARG_ENABLE(
        [maintainer-make-rules],
        [Import maintaner-specific rules in the GNUmakefile. Use it only
         if you have GNU make, and you know what you are doing. Also, note
         that this option has *NOTHING* to do with the infamous automake's
         "maintainer-mode".],
        [ax_enable_maintainer_make_rules=yes],
        [ax_enable_maintainer_make_rules=no])
    m4_pattern_allow([ENABLE_MAINTAINER_MAKE_RULES])
    AM_CONDITIONAL([ENABLE_MAINTAINER_MAKE_RULES],
                   [test x"$enable_maintainer_make_rules" = x"yes"])])

# vim: ft=m4 ts=4 sw=4 et
