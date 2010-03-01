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

(my $me = $0) =~ s|.*/||;

my %deprecated_vanilla_commands = (
    'sed'   => '$SED',
    'awk'   => '$AWK',
    'diff'  => '$DIFF_U',
    'grep'  => '$GREP',
    'fgrep' => '$FGREP',
    'egrep' => '$EGREP',
);

my %checks = map +(
    "vanilla_$_" => {
        regexp => qr/\b\Q$_\E\b/,
        description => "raw `$_` command",
        instead_use => "`$deprecated_vanilla_commands{$_}`",
    }
), keys %deprecated_vanilla_commands;

my @check_names = sort keys %checks;
my %bad_lines = map { $_ => [] } @check_names;
my @errors = ();

@ARGV or die "$me: Missing arguments\n";

FILE_LOOP:
foreach my $file (@ARGV) {
    unless(open FILE, "<$file") {
        push @errors, "$me: $file: cannot open: $!";
        next FILE_LOOP;
    }
    LINE_LOOP:
    while (<FILE>) {
        # remove trailing and leading spaces
        s/^\s*//; s/\s*$//;
        # assume lines starting with `#' are comments
        next if /^#/;
        foreach my $c (@check_names) {
            my $rx = $checks{$c}->{regexp};
            push @{$bad_lines{$c}}, "$file:$.: $_" if ($_ =~ $rx);
        }
    }
    close FILE or push @errors, "$me: $file: cannot close: $!";
}

foreach my $c (@check_names) {
    if (@{$bad_lines{$c}}) {
        my $h = $checks{$c};
        push @errors,
            "$c: In the lines below: found $h->{description}\n" .
            "$c: Instead, you should use: $h->{instead_use}\n" .
            " " . join("\n ", @{$bad_lines{$c}}) . "\n" .
            "$c: Check failed";
    }
}
die "\n" . join("\n\n", @errors) . "\n\n$me: FAILED\n" if @errors;

# vim: ft=perl et sw=4 ts=4

