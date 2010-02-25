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
# keywords (if, while, for, etc) are *not* processed when found in the
# middle of a statement. This file is expected to be sourced by test
# scripts after the file `rat4-testsuite-init.sh' has already been
# sourced, and also expect the variable `$stmt' to be defined correctly.

echo "x $stmt(1) { pass }" > tst.r

cat tst.r
run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
test ! -s stderr || testcase_FAIL "ratfor produced diagnostic on stderr"
$SED -e '/^[cC]/d' stdout > out
$FGREP "$stmt(1)" out \
  || testcase_FAIL "literal \"$stmt(1)\" not found in ratfor output"
$FGREP "goto" out \
  && testcase_FAIL "literal \"goto\" found in ratfor output"

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
