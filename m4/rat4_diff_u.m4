# serial 1 ratfor_diff_u.m4
# $Id$

# Look for a diff command that can produce better human-readable output.
# IMPORTANT NOTE: the cache variable *must* have the format `ac_cv_*' to
# work; using e.g. `rat4_cv_path_UDIFF' seems tempting, but does not work.
AC_DEFUN([RAT4_PROG_DIFF_U],
    [AC_CACHE_CHECK(
        [for a diff command producing human-readable output],
        [ac_cv_path_UDIFF],
        [AC_PATH_PROGS_FEATURE_CHECK(
            [UDIFF],
            [diff gdiff],
            [echo x >confdf1 && echo x >confdf2 \
               && $ac_path_UDIFF -u confdf1 confdf2 >/dev/null 2>&1 \
               && ac_path_UDIFF_found=: \
               && ac_cv_path_UDIFF=$ac_path_UDIFF \
               && DIFF_U="$ac_path_UDIFF -u"
             rm -f confdf1 confdf2],
            [AC_MSG_RESULT([None found.])
             DIFF_U=diff])])
    AC_SUBST([DIFF_U])])

# vim: ft=m4 ts=4 sw=4 et
