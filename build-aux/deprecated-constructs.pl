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

# Wheter to fail laso on whitelisted failures.
my $ignore_whitelist = 0;

# The hash of static checks to be run on test scripts. Indexed by
# check name, it's values are proper hash references.
my %checks;

# Commands that should not be used directly, but only in the proper
# versions found at configure time and saved in shell variables.
SET_DEPRECATED_VANILLA_COMMAND: {
    my %commands = (
        'sed'   => '$SED',
        'awk'   => '$AWK',
        'diff'  => '$DIFF_U',
        'grep'  => '$GREP',
        'fgrep' => '$FGREP',
        'egrep' => '$EGREP',
    );
    while (my ($raw, $cooked) = each %commands) {
        $checks{"vanilla_$raw"} = {
            bad_match => qr/\b\Q$raw\E\b/,
            description => "raw `$raw` command",
            instead_use => "`$cooked`",
            must_skip => qr/^\s*#/, # comment lines
        };
    }
    $checks{'vanilla_sed'}{'whitelist'} = [
        'sanity-checks/run_RATFOR.test:30',
        'sanity-checks/run_RATFOR.test:31',
    ];
}

# Redirecting standard error to something != /dev/null might be
# inappropriate when shell traces are on, due to a bug in Zsh hadling
# of the xtrace flag.
$checks{'bad-stderr-redirect'} = {
    bad_match => sub {
        local $_ = shift;
        # Normalize format of redirections.
        s|(>+)\s*|$1|g;
        # For our purposes, there is no distinction between overriding
        # (`> out') and appending (`>> out') redirections.
        s|>>|>|g;
        # The construct `>/dev/null 2>&1' still redirects stderr to
        # /dev/null, so we can safely ignore it.
        s|>/dev/null\s+2>&1\b||g;
        # Look for bad/suspicious stderr redirections.
        return (m|\b2>(?!/dev/null\b)| ? 1 : 0);
    },
    description => "problematic stderr redirection",
    instead_use => "the `run_command` shell function",
    must_skip => qr/^\s*#/,
};

# Normalize checks, looking for errors.
NORMALIZE_CHECKS:
while (my ($n, $c) = each %checks) {
    defined $c->{bad_match} or die "$me: check $n: `bad_match' missing.\n";
    defined $c->{must_skip} or $c->{must_skip} = sub { return 0; };
    $c->{bad_lines} = [];
    foreach my $k (qw/bad_match must_skip/) {
        my $reftyp = ref($c->{$k}) || ""; # be sure it's defined
        if ($reftyp =~ /code/i) {
            1; # nothing to do
        } elsif ($reftyp =~ /regexp/i) {
            my $rx = $c->{$k}; # *required* for closure to work correctly
            $c->{$k} = sub { return (shift =~ $rx ? 1 : 0) };
        } elsif (not $reftyp) {
            die "$me: check $n: `$k': not a reference.\n";
        } else {
            die "$me: check $n: `$k': bad reference type: $reftyp.\n";
        }
    }
}

OPTION_PARSING: {
    while (@ARGV and $_ = shift(@ARGV)) {
        /^--$/ and last;
        /^--?ignore-whitelist$/ and $ignore_whitelist = 1, next;
        /^--?use-whitelist$/ and $ignore_whitelist = 0, next;
        /^-.*/ and die "$me: Invalid option: `$_'\n";
        unshift @ARGV, $_; last;
    }
    @ARGV or die "$me: Missing arguments\n";
}

my (@io_errors, @sc_errors);

FILE_LOOP:
foreach my $file (@ARGV) {
    unless(open FILE, "<$file") {
        push @io_errors, "$file: cannot open: $!";
        next FILE_LOOP;
    }
    LINE_LOOP:
    while (<FILE>) {
        chomp; # remove trailing newline
        CHECKS_LOOP:
        while (my ($n, $c) = each %checks) {
            next CHECKS_LOOP if $c->{must_skip}->($_, $file, $.);
            unless ($ignore_whitelist) {
                my $whitelist = $c->{whitelist} || [];
                next CHECKS_LOOP if grep /^$file(:\s*$.)?$/, @$whitelist;
            }
            if ($c->{bad_match}->($_, $file, $.)) {
                push @{$c->{bad_lines}}, "$file:$.: $_";
            }
        }
    }
    close FILE or push @io_errors, "$file: cannot close: $!";
}

BUILD_FAILURE_DETAILS:
while (my ($n, $c) = each %checks) {
    if (@{$c->{bad_lines}}) {
        my $msg = "";
        if (defined $c->{description}) {
            $msg .= "$n: In the lines below: found $c->{description}\n";
        } else {
            $msg .= "$n: Bad lines below\n";
        }
        if (defined $c->{instead_use}) {
            $msg .= "$n: You should use $c->{instead_use}\n";
        }
        $msg .= " " . join("\n ", @{$c->{bad_lines}}) . "\n";
        $msg .= "$n: Check failed, sorry.";
        push @sc_errors, $msg;
    }
}

REPORT: {
    my $msg = "";
    $msg .= "---\n" . join("\n---\n", @sc_errors) . "\n---\n"
        if @sc_errors;
    $msg .= join("\n", map {"I/O ERROR: $_"} @io_errors). "\n---\n"
        if @io_errors;
    die "$msg" . "$me: FAILED." if $msg;
}

# vim: ft=perl et sw=4 ts=4

