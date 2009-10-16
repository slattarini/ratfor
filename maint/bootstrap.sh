#!/bin/sh
# Bootstrap the Ratfor Testsuite package from checked-out sources.

set -x
set -e

AUTORECONF=${AUTORECONF-"autoreconf"}

f=tests/TestsList.am
rm -f $f $f-t
sh ./build-aux/gen-tests-list.sh tests >$f-t
chmod a-w $f-t
mv $f-t $f

$AUTORECONF -vi

# vim: ft=sh ts=4 sw=4 et
