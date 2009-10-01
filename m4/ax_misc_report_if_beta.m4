#-*- Autoconf -*-
# Copied from SteLib at 2009-10-01 16:42:13 +0200.  DO NOT EDIT!
# serial 4 ax_misc_report_if_beta.m4

#
# Copyright (C) 2008, 2009 Lattarini Stefano <stefano.lattarini@gmail.com>
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

ax_flist=""
ax_s=""
for ax_fname in README.alpha BUGS TODO; do
    f="$srcdir/$ax_fname"
    test -f "$f" || continue
    if test -z "$ax_flist"; then
        ax_flist=$ax_fname
        ax_flist2=$ax_fname
        ax_s=""
    else
        ax_flist="$ax_flist2 and $ax_fname"
        ax_flist2="$ax_flist2, $ax_fname"
        ax_s="s"
    fi
done
if test -n "$ax_flist"; then
    cat >&2 <<EOF

Be sure to read carefully the file${ax_s} $ax_flist before proceeding.
EOF
fi

AS_UNSET([ax_fname])
AS_UNSET([ax_flist])
AS_UNSET([ax_flist2])
AS_UNSET([ax_s])dnl
])dnl* m4_bmatch
])dnl* AC_DEFUN

# vim: ft=m4 ts=4 sw=4 et
