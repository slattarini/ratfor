# serial 1 rat4_fortran_stop_and_exit.m4

# -----------------------------------------------------------------------
# Internal subroutine; usage:
# _RAT4_QUIET_FORTRAN_STMT_IFELSE([FORTRAN-STMT],
#                                 [ACTION-IF-SUCCESS], [ACTION-IF-FAIL])
# -----------------------------------------------------------------------
AC_DEFUN([_RAT4_QUIET_FORTRAN_STMT_IFELSE],
  [AC_LANG_PUSH([Fortran 77])
  AC_LINK_IFELSE(
    [AC_LANG_PROGRAM([], [[      $1]])],
    [AS_IF(
      [./conftest$EXEEXT >conftest.out 2>&1 && test ! -s conftest.out],
      [$2], [$3])],
    [$3])
  AC_LANG_POP([Fortran 77])])


AC_DEFUN([RAT4_HAVE_SILENT_FORTRAN_STOP_BUILTIN],
    [AC_CACHE_CHECK(
        [whether `stop' Fortran builtin is quiet],
        [rat4_cv_have_fortran_silent_stop_builtin],
        [_RAT4_QUIET_FORTRAN_STMT_IFELSE(
            [stop],
            [rat4_cv_have_fortran_silent_stop_builtin=yes],
            [rat4_cv_have_fortran_silent_stop_builtin=no])])])


AC_DEFUN([RAT4_HAVE_SILENT_FORTRAN_EXIT_SUBROUTINE],
    [AC_CACHE_CHECK(
        [whether Fortran have a quiet `exit' subroutine],
        [rat4_cv_have_fortran_exit_subroutine],
        [_RAT4_QUIET_FORTRAN_STMT_IFELSE(
            [call exit(0)],
            [rat4_cv_have_fortran_exit_subroutine=yes],
            [rat4_cv_have_fortran_exit_subroutine=no])])])


# vim: ft=m4 ts=4 sw=4 et
