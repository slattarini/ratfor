#!/bin/sh
#
# Shell functions and setting for the Ratfor Testsuite test scripts.
# This file must be included only by `rat4-testsuite-init.sh', and only
# after the inclusion of `rat4-testsuite-defs.sh' and `testlib.sh'.
#
# NAMESPACE CLEANLINESS:  We should try to keep ourselves confined to
# the namespaces `rat4t_*' and `_ra4t_*' as much as possible, for both
# variables and functions.  For environment variables, the namespace
# `RAT4_TESTSUITE_*' should be used instead.  Note that, however, we
# sometimes do some exceptions for the sake of simplicity (e.g. the
# functions `run_F77' and `run_RATFOR').
#

# Usage: rat4t_must_be_directory DIRECTORY [DESC]
# Check that the given DIRECTORY exists, else die with an "hard error".
rat4t_must_be_directory() {
    test -d "$1" || testcase_HARDERROR "${2-directory}" "\`$1' not found"
}

# Check that we have a valid fortran 77 compiler available, else
# cause the testcase to be SKIP'd.
rat4t_require_fortran_compiler() {
    case "$F77" in
        "") testcase_HARDERROR "variable \`\$F77' is empty";;
      NONE) testcase_SKIP "Fotran 77 compiler should not be used" \
                          "(variable \`\$F77' is set to \`NONE')";;
    esac
}

# Check that the given helper script is available (look it up in
# $srcdir if it's not an absolute path), and source it.
rat4t_require_helper_script() {
    case $1 in
           /*) rat4t_hs=$1;;
      ./*|../) rat4t_hs=`pwd`/$1;;
            *) rat4t_hs=$srcdir/$1;;
    esac
    shift # $1
    test -f "$rat4t_hs" && test -r "$rat4t_hs" \
      || testcase_HARDERROR "helper script \`$rat4t_hs' not found"
    . "$rat4t_hs" \
      || testcase_HARDERROR "failed to source helper script \`$rat4t_hs'"
}


# Check that we have available a valid fortran 77 compiler without silly
# limits, else cause the testcase to be SKIP'd.
rat4t_require_strong_fortran_compiler() {
    rat4t_require_fortran_compiler
    #XXX: move this checks at configure time?
    rat4t_f77check_opwd=`pwd`
    rat4t_f77check_ok=no
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
      && rat4t_f77check_ok=yes
    cd "$rat4t_f77check_opwd"
    rm_rf f77check.d
    if test x"$rat4t_f77check_ok" = x"no"; then
        testcase_SKIP "Fotran 77 compiler \`$F77' is too limited"
    fi
}

# run_F77 [-e STATUS] [-m] [--] [ARGUMENTS..]
# -------------------------------------------
# Wrapper around `run_command $F77', to facilitate any present or future
# workaround for bugs/limitations in Fortran compilers.
run_F77() {
    # NOTE: all internal variables used here starts with the `rat4t_runf77'
    # prefix, to minimize possibility of name clashes with global variables
    # defined in user code.
    set +x # xtrace verbosity stops here
    rat4t_runf77_opts='' # options to be passed to run_command
    while test $# -gt 0; do
        case "$1" in
            -e) rat4t_runf77_opts="$rat4t_runf77_opts $1 $2"; shift;;
            -m) rat4t_runf77_opts="$rat4t_runf77_opts $1";;
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
    rat4t_runf77_haltf=$rat4t_testaux_builddir/halt.f
    if test ! -f "$rat4t_runf77_haltf"; then
        testcase_HARDERROR "auxiliary source file \`$rat4t_runf77_haltf'" \
                           "not found"
    fi
    if test $# -eq 1; then
        rat4t_runf77_aout=`echo "$1" | $SED -e 's/\.f\(77\)\?$//'`.exe
        set x "$1" -o "$rat4t_runf77_aout"; shift;
    fi
    set -x # xtrace verbosity restart here
    run_command $rat4t_runf77_opts -- "$F77" "$rat4t_runf77_haltf" \
                ${1+"$@"}
}

# run_RATFOR [-e STATUS] [-m] [-t TIMEOUT] [--] [ARGUMENTS..]
# -----------------------------------------------------------
# Wrapper around `run_command $RATFOR', to work around possible problems
# in the execution of the ratfor preprcessor.  In particular, we keep
# ratfor controlled by the `timer' script, so that even if it hangs, it
# won't cause the whole testsuite to hang (this happened too many times
# already).
run_RATFOR() {
    # NOTE: all internal variables used here starts with the `rat4t_run4'
    # prefix, to minimize possibility of name clashes with global variables
    # defined in user code.
    set +x  # xtrace verbosity stops here
    rat4t_runratfor_opts=''  # options to be passed to run_command
    rat4t_runratfor_timeout=5  # the timeout given to ratfor
    while test $# -gt 0; do
        case "$1" in
            -e) rat4t_runratfor_opts="$rat4t_runratfor_opts $1 $2"; shift;;
            -m) rat4t_runratfor_opts="$rat4t_runratfor_opts $1";;
            -t) rat4t_runratfor_timeout=$2; shift;;
            --) shift; break;;
            # Be more liberal about unrecognized options: stop parsing
            # and pass the rest of arguments and options verbatim to
            # COMMAND.  This makes the use of run_RATFOR more convenient.
            -*) break;;
             *) break;;
        esac
        shift
    done
    rat4t_runratfor_timer=$rat4t_testaux_builddir/timer # the timer script
    if test ! -f "$rat4t_runratfor_timer"; then
        testcase_HARDERROR "\`timer' script not found"
    elif test ! -x "$rat4t_runratfor_timer"; then
        testcase_HARDERROR "\`timer' script not executable"
    fi
    set -x # xtrace verbosity restart here
    run_command $rat4t_runratfor_opts -- "$rat4t_runratfor_timer" \
                -t "$rat4t_runratfor_timeout" -- "$RATFOR" ${1+"$@"}
}

# Print details on the current run of the testcase.
rat4t_print_testrun_info() {
    echo "=== Running test $argv0"  # from testlib.sh
    echo "# srcdir: $srcdir"        # from testlib.sh
    echo "# builddir: $builddir"    # from testlib.sh
    echo "# DATE: `date`"
    echo "# PWD: `pwd`"
    echo "# PATH: $PATH"
    echo "# F77: $F77"
    echo "# RATFOR: $RATFOR"
}

# Useful when grepping the path of the ratfor executable in error messages
# produced by ratfor.
rat4t_ratfor_rx=`escape_for_grep "$RATFOR"`

# Testsuite utilities have always precedence over system ones.
rat4t_must_be_directory "$rat4t_testaux_srcdir"
PATH=${rat4t_testaux_srcdir}${PATH+":$PATH"}
if test x"$rat4t_testaux_srcdir" != x"$rat4t_testaux_builddir"; then
    rat4t_must_be_directory "$rat4t_testaux_builddir"
    PATH=${rat4t_testaux_builddir}${PATH+":$PATH"}
fi
export PATH

# vim: ft=sh ts=4 sw=4 et
