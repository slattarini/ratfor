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

# Helper file, sourced by scripts checking (through Fortran) that ratfor
# control statements (if, while, for, etc) are processed correctly when
# found after a numerical label.  This file is expected to be sourced by
# test scripts after the file `rat4-testsuite-init.sh' has already been
# sourced.

rat4t_require_fortran_compiler

set +x
echo "$me: INFO: disable shell verbosity while defining shell function"

cat >exp <<'EOF'
1
2
3
EOF

# Usage: write_test_file LOOP-OPENING-CODE LOOP-CLOSING-CODE
write_test_file() {
  cat > tst.r <<EOF
program test
    integer i
    i = 1
    goto 10
    write(*,100) i-1
10  $1
      write(*,100) i
    $2
20  write(*,100) i
30  call halt
100 format(I1)
end
EOF
}

testf77_control_structure_after_label() {

  write_test_file "$1" "$2"
  cat tst.r

  run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
  test ! -s stderr || testcase_FAIL "ratfor produced diagnostic on stderr"

  mv stdout tst.f
  mv stderr ratfor-stderr

  run_F77 tst.f || testcase_FAIL "ratfor produced uncompilable code"

  run_command ./tst.exe || testcase_FAIL "testprg failed"
  test ! -s stderr || testcase_FAIL "testprg produced diagnostic on stderr"

  mv stdout got

  cat exp
  cat got
  $DIFF_U exp got || testcase_FAIL "expected output and got output differs"

  testcase_DONE
}

echo "$me: INFO: reactivate shell verbosity before running tests"
set -x

# vim: ft=sh ts=4 sw=4 et
