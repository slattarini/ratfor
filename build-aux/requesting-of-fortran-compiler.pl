#!/usr/bin/env perl
# -*- perl -*-
#
# Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>.
# This file is part of the Ratfor Testsuite.
#
#------------------------------------------------------------------------
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
#------------------------------------------------------------------------
#
# This script implements a maintainer-check verifying that tests using
# the fortran compiler correctly check whether it's available.
#
#------------------------------------------------------------------------
#

require 5.008;

use strict;
use warnings FATAL => "all";

my ($rx, $me);

($me = $0) =~ s|.*/||;

$rx = 'rat4t_require_';
$rx = $rx . '((strong_)?fortran_compiler|helper_script\s.*_f77\.sh)';
$rx = qr/(^\s*|^\s*[^#].*)\b$rx\b/;

@ARGV or die "$me: Missing argument(s).\nUsage: $me FILES\n";

sub f77_missing_request($) {
    local *FH = shift;
    while (my $line = <FH>) {
        $line =~ $rx and return 0;
    }
    return 1;
}

sub f77_useless_request($) {
    local *FH = shift;
    while (my $line = <FH>) {
        $line =~ $rx and return 1;
    }
    return 0;
}

MAIN: {
    my (@useless_requests, @missing_requests);
    foreach (@ARGV) {
        /\.(test|sh)\z/ or die "$me: $_: invalid name\n";
        open(FILE, "<$_") or die "$me: $_: cannot open: $!\n";
        if (/_f77\.(test|sh)\z/) {
            push @missing_requests, $_ if f77_missing_request(\*FILE);
        } else {
            push @useless_requests, $_ if f77_useless_request(\*FILE);
        }
        close(FILE) or die "$me: $_: cannot close: $!\n";
    }
    my $msg = "";
    @missing_requests and $msg .= 
        "$me: Check failed for the following files:\n" .
        join("\n", map { " - $_" } @missing_requests) . "\n" .
        "They forget to require a fortran compiler, or to require" .
        " a proper `*_f77.sh` script.\n" .
        "You might want to add a call to the shell function" .
        " `rat4t_require_fortran_compiler` in them.\n" .
        "$me: Check failed.\n";
    $! = 1, die $msg if $msg;
}

# vim: ft=perl et sw=4 ts=4
