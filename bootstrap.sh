#!/bin/sh
# Bootstrap the Public Domain Ratfor package from checked-out sources.

GIT=${GIT-"git"}
AUTORECONF=${AUTORECONF-"autoreconf"}

set -e
set -x

pwd
$GIT submodule init
$GIT submodule update

cd blackbox-tests
pwd
./bootstrap.sh --no-autoreconf
cd ..

pwd
$AUTORECONF -vi

# vim: ft=sh ts=4 sw=4 et
