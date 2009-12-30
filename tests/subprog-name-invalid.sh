#!/bin/sh

# Define shell functions which can be used to check that ratfor complains
# if the keywords `function' or `subprograms' are not not followed by an
# alphanumeric token.  This is expected to be sourced by test scripts after
# the file `rat4-testsuite-init.sh' has already been sourced.

t_count=0

do_define() {
    case $2 in
        *'('|*')'*) echo "define $1 $2 ";;
        *) echo "define($1,$2)";;
    esac
}

do_prefix() {
    case ${2} in
        "") echo "$1";;
         *) echo "$2 $1";;
    esac
}

# Signature: write_testfile SUBPROG-TYPE [SUBPROG-NAME [PREFIX]]
write_testfile() {
    s=$1
    case "${2}" in
        ""|" "|";"|"$TAB") s="$s$2";;
        *) s="$s $2";;
    esac
    do_prefix "$s" "${3-}"
}

run_failing_ratfor() {
    t_count=`expr $t_count + 1`
    file=tst-$t_count.r
    "$@" >"$file"
    cat "$file"
    run_RATFOR -e 1 "$file" || testcase_FAIL
    stderr=stderr$t_count
    test ! -f "$stderr"
    test ! -r "$stderr"
    mv stderr "$stderr"
    rm -f stdout
}

check_subprog_bad_name() {
    subprog_type=$1
    name=$2
    prefix=${3-}
    run_failing_ratfor write_testfile "$subprog_type" "$name" "$prefix"
    if echo "$name" | $EGREP '^[0-9]+[A-Za-z_$]+$' >/dev/null; then
        # Some implementations are smarter in dealing with bad tokens
        # like `1a'
        nonalpha=`echo "$name" | $SED 's/[a-zA-Z_$]*$//'`
        $FGREP "invalid $subprog_type name \`$name'" "$stderr" \
          || $FGREP "invalid $subprog_type name \`$nonalpha'" "$stderr" \
            || testcase_FAIL
    else
        $FGREP "invalid $subprog_type name \`$name'" "$stderr" \
          || testcase_FAIL
    fi
}

check_subprog_missing_name() {
    subprog_type=$1
    stmt_terminator=$2
    prefix=${3-}
    run_failing_ratfor write_testfile "$subprog_type" "$stmt_terminator" \
                                      "$prefix"
    $FGREP "missing $subprog_type name" "$stderr" || testcase_FAIL
}

check_default_subprog_badnames() {
    for nm in \
        '1' '12' '123' '1a' '123a' '1abc' '123abc' \
        '@' '+' '-' '*' '.' ',' ':' '/' '=' '?' \
        '(' ')' '[' ']' '{' '}'
    do
        for prfx in '' ' ' "$TAB" 'fnord' 'x y z'; do
            check_subprog_bad_name "$1" "$nm" "$prfx"
        done
    done
}

check_problematic_subprog_badnames() {
    for nm in \
        '!' '^' '~' \
        '!=' '^=' '~=' '==' \
        '==' '>' '>=' '<' '<=' \
        '&' '&&' '|' '||'
    do
        for prfx in "" " " "$TAB" "fnord" "x y z"; do
            check_subprog_bad_name "$1" "$nm" "$prfx"
        done
    done
}

check_default_subprog_missnames() {
    for term in "" " " "$TAB" ";"; do
        for prfx in "" " " "$TAB" "fnord" "x y z"; do
            check_subprog_missing_name "$1" "$term" "$prfx"
        done
    done
}

# vim: ft=sh ts=4 sw=4 et
