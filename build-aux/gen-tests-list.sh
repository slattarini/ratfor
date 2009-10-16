#!/bin/sh
# Generate a makefile fragment holding the definition of the list
# of tests for this project (assumes that these tests are all and only
# the files `*.test' in the current directory'

set -e
cd ${1-.}
echo '## This file has been automatically generated.  DO NOT EDIT!'
echo 'ALL_TESTS = \'
for t in *.test; do
    echo "    $t \\"
done | sed '$s/ *\\$//'
echo

# vim: ft=sh ts=4 sw=4 et
