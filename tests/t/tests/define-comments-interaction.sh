#!/bin/sh
# Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>.
# This file is part of the Ratfor Testsuite.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

# Helper file, sourced by scripts checking that the `define' builtin,
# when used in the form `define(x, ...)', preserve comments.  This
# file is expected to be sourced by test scripts after the file
# `rat4-testsuite-init.sh' has already been sourced.

set +x
echo "$me: INFO: disable shell verbosity while defining shell function"

ctrlerr() {
    testcase_HARDERROR "\"control file\" ctrl.r: \`ratfor -C' $*"
}

test_with_commented_strings() {

    $SED -e 's/^    //' >tst.r <<EOF
    define(MACRO1,
    foo=1 # this macro contains one c@mment
    )
    define(MACRO2,
    # c@mment1 $1
    bar=1
    bar=2 # c@mment2 $2
    )
    tt
    MACRO1
    tt
    MACRO1 x=y
    tt
    MACRO2
    tt
    MACRO2 if(u == v) pass
EOF

    $SED -e 's/^    //' >ctrl.r <<EOF
    tt
    foo=1 # this macro contains one c@mment
    tt
    foo=1 # this macro contains one c@mment
    x=y
    tt
    # c@mment1 $1
    bar=1
    bar=2 # c@mment2 $2
    tt
    # c@mment1 $1
    bar=1
    bar=2 # c@mment2 $2
    if (u == v) pass
EOF

    cat ctrl.r
    run_RATFOR -C ctrl.r || ctrlerr "failed"
    mv stdout ctrl-stdout
    mv stderr ctrl-stderr
    test ! -s ctrl-stderr || ctrlerr "complained on stderr"
    $FGREP "this macro contains one c@mment" ctrl-stdout \
      && $FGREP "c@mment1 $1" ctrl-stdout \
      && $FGREP "c@mment2 $2" ctrl-stdout \
      || ctrlerr "dind't kept comments"
    $SED "/^$ws0p$/d" ctrl-stdout > exp

    cat tst.r
    run_RATFOR -C tst.r || testcase_FAIL
    test ! -s stderr || testcase_FAIL
    $SED "/^$ws0p$/d" stdout > got

    cat exp
    cat got

    $DIFF_U exp got || testcase_FAIL

    testcase_DONE
}

echo "$me: INFO: reactivate shell verbosity before running tests"
set -x

# vim: ft=sh ts=4 sw=4 et
