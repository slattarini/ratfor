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

# Helper file, sourced by scripts checking that the `include' works as
# expected when used at the beginning of a statement.
# This file is expected to be sourced by test scripts after the file
# `rat4-testsuite-init.sh' has already been sourced, and the variable
# `$quote_char' properly defined.

if test x${quote_char+"set"} != x"set"; then
    testcase_HARDERROR "shell variable \`\$quote_char' not set"
fi

: > tst.r
: > exp
i=0
for b in '' ';' 'pass;' '{'; do
    for w in '' ' ' "$TAB" " $TAB$TAB "; do
        i=`expr $i + 1`
        echo "${b}${w}include ${quote_char}inc${i}${quote_char}" >> tst.r
        if test x"$b" = x"{"; then echo '}' >> tst.r; fi
        echo "foo$i" >> exp
        echo "foo$i" > inc$i
    done
done

cat tst.r
run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
test ! -s stderr || testcase_FAIL "unexpected message(s) on stderr"

ws_normalize <stdout | $SED -e '/^[cC]/d' -e '/^$/d' -e '/^pass$/d' >got

$FGREP -i 'include' got && testcase_FAIL "raw include in ratfor output"

cat exp
cat got
$DIFF_U exp got || testcase_FAIL

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
