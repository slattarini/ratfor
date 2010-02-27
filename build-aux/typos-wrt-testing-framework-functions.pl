#!/usr/bin/env perl
# -*- perl -*-
#
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
#

require 5.008;

use strict;
use warnings FATAL => "all";
$^W = 1;

(my $me = $0) =~ s|.*/||;
@ARGV or die "$me: Missing argument(s).\nUsage: $me FILES\n";

my @valid = qw/FAIL SKIP HARDERROR DONE/;
my $rx_valid = "(" . join("|", @valid) . ")";
my @bad = ();
foreach my $file (@ARGV) {
    open(FILE, "<$file") or die "$me: $file: cannot open: $!\n";
    while (<FILE>) {
        my @x = grep { /^_?testcase_\w+$/ } (split /\b/);
        my @y = grep { !/^testcase_$rx_valid$/ } @x;
        push @bad, map { "$file:$.: $_" } @y;
    };
    close(FILE) or die "$me: $file: cannot close: $!\n";
};

if (@bad) {
    die("$me: Suspicious token(s) found in the following lines:\n" .
        " " . join(" \n", @bad) . "\n" .
        "$me: The only valid `testcase_xxx` functions are:\n" .
        join("\n", (map { " - testcase_$_" } @valid)) .
        "$me: Check failed.\n");
}

# vim: ft=perl et sw=4 ts=4

