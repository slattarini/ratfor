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

test x${stmt+"set"} = x"set" \
  || testcase_HARDERROR "variable \`\$stmt' not set"

# labels used in our test program
ilabel=10
fmt=100

cat >exp <<'EOF'
1
2
3
4
EOF

testprogname=`echo x"$stmt"| $SED -e 's/^x//' -e 's/[^a-zA-Z0-9]//g'`
cat > tst.r <<EOF
program $testprogname
    integer i
    i = 1
    goto $ilabel
    write(*,$fmt) 666
EOF

case $stmt in

# DO statement.
"do")
cat <<EOF
    $ilabel do i=1,3
        write(*,$fmt) i
EOF
;;

# BREAK keyword.
"break")
cat <<EOF
    while(i < 5) {
        $ilabel break
        write(*,$fmt) i
        i=i+1
    }
    write(*,$fmt) i; i=i+1
    write(*,$fmt) i; i=i+1
    write(*,$fmt) i; i=i+1
EOF
;;

# NEXT keyword.
"next")
cat <<EOF
    while(i < 4) {
        write(*,$fmt) i
        i=i+1
        $ilabel next
        write(*,$fmt) i
    }
EOF
;;

# FOR statement.
"for")
cat <<EOF
    $ilabel for(i = 1; i < 4; i = i + 1)
        write(*,$fmt) i
EOF
;;

# IF statement.
"if")
cat <<EOF
    $ilabel if(i < 4) {
        write(*,$fmt) i
        i=i+1
        goto $ilabel
    }
EOF
;;

# IF statement with an ELSE part.
"if-else")
cat <<EOF
    $ilabel if(i < 3) {
        write(*,$fmt) i
        i=i+1
        goto $ilabel
    } else {
        write(*,$fmt) i
        i=i+1
    }
EOF
;;

# REPEAT statement without UNTIL.
"repeat")
cat <<EOF
    $ilabel repeat {
        write(*,$fmt) i
        i=i+1
        if(i >= 4)
            break
    }
EOF
;;

# REPEAT/UNTIL statement.
"repeat-until")
cat <<EOF
    $ilabel repeat {
        write(*,$fmt) i
        i=i+1
    } until(i >=4)
EOF
;;

# SWITCH statement.
"switch")
cat <<EOF
    $ilabel switch(i) {
        case 1,2,3:
            write(*,$fmt) i
            i=i+1
            goto $ilabel
    }
EOF
;;

# WHILE statement.
"while")
cat <<EOF
    $ilabel while(i < 4) {
        write(*,$fmt) i
        i=i+1
    }
EOF
;;

# This shouldn't happen, really.
*) testcase_HARDERROR "invalid statement \`$stmt";;

esac >> tst.r

cat >> tst.r <<EOF
    write(*,$fmt) i
    call halt
    $fmt    format(I1)
end
EOF

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

# vim: ft=sh ts=4 sw=4 et
