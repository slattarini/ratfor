#-*- Autoconf -*-
# Copied from SteLib at 2010-02-17 00:50:49 +0100.  DO NOT EDIT!
# serial 7 ax_misc_report_if_beta.m4

#
# Copyright (C) 2008-2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>.
#
# Copying and distribution of this file, with or without modification, are
# permitted in any medium without royalty provided the copyright notice and
# this notice are preserved.
#

# AX_REPORT_IF_BETA()
# -------------------
# If the distributed package is determined to be a alpha/beta release,
# then display a suitable warning message on standard error.
# The package is considered to be an alpha/beta release if
# AC_PACKAGE_VERSION matches the (extended) regexp '[a-z]+[0-9]*'; e.g.,
# package whose distname is 'foo-3.0a', 'bar-3.0beta1' or 'foo-3.0a2' are
# considered to be alpha/beta releases.
AC_DEFUN([AX_REPORT_IF_BETA],
dnl*| if AC_PACKAGE_VERSION matches the following regexp...
    [m4_bmatch(m4_defn([AC_PACKAGE_VERSION]), [\([a-z]+[0-9]*\)$],
dnl*| ... then emit the following code.
        [cat >&2 <<\EOF

WARNING: You are about to use an experimental version of AC_PACKAGE_NAME.
It may suffer of strange bugs and undocumented incompatibilities,
so beware.
EOF

ax_report_if_beta__list=''
ax_report_if_beta__seen=no
ax_report_if_beta__what=file
for ax_report_if_beta__f in README.alpha README.beta BUGS TODO; do
  test -f "$srcdir/$ax_report_if_beta__f" || continue
  if test x"$ax_report_if_beta__seen" = x"yes"; then
    AS_VAR_APPEND([ax_report_if_beta__list], [", $ax_report_if_beta__f"])
    ax_report_if_beta__what=files
  else
    ax_report_if_beta__seen=yes
    ax_report_if_beta__list=$ax_report_if_beta__f
  fi
done
if test x"$ax_report_if_beta__seen" = x"yes"; then
    echo "" >&2
    echo "Be sure to read carefully the $ax_report_if_beta__what" \
         "$ax_report_if_beta__list before proceeding." >&2
fi

AS_UNSET([ax_report_if_beta__list])
AS_UNSET([ax_report_if_beta__seen])
AS_UNSET([ax_report_if_beta__what])
AS_UNSET([ax_report_if_beta__f])dnl
])dnl* m4_bmatch
])dnl* AC_DEFUN

# vim: ft=m4 ts=4 sw=4 et
