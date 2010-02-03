#-*- Autoconf -*-
# Copied from SteLib at 2010-02-03 18:07:04 +0100.  DO NOT EDIT!
# serial 3 ax_misc_include_maint_config_site.m4

#
# Copyright (C) 2009, 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_INCLUDE_MAINT_CONFIG_SITE([MAINT-DIR])
# -------------------------------------------
# Include content of file `MAINT-DIR/config.site', if it exists (MAINT-DIR
# defaults to `$srcdir/maint').  If an error occurs, abort the configure
# script.  This macro should be used called in configure.ac, in particular
# before any check and, if using automake, before `AM_INIT_AUTOMAKE'.
AC_DEFUN([AX_INCLUDE_MAINT_CONFIG_SITE],
    [ax_maint_config_site=m4_default([$1], [$srcdir/maint])/config.site
    if test -f "$ax_maint_config_site"; then
      AC_MSG_NOTICE([loading mainainer script `$ax_maint_config_site'])
      . "$ax_maint_config_site"
      if test $? -ne 0; then
        AC_MSG_ERROR([failed to source script `$ax_maint_config_site'])
      fi
    fi])

# vim: ft=m4 ts=4 sw=4 et
