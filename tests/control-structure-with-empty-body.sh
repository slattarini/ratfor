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
# control statements (if, while, for, etc) works correctly also with an
# empty body.  This file is expected to be sourced by test scripts after
# the file `rat4-testsuite-init.sh' has already been sourced.

test x${stmt+"set"} = x"set" \
  || testcase_HARDERROR "variable \`\$stmt' not set"

b="tst-$stmt"
# we shouldn't clobber exising files here, so try to tell the shell
# to abort on any attempt to do so (if this is supported)
(set -C) >/dev/null 2>&1 && { set -C || testcase_HARDERROR; }

case $stmt in
    # The double quoting here is *required* to avoid spurious parsing
    # errors from some shells (among them, Bash). Sigh.

    "if"|"while")
        echo "$stmt(1);"  > $b-1.r
        echo "$stmt(1){}" > $b-2.r
        ;;

    "do")
        (echo "do i=1,10"; echo ";")  > $b-1.r
        (echo "do i=1,10"; echo "{}") > $b-2.r
        echo "do i=1,10; ;"           > $b-3.r
        echo "do i=1,10 {}"           > $b-4.r
        echo "do i=1,10; {}"          > $b-5.r
        ;;

    "for")
        echo "for(x=1; x<2; x=x+1);"  > $b-1.r
        echo "for(x=1; x<2; x=x+1){}" > $b-2.r
        ;;

    "else")
        echo "if(1) pass; else;"   > $b-1.r
        echo "if(1) pass; else {}" > $b-2.r
        ;;

    "if-else")
        echo "if(1); else;"     > $b-1.r
        echo "if(1); else {}"   > $b-2.r
        echo "if(1) {} else;"   > $b-3.r
        echo "if(1) {} else {}" > $b-4.r
        ;;

    "repeat")
        echo "repeat;"   > $b-1.r
        echo "repeat {}" > $b-2.r
        ;;

    "repeat-until")
        echo "repeat; until(1)"   > $b-1.r
        echo "repeat {} until(1)" > $b-2.r
        ;;

    "switch")
        # `switch' statement require braces
        echo "switch(x) {}" > $b.r
        ;;

    "case"|"default")
        if x"$stmt" = x"case"; then
            s="case 1"
        else
            s="default"
        fi
        i=1
        for c1 in "" " " "$NEWLINE"; do
            for c2 in "" " " "$NEWLINE"; do
                echo "switch(x) { $s:$c1$c2}"   > $b-$i.r
                i=`expr $i + 1`
                echo "switch(x) { $s:$c1;$c2}"  > $b-$i.r
                i=`expr $i + 1`
                echo "switch(x) { $s:$c1{}$c2}" > $b-$i.r
                i=`expr $i + 1`
            done
        done
        ;;

    *)
        # CANTHAPPEN
        testcase_HARDERROR "invalid statement \`$stmt"
        ;;

esac

ls -l tst-*.r
for f in tst-*.r; do
    cat $f
    run_RATFOR $f || testcase_FAIL "unexpected ratfor failure"
    mv stdout $f--stdout
    if test -s stderr; then
        testcase_FAIL "ratfor produced diagnostic on stderr"
        mv stderr $f--stderr
    else
        rm -f stderr
    fi
done

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
