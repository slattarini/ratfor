#!/bin/bash
# Wrapper around gcc version 2.95.
#
# We need to use this wrapper because gcc 2.95 produces spurious warnings
# and thus abort compilation under `-Werror', even if nothing is wrong.
# This wrapper script takes care of this, by stripping `-Werror' from the
# command line arguments.

set -e; set -u;

CC=/usr/bin/gcc-2.95

vmsg() {
    ((quiet)) || echo "$0: $*"
}

vexec() {
    vmsg "++ $*"
    exec "$@"
}

declare -i werror_discarded=0
declare -i quiet=0
declare -a args=()
for arg in "$@"; do
    case "$arg" in
        -Werror) : $((werror_discarded++));;
        --quiet|--silent) quiet=1;;
        *) args=(${args+"${args[@]}"} "$arg");;
    esac
done

if (($werror_discarded > 0)); then
    vmsg "discarded $werror_discarded option(s) \`-Werror'"
fi

vexec "$CC" ${args+"${args[@]}"}

exit 255 #NOTREACHED

# vim: ft=sh ts=4 sw=4 et
