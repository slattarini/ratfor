#-*- Autoconf -*-
# serial 2 rat4_prog_f77.m4
#
#--------------------------------------------------------------------------
#
# Copyright (C) 2009, 2010 Lattarini Stefano.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# and this notice are preserved.
#
#--------------------------------------------------------------------------
#
# Find a Fortran 77 compiler, to be used by the ratfor testsuite.
# First look for a user-provided compiler (expected to be found in
# variable `F77'), else search the system for a valid and working
# Fortran 77 compiler, using the `AC_PROG_F77' macro.
#
AC_DEFUN([RAT4_PROG_F77],
  [AC_ARG_VAR([F77],
    [Fortran 77 compiler to be used in testing. If empty, configure will
     scan the system looking for a working Fortran 77 compiler. If set to
     the special value `NONE', no Fortran 77 compiler will be used in the
     tests (the tests requiring such a compiler will be skipped).])
   AS_IF([test x"$F77" = x"NONE"],
    [AC_MSG_NOTICE([Fortran 77 compiler must not be used in testing])],
    [AC_PROG_F77()
     if test -n "$F77"; then
       AC_MSG_NOTICE([Fortran 77 compiler to be used in testing: $F77])
     else
       AC_MSG_WARN([No Fortran 77 compiler found to be used in testing])
       AC_MSG_WARN([Some parts of the testsuite will be skipped])
     fi])])

# vim: ft=m4 ts=4 sw=4 et
