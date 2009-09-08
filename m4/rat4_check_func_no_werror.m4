# serial 1 rat4_check_func_no_werror.m4

# internal macro
AC_DEFUN([_RAT4_CHECK_FUNC_NO_WERROR_DEFINE_HELPER_SUB],
[rat4_fn_prune_Werror() {
    echo " $[1] " | sed -e 's/[[ 	]]-Werror[[ 	]]/ /g'
}])

# Like AC_CHEC_FUNC, but be sure ont to have the `-Werror' compiler switch
# on, as it might sometimes botch up results from AC_CHECK_FUNC.
AC_DEFUN([RAT4_CHECK_FUNC_NO_WERROR],
    [AC_REQUIRE([_RAT4_CHECK_FUNC_NO_WERROR_DEFINE_HELPER_SUB])
    for v in CC CFLAGS CCFLAGS CPPFLAGS LDFLAGS; do
        eval save_$v=\$$v
        eval $v=\`rat4_fn_prune_Werror \"\$$v\"\`
    done
    AC_CHECK_FUNC([$1], [$2], [$3])
    for v in CC CFLAGS CCFLAGS CPPFLAGS LDFLAGS; do
        eval $v=\$save_$v
    done])

# vim: ft=m4 ts=4 sw=4 et
