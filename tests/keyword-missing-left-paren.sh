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

# Helper file, sourced by scripts checking that ratfor errors out if
# no left parenthesis is seen after control statements requiring a
# parenthesized expression (at least if, while, for, switch and until).
# This file is expected to be sourced by test scripts after the file
# `rat4-testsuite-init.sh' has already been sourced and the `$stmt'
# variable properly defined.

test x${stmt+"set"} = x"set" \
  || testcase_HARDERROR "variable \`\$stmt' not set"

case $stmt in
    # The double quoting here is *required* to avoid spurious parsing
    # errors from some shells (among them, Bash).  Sigh.
    "for"|"while"|"if") echo "$stmt TOKEN { pass; }";;
    "switch") echo "switch TOKEN { case 1: pass; }";;
    "until") echo "repeat { pass; } until TOKEN";;
    *) testcase_HARDERROR "invalid statement \`$stmt";;
esac > template

i=0
for token in '' '1' 'foo' ';' 'a;b' '_' ':' '\&' '@'; do
  i=`expr $i + 1`
  sed "s/TOKEN/$token/" template | tr '@' "$NEWLINE" > tst-$i.r
  cat tst-$i.r
  run_RATFOR -e 1 tst-$i.r
  mv stdout stdout-$i
  mv stderr stderr-$i
  $GREP 'missing left paren' stderr-$i || testcase_FAIL
done

testcase_DONE

# vim: ft=sh ts=4 sw=4 et
