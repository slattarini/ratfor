# -*- Autoconf -*-
# Copied from SteLib at 2010-10-27 14:50:38 +0200.  DO NOT EDIT!
# serial 5 ax_cc_enable_all_warnings.m4

#
# Copyright (C) 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_CC_ENABLE_ALL_COMPILER_WARNINGS([EXTRA-WARNINGS])
# ----------------------------------------------------
# Enable as many compiler warnings as possible (this is tailered to gcc,
# but should not cause failures of problems for other compilers).  Try to
# enable also the warning flags specified in EXTRA-WARNING, in addition
# to the builtin list of warnings flags.  Save the list of supported
# warnings in the AC_SUBST'd variable `WARN_CFLAGS'.
AC_DEFUN([AX_CC_ENABLE_ALL_COMPILER_WARNINGS],
    [AC_REQUIRE([AX_CACHE_CHECK_CC_IS_CXX])dnl
    # Enable as many compiler warnings as possible.
    ax_cc_all_warning_flags="
        -Wall
        -Wextra
        -Wshadow
        -Wmissing-declarations
        -Wwrite-strings
        -Wsign-conversion
        $*
    "
    if test x"$ax_cv_check_cc_is_cxx" != x"yes"; then
        ax_cc_all_warning_flags="
            $ax_cc_all_warning_flags
            -Wnested-externs
            -Wc++-compat
            -Wdeclaration-after-statement"
    fi
    # The C compiler flag `-pedantic' can confuse third party compilers
    # (e.g. SunStudio cc), so try it only if we know that the C compiler
    # is gcc.
dnl: The best fix in the long run would be to make AX_CACHE_CHECK_CC_FLAG
dnl: smarter and more reliable.  Oh well.
    if test x"$GCC" = x"yes"; then
        AS_VAR_APPEND([ax_cc_all_warning_flags], [" -pedantic"])
    fi
    WARN_CFLAGS=''
    for ax_cc_warning_flag in $ax_cc_all_warning_flags; do
        AX_CACHE_CHECK_CC_FLAG([$ax_cc_warning_flag], [], [],
                               [AS_VAR_APPEND([WARN_CFLAGS],
                                              [" $ax_cc_warning_flag"])])
    done
    AS_UNSET([ax_cc_warning_flag])
    AC_SUBST([WARN_CFLAGS])
])

# vim: ft=m4 ts=4 sw=4 et
