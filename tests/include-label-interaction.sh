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

# Helper file, sourced by scripts checking that the `include' builtin
# is *not* activated when found after a numerical label.

if test x${quote_char+"set"} != x"set"; then
    testcase_HARDERROR "shell variable \`\$quote_char' not set"
fi

echo notseen > inc.r

for l in  0 1 100 10000; do
    for s in '' ' ' "$TAB" " $TAB$TAB "; do
        test -z "$s" && l1="$l "; l2="$l "
        echo "${l1}${s}include 'inc.r'"
        echo "${s}${l2}include 'inc.r'"
        echo "${s}${l1}${s}include 'inc.r'"
    done
done | sed -e "s/'/$quote_char/g" >tst.r

cat inc.r
cat tst.r

run_RATFOR tst.r || testcase_FAIL "unexpected ratfor failure"
test ! -s stderr || testcase_FAIL "stderr not empty"
$SED '/^[cC]/d' stdout >xstdout

if $FGREP include xstdout; then :; else
    testcase_FAIL "string \"include\" not found in output"
fi

if $FGREP notseen xstdout; then
    testcase_FAIL "string \"notseen\" found in output"
fi

exp=`$FGREP -c include tst.r` || testcase_HADERRROR
got=`$FGREP -c include xstdout || test $? -eq 1` || testcase_HADERROR
if test x"$got" != x"$exp"; then
    testcase_FAIL "bad count of strings \"include\" in output" \
                  "(expected = '$exp', got = '$got')"
fi

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
