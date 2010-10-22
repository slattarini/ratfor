#!/bin/sh
# Bootstrap the C Ratfor package from checked-out sources.

GIT=${GIT-"git"}
AUTORECONF=${AUTORECONF-"autoreconf"}

set -e
set -x

pwd
$GIT submodule init
$GIT submodule update

pwd
$AUTORECONF -vi

# vim: ft=sh ts=4 sw=4 et
