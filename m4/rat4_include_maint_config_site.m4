# serial 1 rat4_include_maint_config_site.m4

# RAT4_INCLUDE_MAINT_CONFIG_SITE([MAINT-DIR])
# -------------------------------------------
# Include content of file `MAINT-DIR/config.site', it it exists
# (`MAINT-DIR' defaults to `$srcdir/maint'). If an error occurs,
# abort the configure script.
AC_DEFUN([RAT4_INCLUDE_MAINT_CONFIG_SITE],
    [rat4_maint_config_site=m4_default([$1], [$srcdir/maint])/config.site
    if test -f "$rat4_maint_config_site"; then
      AC_MSG_NOTICE([loading mainainer script `$rat4_maint_config_site'])
      . "$rat4_maint_config_site"
      if test $? -ne 0; then
        AC_MSG_ERROR([failed to source script `$rat4_maint_config_site'])
      fi
    fi])

# vim: ft=m4 ts=4 sw=4 et
