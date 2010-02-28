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

BEGIN { $^W = 1; }
use strict;
use warnings FATAL => "all";

use Getopt::Long;

(my $me = $0) =~ s|.*/||;

my ($vanilla, $cooked); # must be define from command line
my @bad = ();

my %options_hash = (
    "v|vanilla=s" => \$vanilla,
    "c|cooked=s" => \$cooked,
);
Getopt::Long::Configure(qw/gnu_getopt require_order no_ignore_case/);
GetOptions(%options_hash) && @ARGV && defined $vanilla && defined $cooked
  or die "Usage: $me -v VANILLA-CMD -c COOKED-CMD FILES\n";

foreach my $file (@ARGV) {
    open(FILE, "<$file") or die "$me: $file: cannot open: $!\n";
    while (<FILE>) {
        # remove trailing and leading spaces
        s/^\s*//; s/\s*$//;
        # assume lines starting with `#' are comments
        next if /^#/;
        push @bad, "$file:$.: $_" if (/\b\Q$vanilla\E\b/);
    }
    close(FILE) or die "$me: $file: cannot close: $!\n";
}

if (@bad) {
    die("$me: Found vanilla `$vanilla` command in the lines below.\n" .
        "$me: Use `$cooked` instead.\n" .
        " " . join("\n ", @bad) . "\n" .
        "$me: Check failed.\n");
}

# vim: ft=perl et sw=4 ts=4

