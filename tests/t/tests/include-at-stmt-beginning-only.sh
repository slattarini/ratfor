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
# is activated *only* at the beginning of a statement.
# This file is expected to be sourced by test scripts after the file
# `rat4-testsuite-init.sh' has already been sourced, and the variable
# `$quote_char' properly defined.

if test x${quote_char+"set"} != x"set"; then
    testcase_HARDERROR "shell variable \`\$quote_char' not set"
fi

echo notseen > inc.r

for p in a A 'foo=' 'foo =' 'foo; bar' @ + - : . , _ '&' '|' '=='; do
    for s in '' ' ' "$TAB" " $TAB$TAB "; do
        p1=$p p2=$p
        case $p in *[a-zA-Z0-9_]) test -z "$s" && p1="$p "; p2="$p ";; esac
        echo "${p1}${s}include 'inc.r'"
        echo "${s}${p2}include 'inc.r'"
        echo "${s}${p1}${s}include 'inc.r'"
    done
done | $SED -e "s/'/$quote_char/g" >tst.r

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

exp=`$FGREP -c include tst.r` || testcase_HARDERROR
got=`$FGREP -c include xstdout || test $? -eq 1` || testcase_HARDERROR
if test x"$got" != x"$exp"; then
    testcase_FAIL "bad count of strings \"include\" in output" \
                  "(expected = '$exp', got = '$got')"
fi

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
