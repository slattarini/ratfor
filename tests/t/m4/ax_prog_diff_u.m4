#-*- Autoconf -*-
# Copied from SteLib at 2010-02-17 00:50:49 +0100.  DO NOT EDIT!
# serial 5 ax_prog_diff_u.m4

#
# Copyright (C) 2009, 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# and this notice are preserved.
#

# AX_PROG_DIFF_U([DIFF-PROGS])
# ----------------------------
# Look for a diff command that can output unified diffs, by seaching in
# PATH each of the program DIFF-PROGS (default `diff gdiff'), and then
# testing it too se if it works as expected.
# If a proper diff program is found at, say, /path/to/diff, set $DIFF_U
# to `/path/to/diff -u', else set it to simply `diff'. Also call AC_SUBST
# on DIFF_U.
AC_DEFUN([AX_PROG_DIFF_U],
    dnl| IMPORTANT NOTE: the cache variable *must* have the format
    dnl| `ac_cv_*' to work; using e.g. `ax_cv_path_UDIFF' seems
    dnl| tempting, but does not work.
    [AC_CACHE_CHECK(
        [for a diff command supporting unified diffs],
        [ac_cv_path_UDIFF],
        [AC_PATH_PROGS_FEATURE_CHECK(
            [UDIFF],
            [diff gdiff],
            [$ac_path_UDIFF -u /dev/null /dev/null >/dev/null 2>&1 \
               && ac_path_UDIFF_found=: \
               && ac_cv_path_UDIFF=$ac_path_UDIFF \
               && DIFF_U="$ac_path_UDIFF -u"],
            [AC_MSG_RESULT([none found.])
             DIFF_U=diff])])
    AC_SUBST([DIFF_U])])

# vim: ft=m4 ts=4 sw=4 et
