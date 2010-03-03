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
# sourced and the `$stmt' variable properly defined.

test x${stmt+"set"} = x"set" \
  || testcase_HARDERROR "variable \`\$stmt' not set"

label=100
out=out

check_expanded() {
    $GREP "^$label.*pass" $out \
      && testcase_FAIL "statement \"$stmt\" not expanded correctly"
}

check_not_in_output() {
    for s in ${1+"$@"}; do
        $FGREP "$s" $out \
          && testcase_FAIL "string \"$s\" found in ratfor output"
    done
}

case $stmt in
    
    # The double quoting here is *required* to avoid spurious parsing
    # errors from some shells (among them, Bash). Sigh.
    "break"|"next") echo "while(1) { $label $stmt; }"           ;;
    "if"|"while")   echo "$label $stmt(1) pass;"                ;;
    "if-else")      echo "$label if(1) pass; else pass;"        ;;
    "do")           echo "$label do i=1,2 { pass; }"            ;;
    "for")          echo "$label for (x=1; x<2; x=x+1) pass;"   ;;
    "repeat")       echo "$label repeat pass;"                  ;;
    "repeat-until") echo "$label repeat pass; until(1)"         ;;
    "switch")       echo "$label switch(1) { case 1: pass; }"   ;;
    
    *) testcase_HARDERROR "invalid statement \`$stmt";;

esac > tst.r

case $stmt in
    break|next|while|for|repeat|switch) bad_strings="$stmt";;
    repeat-until) bad_strings="repeat until";;
    *) bad_strings="";;
esac

cat tst.r
run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
test -s stderr && testcase_FAIL "ratfor produced diagnostic on stderr"

$SED -e '/^[cC]/d' stdout > $out
cat $out

check_expanded
check_not_in_output $bad_strings

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
