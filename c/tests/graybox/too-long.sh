#!/bin/sh
# Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>.
# This file is part of C Ratfor.
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

# Common code shared by tests checking that C Ratfor does not crash when
# finding in input "objects" too long (tokens, strings, comments, etc),
# but instead handles them correctly, or at least dies gracefully.
# This file must be included after the file `rat4-testsuite-init.sh' has
# been included, and the shell variable `$c' has been set properly.

: dont clutter the output with long irrelevant traces
set +x

# Assume that 2**13 = 8192 characters make up a comment too long
long_token=$c
i=1
while test $i -le 13; do
    i=`expr $i + 1`
    long_token="$long_token$long_token"
done
len_long_token=`echo $long_token | wc -c`
len_long_token=`expr $len_long_token - 1` # account for extra newline
if test x"$len_long_token" != x"8192"; then
    testcase_HARDERROR "len(\$long_token) = $len_long_token != 8192"
fi

set -x
: xtraces have been restored

# vim: ft=sh ts=4 sw=4 et
