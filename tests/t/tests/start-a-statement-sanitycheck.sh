#!/bin/sh
# Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>.
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

# Do a sanity check on the ratfor interpreter, to check that is does not
# preprocess ratfor statements in a middle of another statement.
# Some tests require and use this feature to test other features, so they
# should be skipped if the feature in question is broken.  They can do so
# by including the present file.
# Also note that this file is expected to be sourced by test scripts only
# *after* the file `rat4-testsuite-init.sh' has already been sourced.

sc_flgs=$-

set +x
echo "$me: Sanity check: the test will be skipped if it fails"
sc_skip() { testcase_SKIP "sanity check failed: $* on input \`$sc_src'"; }
sc_src='x=1 repeat none'
case $sc_flgs in *x*) set -x;; esac

echo "$sc_src" >sanity.r
run_RATFOR sanity.r || sc_skip "ratfor fails"
test ! -s stderr || sc_skip "write diagnostics"
$SED '/^[cC]/d' stdout >sc.tmp
$FGREP "repeat" sc.tmp || sc_skip "expand \`repeat' statement"

set +x
echo "$me: Sanity check successfull"
rm -f sc.tmp stdout stderr || :
eval "
  unset sc_src || :
  unset sc_skip || :
  unset -f sc_skip || :
  unset sc_flgs || :
  case '$sc_flgs' in *x*) set -x;; esac
"

# vim: ft=sh ts=4 sw=4 et
