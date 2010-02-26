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

# Helper file, sourced by scripts checking (through f77) that ratfor
# keywords (if, while, for, etc) are *not* processed when
# found in the middle of a statement. This file is expected to be sourced
# by test scripts after the file `rat4-testsuite-init.sh' has already been
# sourced, and also expect the variable `$stmt' to be defined correctly.

rat4t_require_fortran_compiler

set +x
echo "$me: INFO: disable shell verbosity while defining shell function"

testf77_keyword_in_statement_middle() {

    stmt=$1
    shift

    cat >tst.r <<EOF
subroutine $stmt(x)
    integer x
    write(*,100) x; 100 format(I1)
end
program test$stmt
    call $stmt(1)
end
EOF
    cat tst.r

    run_RATFOR tst.r \
      || testcase_FAIL "unexpected ratfor failure"
    test ! -s stderr \
      || testcase_FAIL "ratfor produced diagnostic on stderr"

    mv stdout tst.f && mv stderr ratfor-stderr \
      || testcase_HARDERROR

    run_F77 tst.f \
      || testcase_FAIL "unexpected fortran failure"

    run_command ./tst.exe \
      || testcase_FAIL "unexpected test-program failure"
    test ! -s stderr \
      || testcase_FAIL "test-program produced diagnostic on stderr"
    test x"`cat stdout`" = x"1" \
      || testcase_FAIL "bad test-program output"

    testcase_DONE
}

echo "$me: INFO: reactivate shell verbosity before running tests"
set -x

# vim: ft=sh ts=4 sw=4 et
