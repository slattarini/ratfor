# rat4_sh_errexit_works.m4 serial 1
# $Id$

# Check if the $SHELL supports `trap "..." EXIT' and `set -e' that play
# well togheter. Properly AC_SUBST'd the `SH_ERREXIT_WORKS' variable
# to "yes" or "no", depending on the outcome of the test.
AC_DEFUN([RAT4_CHECK_SH_ERREXIT_WORKS],
    [AC_CACHE_CHECK(
        [whether \$SHELL=$SHELL has working 'set -e' with exit trap],
        [rat4_cv_sh_errexit_works],
        [if $SHELL -c "set -e; trap 'exit \$?' 0; (exit 77); exit 77";
            test $? = 77;
         then
            rat4_cv_sh_errexit_works=yes
        else
            rat4_cv_sh_errexit_works=no
        fi])
    AC_SUBST([SH_ERREXIT_WORKS], [$rat4_cv_sh_errexit_works])])

# vim: ft=m4 ts=4 sw=4 et
