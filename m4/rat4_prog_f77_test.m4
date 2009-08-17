# ratfor_prog_f77_testing.m4 serial 2
# $Id$

# Get a list of available fortran compilers, to be used in testing.
# First look for a user-provided list, else search the system for a
# valid and working F77 compiler.
# Put the list of the compilers in the RAT4_TESTSUITE_F77_LIST, and declare
# it as a "precious" variable (so that in particular it is autoomatically
# AC_SUBST'd).

AC_DEFUN([RAT4_PROG_F77_FOR_TESTSUITE],
   [dnl ---
    AC_ARG_VAR([RAT4_TESTSUITE_F77_LIST],
      [List of Fortran 77 compilers to be used in testing (should all be
       given as absolute paths!). If empty, configure will scan the system
       looking for a working Fortran 77 compiler.])
    dnl ---
    AC_MSG_CHECKING([a list of Fortran 77 compilers to be used in testing])
    if test -n "$RAT4_TESTSUITE_F77_LIST"; then
      : # do nothing: user choiche override our checks
      AC_MSG_RESULT([got a user-provided list!])
    else
      AC_MSG_RESULT(
        [no default list available])
      AC_MSG_NOTICE(
        [Now will scan the system to find a Fortran 77 compiler])
      AC_PROG_F77
      RAT4_TESTSUITE_F77_LIST=$F77
    fi
    if test -n "$RAT4_TESTSUITE_F77_LIST"; then
      AC_MSG_NOTICE(
        m4_do([Fortran 77 compilers to be used in testing],
              [$RAT4_TESTSUITE_F77_LIST]))
    else
      AC_MSG_WARN([No Fortran 77 compiler found to be used in testing])
      AC_MSG_WARN([Some parts of the testsuite will be skipped])
    fi
    # TODO: check that all compilers in $RAT4_TESTSUITE_F77_LIST work,
    #       bailout on error
])

# vim: ft=m4 ts=4 sw=4 et
