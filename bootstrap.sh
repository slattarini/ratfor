#!/bin/sh
# Bootstrap the Ratfor Testsuite package from checked-out sources.

USAGE="Usage: $0 [--autoreconf|--no-autoreconf]"

vrun() {
    echo "+ $*" >&2
    "$@"
}

run_autoreconf=true
while test $# -gt 0; do
    case "$1" in
        --help) echo "$USAGE"; exit;;
        --autoreconf) run_autoreconf=true;;
        --no-autoreconf) run_autoreconf=false;;
        *) echo "$0: \`$1': Invalid argument" >&2
           echo "$USAGE" >&2
           exit 2;;
    esac
    shift
done

set -e

AUTORECONF=${AUTORECONF-"autoreconf"}

f=tests/TestsList.am
vrun rm -f $f $f-t
vrun sh ./build-aux/gen-tests-list.sh tests >$f-t
vrun chmod a-w $f-t
vrun mv $f-t $f

if $run_autoreconf; then
    vrun $AUTORECONF -vi
else
    :
fi

# vim: ft=sh ts=4 sw=4 et
