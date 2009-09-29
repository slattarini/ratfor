#!/bin/sh
# Bootstrap the Public Domain Ratfor package from checked-out sources.

GIT=${GIT-"git"}
AUTORECONF=${AUTORECONF-"autoreconf"}

set -e
set -x

$GIT submodule init
$GIT submodule update

$AUTORECONF -vi

# vim: ft=sh ts=4 sw=4 et
