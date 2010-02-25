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
# sourced.  It also expect the variables $stmt, $stmt_arg, $stmt_post
# and $stmt_post_arg to be set correctly.

for v in stmt stmt_arg stmt_post stmt_post_arg; do
    if eval "test x\${$v+\"set\"} != x\"set\""; then
        testcase_HARDERROR "shell variable \`\$$v' not set"
    fi
done

echo "100 $stmt $stmt_arg pass; $stmt_post $stmt_post_arg" > tst.r

cat tst.r

run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
test ! -s stderr || testcase_FAIL "ratfor produced diagnostic on stderr"

$SED -e '/^[cC]/d' stdout > out
cat out

if test x"${fail_if_stmt_in_output-yes}" != x"no"; then
    $FGREP "$stmt" out \
      && testcase_FAIL "string \"$stmt\" found in ratfor output"
fi

$GREP "^100 .*pass" out \
  && testcase_FAIL "statement \"$stmt\" not expanded correctly"

if test x"${fail_if_stmt_post_in_output-no}" != x"no"; then
    $FGREP "$stmt_post" out \
      && testcase_FAIL "string \"$stmt_post\" found in ratfor output"
fi

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
