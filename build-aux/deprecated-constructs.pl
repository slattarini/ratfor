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
            run_check => qr/\b\Q$raw\E\b/,
            description => "raw `$raw` command",
            instead_use => "`$cooked`",
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
    run_check => sub {
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
};

my @check_names = sort keys %checks;
my %bad_lines = map { $_ => [] } @check_names;
my @errors = ();

# Normalize checks, looking for errors.
foreach my $c (@check_names) {
    die "$me: check $c: `run_check' undefined.\n"
      unless defined $checks{$c}->{run_check};
    my $x = $checks{$c}->{run_check};
    my $r = ref $x || ""; # be sure it's defined
    if ($r =~ /code/i) {
        1; # nothing to do
    } elsif ($r =~ /regexp/i) {
        $checks{$c}->{run_check} = sub { return (shift =~ $x ? 1 : 0) };
    } elsif (not $r) {
        die "$me: check $c: `run_check': not a reference.\n";
    } else {
        die "$me: check $c: `run_check': bad reference type: $r.\n";
    }
}

OPTION_PARSING:
while (@ARGV and $_ = shift(@ARGV)) {
    /^--$/ and last;
    /^--?ignore-whitelist$/ and $ignore_whitelist = 1, next;
    /^--?use-whitelist$/ and $ignore_whitelist = 0, next;
    /^-.*/ and die "$me: Invalid option: `$_'\n";
    unshift @ARGV, $_; last;
}

@ARGV or die "$me: Missing arguments\n";

FILE_LOOP:
foreach my $file (@ARGV) {
    unless(open FILE, "<$file") {
        push @errors, "$me: $file: cannot open: $!";
        next FILE_LOOP;
    }
    LINE_LOOP:
    while (<FILE>) {
        # remove trailing newline
        chomp;
        # assume lines starting with `#' are comments
        next if /^#/;
        foreach my $c (@check_names) {
            unless ($ignore_whitelist) {
                my $whitelist = $checks{$c}->{whitelist} || [];
                next if grep { /^$file(:\s*$.)?$/ } @$whitelist;
            }
            next unless &{$checks{$c}->{run_check}}($_);
            push @{$bad_lines{$c}}, "$file:$.: $_"
        }
    }
    close FILE or push @errors, "$me: $file: cannot close: $!";
}

foreach my $c (@check_names) {
    if (@{$bad_lines{$c}}) {
        my $h = $checks{$c};
        my $msg = "";
        if (defined $h->{description}) {
            $msg .= "$c: In the lines below: found $h->{description}\n";
        } else {
            $msg .= "$c: Bad lines below\n";
        }
        if (defined $h->{instead_use}) {
            $msg .= "$c: Instead, you should use: $h->{instead_use}\n";
        }
        $msg .= " " . join("\n ", @{$bad_lines{$c}}) . "\n";
        $msg .= "$c: Check failed, sorry.";
        push @errors, $msg;
    }
}
die "\n" . join("\n\n", @errors) . "\n\n$me: FAILED\n" if @errors;

# vim: ft=perl et sw=4 ts=4

