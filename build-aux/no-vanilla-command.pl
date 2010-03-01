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

my ($no_regexp, $use_instead, $description);
my @bad = ();

Getopt::Long::Configure(qw/gnu_getopt require_order no_ignore_case/);
GetOptions(
    "no-regexp=s" => \$no_regexp,
    "description=s" => \$description,
    "use-instead|instead-use=s" => \$use_instead
) && @ARGV && defined $use_instead && defined $no_regexp
or do {
    my $sp = ' 'x length($me);
    die <<EOF
Usage: $me --no-regexp=DEPRECATED-REGEXP --use-instead=SUGGESTED-COMMAND
       $sp [--description=BAD-REGEXP-DESCRIPTION] FILES
EOF
}; # semicolon required

$description ||= "regular expression `$no_regexp`";

COMPILE_REGEXP: {
    local $@;
    eval {
        $no_regexp = qr/$no_regexp/o;
        (my $dummy = "") =~ $no_regexp;
    }; # semicolon required
    if ($@) {
        $@ =~ s/ at \Q$0\E line \d+\.?\s*\z//;
        die "$me: Bad regular expression given.\n" . "$me: $@\n";
    }
}

foreach my $file (@ARGV) {
    open(FILE, "<$file") or die "$me: $file: cannot open: $!\n";
    while (<FILE>) {
        # remove trailing and leading spaces
        s/^\s*//; s/\s*$//;
        # assume lines starting with `#' are comments
        next if /^#/;
        push @bad, "$file:$.: $_" if $_ =~ $no_regexp;
    }
    close(FILE) or die "$me: $file: cannot close: $!\n";
}

if (@bad) {
    die "$me: Found $description in the lines below.\n" .
        "$me: Use $use_instead instead.\n" .
        " " . join("\n ", @bad) . "\n" .
        "$me: Check failed.\n";
}

# vim: ft=perl et sw=4 ts=4

