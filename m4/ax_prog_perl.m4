#-*- Autoconf -*-
# serial 1 ax_prog_perl.m4

#
# Copyright (C) 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# and this notice are preserved.
#

# AX_PROG_PERL([PERL-VERSION], [PERL-PROGS], [FALLBACK-PERL])
# -----------------------------------------------------------
# Look for a working perl interpreter having version PERL-VERSION or later
# (default: `5.006'), by seaching in PATH each of the program PERL-PROGS
# (default `perl perl5').
# If a proper perl program is found, set $PERL to its absolute path,
# else set it to `FALLBACK-PERL' (default `false').
# Also call AC_SUBST on PERL.
# NOTE 1: `PERL-VERSION' should be specified in a format accepted by the
#          `require' perl builtin.
# NOTE 2: `FALLBACK-PERL' is used insede a duoble-quote strings, so it
#          should be properly escaped if containing characters like `"'
#          or `$'.
AC_DEFUN([AX_PROG_PERL],
  dnl*  IMPORTANT NOTE: the cache variable *must* have the format
  dnl*  `ac_cv_*' to work; using e.g. `ax_cv_path_PERL' might seem
  dnl*  tempting, but it doesn't work.
  [m4_pushdef([_$0_ReqPerlVersion], [m4_default([$1], [5.006])])
  AC_CACHE_CHECK(
    [for a perl interpreter, version _$0_ReqPerlVersion or later],
    [ac_cv_path_PERL],
    [AC_PATH_PROGS_FEATURE_CHECK(
      [PERL],
      [m4_default([$2], [perl perl5])],
      [$ac_path_PERL -e "require _$0_ReqPerlVersion;" >/dev/null 2>&1 \
        && test x"`echo '@' | $ac_path_PERL -npe 's{@}{.}' 2>&1`" = x"." \
        && ac_path_PERL_found=: \
        && ac_cv_path_PERL=$ac_path_PERL \
        && PERL="$ac_path_PERL"],
      [AC_MSG_RESULT([none found.])
       PERL="m4_default([$3], [false])"])])
  m4_popdef([_$0_ReqPerlVersion])
  AC_SUBST([PERL])])

# vim: ft=m4 ts=2 sw=2 et
