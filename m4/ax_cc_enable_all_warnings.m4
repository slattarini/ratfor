# -*- Autoconf -*-
# Copied from SteLib at 2010-02-03 18:07:04 +0100.  DO NOT EDIT!
# serial 2 ax_cc_is_cxx_support.m4

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
# to the builtin list of warnings flags.
# IMPORTANT NOTE: this macro *must* be expanded only after any check on
# C compiler, C headers, C functions or C libraries, as stricter handling
# of warnings can interfere with usual Autoconf checks.
AC_DEFUN([AX_CC_ENABLE_ALL_COMPILER_WARNINGS],
    [AC_REQUIRE([AX_CACHE_CHECK_CC_IS_CXX])dnl
    # Enable as many compiler warnings as possible.
    ax_gcc_all_warning_flags="
        -Wall
        -Wextra
        -Wshadow
        -Wmissing-declarations
        -Wwrite-strings
        -Wsign-conversion
        -pedantic
        $*
    "
    if test x"$ax_cv_check_cc_is_cxx" != x"yes"; then
        ax_gcc_all_warning_flags="
            $ax_gcc_all_warning_flags
            -Wnested-externs
            -Wc++-compat
            -Wdeclaration-after-statement"
    fi
    for ax_gcc_warning_flag in $ax_gcc_all_warning_flags; do
        AX_CACHE_CHECK_CC_FLAG([$ax_gcc_warning_flag], [], [],
                               [CFLAGS="$CFLAGS $ax_gcc_warning_flag"])
    done
    AS_UNSET([ax_gcc_warning_flag])
])

# vim: ft=m4 ts=4 sw=4 et
