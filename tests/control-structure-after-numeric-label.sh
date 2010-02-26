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

# Helper file, sourced by scripts checking (through grep) that ratfor
# control statements (if, while, for, etc) are processed correctly when
# found after a numerical label.  This file is expected to be sourced by
# test scripts after the file `rat4-testsuite-init.sh' has already been
# sourced.

set +x
echo "$me: INFO: disable shell verbosity while defining shell function"

testgrep_control_structure_after_label() {

    funcname=testgrep_control_structure_after_label # for error messages

    fail_if_stmt_is_in_output=:
    fail_if_stmt_post_is_in_output='' # default will be set later
    while test $# -gt 0; do
        case $1 in
            '--dont-fail-if-stmt-is-in-output')
                fail_if_stmt_is_in_output=false
                ;;
            '--fail-if-stmt-is-in-output')
                fail_if_stmt_is_in_output=:
                ;;
            '--dont-fail-if-stmt-post-is-in-output')
                fail_if_stmt_post_is_in_output=false
                ;;
            '--fail-if-stmt-post-is-in-output')
                fail_if_stmt_post_is_in_output=:
                ;;
            -*)
                testcase_HARDERROR "$funcname: $1: invalid option"
                ;;
             *)
                break
                ;;
        esac
        shift
    done

    test $# -ge 2 || testcase_HARDERROR "$funcname: missing argument"

    stmt=$1
    stmt_arg=$2
    stmt_post=${3-}
    stmt_post_arg=${4-}
    shift $#

    if test -z "$fail_if_stmt_post_is_in_output"; then
        # set default for $fail_if_stmt_post_is_in_output
        if test -z "$stmt_post"; then
            fail_if_stmt_post_is_in_output=false
        else
            fail_if_stmt_post_is_in_output=:
        fi
    fi

    echo "100 $stmt $stmt_arg pass; $stmt_post $stmt_post_arg" > tst.r
    cat tst.r

    run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
    test -s stderr && testcase_FAIL "ratfor produced diagnostic on stderr"

    $SED -e '/^[cC]/d' stdout > out
    cat out

    if $fail_if_stmt_is_in_output; then
        $FGREP "$stmt" out \
          && testcase_FAIL "string \"$stmt\" found in ratfor output"
    fi

    $GREP "^100 .*pass" out \
      && testcase_FAIL "statement \"$stmt\" not expanded correctly"

    if $fail_if_stmt_post_is_in_output; then
        $FGREP "$stmt_post" out \
          && testcase_FAIL "string \"$stmt_post\" found in ratfor output"
    fi

    testcase_DONE
}

echo "$me: INFO: reactivate shell verbosity before running tests"
set -x

# vim: ft=sh ts=4 sw=4 et
