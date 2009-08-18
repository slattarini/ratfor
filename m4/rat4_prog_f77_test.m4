# serial 3 ratfor_prog_f77_testing.m4
# $Id$

# Get a Fortran 77 compiler, to be used in testing.
# First look for a user-provided compiler (expected to be found in variable
# `RAT4_TESTSUITE_F77 list'), else search the system for a valid and
# working Fortran 77 compiler, using the `AC_PROG_F77' macro.
# Put the detected compiler in `RAT4_TESTSUITE_F77' (this is set empty if
# no compiler was found), and declare it as a "precious" variable (so that
# in particular it is automatically AC_SUBST'd).

AC_DEFUN([RAT4_PROG_F77_FOR_TESTSUITE],
   [dnl ---
    AC_ARG_VAR([RAT4_TESTSUITE_F77],
      [List of Fortran 77 compilers to be used in testing (should all be
       given as absolute paths!). If empty, configure will scan the system
       looking for a working Fortran 77 compiler.])
    dnl ---
    AC_MSG_CHECKING([a of Fortran 77 compilers to be used in testing])
    if test -n "$RAT4_TESTSUITE_F77"; then
      : # do nothing: user choiche override our checks
      AC_MSG_RESULT([got a user-provided value!])
    else
      AC_MSG_RESULT(
        [no default value available])
      AC_MSG_NOTICE(
        [Now will scan the system to find a Fortran 77 compiler])
      AC_PROG_F77()
      RAT4_TESTSUITE_F77=$F77
    fi
    if test -n "$RAT4_TESTSUITE_F77"; then
      AC_MSG_NOTICE(
        m4_do([Fortran 77 compiler to be used in testing:],
              [$RAT4_TESTSUITE_F77]))
    else
      AC_MSG_WARN([No Fortran 77 compiler found to be used in testing])
      AC_MSG_WARN([Some parts of the testsuite will be skipped])
    fi
])

# vim: ft=m4 ts=4 sw=4 et
