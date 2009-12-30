#!/bin/sh
#
# Shell functions and setting for the Ratfor Testsuite test scripts.
# This file must be included after testlib.sh.
#

# Usage: must_be_directory DIRECTORY [DESC]
# Check that the given DIRECTORY exists, else die with an "hard error".
must_be_directory() {
    test -d "$1" || testcase_HARDERROR "${2-directory}" "\`$1' not found"
}

# Check that we have a valid fortran 77 compiler available, else
# cause the testcase to be SKIP'd.
require_fortran_compiler() {
    case "$F77" in
        "") testcase_HARDERROR "variable \`\$F77' is empty";;
      NONE) testcase_SKIP "Fotran 77 compiler should not be used" \
                          "(variable \`\$F77' is set to \`NONE')";;
    esac
}

# Check that we have available a valid fortran 77 compiler without silly
# limits, else cause the testcase to be SKIP'd.
require_strong_fortran_compiler() {
    require_fortran_compiler
    #XXX: move this checks at configure time?
    f77check_opwd=`pwd`
    f77check_ok=no
    mkdir f77check.d \
      && cd f77check.d \
      && $AWK 'BEGIN {
            print "      program testf77"
            for(i = 1; i <= 8192; i++)
                printf("%-5d i%d = 1\n", i, i)
            print "      stop"
            print "      end"
        }'> f77check.f </dev/null \
      && "$F77" -o f77check.exe f77check.f \
      && f77check_ok=yes
    cd "$f77check_opwd"
    rm_rf f77check.d
    if test x"$f77check_ok" = x"no"; then
        testcase_SKIP "Fotran 77 compiler \`$F77' is too limited"
    fi
}

# run_F77 [-e STATUS] [-m] [--] [ARGUMENTS..]
# -------------------------------------------
# Wrapper around `run_command $F77', to facilitate any present or future
# workaround for bugs/limitations in Fortran compilers.
run_F77() {
    # NOTE: all internal variables used here starts with the `run77'
    # prefix, to minimize possibility of name clashes with global
    # variables defined in user code.
    set +x # xtrace verbosity stops here
    run77_opts='' # options to be passed to run_command
    while test $# -gt 0; do
        case "$1" in
            -e) run77_opts="$run77_opts $1 $2"; shift;;
            -m) run77_opts="$run77_opts $1";;
            --) shift; break;;
            # Be more liberal about unrecognized options: stop parsing
            # and pass the rest of arguments and options verbatim to
            # COMMAND.  This makes the use of run_F77 more convenient.
            -*) break;;
             *) break;;
        esac
        shift
    done
    # file providing the custom `halt' procedure
    run77_haltf=$testaux_builddir/halt.f
    if test ! -f "$run77_haltf"; then
        testcase_HARDERROR "auxiliary source file \`$run77_haltf'" \
                           "not found"
    fi
    if test $# -eq 1; then
        run77_aout=`echo "$1" | $SED -e 's/\.f\(77\)\?$//'`.exe
        set x "$1" -o "$run77_aout"; shift;
    fi
    set -x # xtrace verbosity restart here
    run_command $run77_opts -- "$F77" "$run77_haltf" ${1+"$@"}
}

# run_RATFOR [-e STATUS] [-m] [-t TIMEOUT] [--] [ARGUMENTS..]
# -----------------------------------------------------------
# Wrapper around `run_command $RATFOR', to work around possible problems
# in the execution of the ratfor preprcessor.  In particular, we keep
# ratfor controlled by the `timer' script, so that even if it hangs, it
# won't cause the whole testsuite to hang (this happened too many times
# already).
run_RATFOR() {
    # NOTE: all internal variables used here starts with the `run4'
    # prefix, to minimize possibility of name clashes with global
    # variables defined in user code.
    set +x  # xtrace verbosity stops here
    run4_opts=''  # options to be passed to run_command
    run4_timeout=5  # the timeout given to ratfor
    while test $# -gt 0; do
        case "$1" in
            -e) run4_opts="$run4_opts $1 $2"; shift;;
            -m) run4_opts="$run4_opts $1";;
            -t) run4_timeout=$2; shift;;
            --) shift; break;;
            # Be more liberal about unrecognized options: stop parsing
            # and pass the rest of arguments and options verbatim to
            # COMMAND.  This makes the use of run_RATFOR more convenient.
            -*) break;;
             *) break;;
        esac
        shift
    done
    run4_timer=$testaux_builddir/timer  # the timer script
    if test ! -f "$run4_timer"; then
        testcase_HARDERROR "\`timer' script not found"
    elif test ! -x "$run4_timer"; then
        testcase_HARDERROR "\`timer' script not executable"
    fi
    set -x # xtrace verbosity restart here
    run_command $run4_opts -- "$run4_timer" -t "$run4_timeout" -- \
                "$RATFOR" ${1+"$@"}
}

# Print details on the current run of the testcase.
print_testrun_info() {
    echo "=== Running test $argv0"
    echo "# srcdir: $srcdir"
    echo "# builddir: $builddir"
    echo "# DATE: `date`"
    echo "# PWD: `pwd`"
    echo "# PATH: $PATH"
    echo "# F77: $F77"
    echo "# RATFOR: $RATFOR"
}

# Useful when grepping the path of the ratfor executable in error messages
# produced by rator.
ratfor_rx=`escape_for_grep "$RATFOR"`

# Testsuite utilities have always precedence over system ones.
must_be_directory "$testaux_srcdir"
PATH=${testaux_srcdir}${PATH+":$PATH"}
if test x"$testaux_srcdir" != x"$testaux_builddir"; then
    must_be_directory "$testaux_builddir"
    PATH=${testaux_builddir}${PATH+":$PATH"}
fi
export PATH

# vim: ft=sh ts=4 sw=4 et
