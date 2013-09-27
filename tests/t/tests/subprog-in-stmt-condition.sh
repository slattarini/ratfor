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

# Helper file, sourced by scripts checking (through grep) that function
# opening is not recognized inside the "condition" or "(re)initialization"
# part of a ratfor statement -- examples:
#   while(CONDITION)
#   repeat STATEMENT until(CONDITION)
#   for(INITIALIZATION; CONDITION; REINITIALIZATION)
# This file is expected to be sourced by test scripts after
# the file `rat4-testsuite-init.sh' has already been sourced.

if echo 'return' | $RATFOR; then
  testcase_HARDERROR "ratfor does not die on \"return\" statement" \
                     "outside any function/subroutine"
fi

b="tst-${stmt-x}"
# we shouldn't clobber exising files here, so try to tell the shell
# to abort on any attempt to do so (if this is supported)
(set -C) >/dev/null 2>&1 && { set -C || testcase_HARDERROR; }

case ${subprog-} in
    "function"|"subroutine") ;;
    "") testcase_HARDERROR "variable \`\$subprog' not set or empty";;
    *) testcase_HARDERROR "invalid subprogram type \`$subprog";;
esac

case ${stmt-} in
    # The double quoting here is *required* to avoid spurious parsing
    # errors from some shells (among them, Bash). Sigh.
    "if"|"while")
        n=1
        echo "$stmt($subprog foo); return" > $b-1.r
        ;;
    "do")
        n=2
        echo "do $subprog foo; return" > $b-1.r
        (echo "do $subprog foo" && echo return) > $b-2.r
        ;;
    "for")
        n=3
        echo "for($subprog foo; pass; pass) return" > $b-1.r
        echo "for(pass; $subprog foo; pass) return" > $b-2.r
        echo "for(pass; pass; $subprog foo) return" > $b-3.r
        ;;
    "until")
        n=3
        (echo "repeat pass; until($subprog foo)" && echo return) > $b-1.r
        echo "repeat { pass } until($subprog foo); return" > $b-2.r
        echo "repeat { return } until($subprog foo)" > $b-3.r
        ;;
    "switch")
        n=2
        echo "switch($subprog) { default: return }" > $b-1.r
        echo "switch($subprog foo) { default: return }" > $b-2.r
        ;;
    "")
        testcase_HARDERROR "variable \`\$stmt' not set or empty"
        ;;
    *)
        testcase_HARDERROR "invalid statement \`$stmt"
        ;;
esac

ls -l $b*.r
for f in $b*.r; do
    x=`expr x"$f" : x'\(.*\)\.r'`
    cat $f
    run_RATFOR -e 1 $f || testcase_FAIL "ratfor didn't fail"
    mv stdout $x-stdout
    mv stderr $x-stderr
    test -s $x-stderr \
      || testcase_FAIL "ratfor didn't produce diagnostic on stderr"
    $GREP "return.*outside.*function or subroutine" $x-stderr \
      || testcase_FAIL "ratfor didn't produce expected diagnostic"
done

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
