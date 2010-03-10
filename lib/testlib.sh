#!/bin/sh
# Copied from SteLib at 2010-03-10 16:46:32 +0100.  DO NOT EDIT!
#
# -------------------------------------------------------------------------
#
# Shell library to write test cases.  The only available documentation are
# the comments and the description of variables/functions embedded in the
# code below.
#
# -------------------------------------------------------------------------
#
# Copyright (C) 2009, 2010 Stefano Lattarini.
# Written by Stefano Lattarini <stefano.lattarini@gmail.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# -------------------------------------------------------------------------
#

# Be more Bourne compatible.
# (Snippet inspired to configure's initialization in Autoconf 2.64)
DUALCASE=1; export DUALCASE # for MKS sh
if test -n "${ZSH_VERSION+set}" && (emulate sh) >/dev/null 2>&1; then
  emulate sh
  NULLCMD=:
  # Pre-4.2 versions of Zsh do word splitting on ${1+"$@"}, which
  # is contrary to our usage.  Disable this feature.
  alias -g '${1+"$@"}'='"$@"'
  setopt NO_GLOB_SUBST
  # Zsh, when not directly starting in Bourne-compatibility mode, has some
  # incompatibilities in the handling of `$0' which conflict with our usage
  # (e.g., `$0' inside a file sourced with the `.' builtin is temporarly
  # set to the name of the sourced file).  Work around this when possible,
  # otherwise abort the script.
  # Note that a bug in some versions of Zsh prevents us from resetting $0
  # in a sourced script, so the use of $argv0.  For more info see:
  #  <http://www.zsh.org/mla/workers/2009/msg01140.html>
  if [[ "$0" = *.'test' ]]; then
    # Good, FUNCTION_ARGZERO option was already off when this file was
    # sourced.  Thus we've nothing to do.
    argv0=$0
  elif test -n "${functrace[-1]}"; then
    # FUNCTION_ARGZERO option was on, but we have an easy workaround.
    argv0=${functrace[-1]%:*}
  else
    # Give up.
    echo "$0: (Zsh version $ZSH_VERSION) cannot determine the" \
         "path of running test script" >&2
    echo "$0:$LINENO: *** ABORT ***" >&2
    exit 99
  fi
else
  argv0=$0
  case `(set -o) 2>/dev/null` in *posix*) set -o posix;; esac
fi

(unset CDPATH) >/dev/null 2>&1 && unset CDPATH
(set -u) >/dev/null 2>&1 && set -u

# Make errors fatal.
set -e

# NLS nuisances: we want to run in plain C locale, for repeatibility
LC_ALL=C; export LC_ALL
LANGUAGE=C; export LANGUAGE
LANG=C; export LANG

# Testing tools.
GREP=${GREP-'grep'}
EGREP=${EGREP-'egrep'}
FGREP=${FGREP-'fgrep'}
SED=${SED-'sed'}
AWK=${AWK-'awk'}
SHELL=${SHELL-'/bin/sh'}
DIFF_U=${DIFF_U-'diff'}

# Symbolic names for exit statuses.
SUCCESS=0
FAILURE=1
E_SKIP=77 # test case skipped
E_HARD=99 # test case aborted due to some "hard error"

# Symbolic names for whitespace characters.
SPACE=' '
TAB='	'
NEWLINE='
'

# Some shorthands for common sed regexps and commands.
ws="[${SPACE}${TAB}]"
ws0p="${ws}*" # zero or more white spaces
ws1p="${ws}${ws0p}" # one or more white spaces

# In a test script, it is an error if any command reading from standard
# input does so without first redirecting it; so we want to close stdin
# to make such an error sticking out clearly.
# However, some shells give spurious errors when their stdin is closed
# (especially when dealing with here documents); in that case, just
# redirect the standard input from /dev/null, to be at least sure that
# script, when running attached to a tty, does not hang waiting input
# from the user.
if (exec <&- && cat <<EOF
foo
EOF
) >/dev/null 2>&1; then
    exec <&-
    stdin_is_closed=yes
else
    exec </dev/null
    stdin_is_closed=no
fi

# Name of the running test script.
me=`echo x/"$argv0" | $SED -e 's|.*/||' -e 's/\.test$//'`

# Find out srcdir, if not explicitly given. Always use an absolute path.
if test x${srcdir+"set"} != x"set"; then
   srcdir=`echo x"$argv0" | sed -e 's/^x//' -e 's,/[^\\/]*$,,'`
   test x"$srcdir" != x"$argv0" || srcdir=.
fi
case "$srcdir" in
    /*) : already an absolute path;;
     *) srcdir=`cd "$srcdir" && pwd` || exit 99;;
esac

# The builddir is alwys the directory in which the script is run.
builddir=`pwd` # absolute path

# Usage: _testcase_msg [-p PREFIX-TO-MESSAGE] MESSAGE
# Write a message on the standard error, with bells and whistles.
# For INTERNAL USE ONLY.
_testcase_msg() {
    testcase_msg_prefix="*** $me"
    # This function is expected to be run only when traces are turned off.
    case $- in *x*)
        set +x # otherwise we'll trigger an infinite loop!
        _testcase_msg -p "_testcase_msg" -p "INTERNAL ERROR" \
                      "called with shell tracing turned on"
        _Exit $E_HARD
        ;;
    esac
    while test $# -gt 0; do
        case "$1" in
            -p)
                testcase_msg_prefix="$testcase_msg_prefix: $2"
                shift
                ;;
            --)
                shift
                break
                ;;
            -*)
                _testcase_msg -p "_testcase_msg" -p "INTERNAL ERROR" \
                              "invalid switch \`$1'"
                _Exit $E_HARD
                ;;
             *)
                break
                ;;
        esac
        shift
    done
    test $# -eq 0 || echo "$testcase_msg_prefix: $*" >&2
}

# We use a trap for cleanup.  This requires us to go through hoops to
# get the right exit status transported through signals.  Also, more
# hoops are required since there are shells (even some modern version
# of Bash, e.g. bash-3.2 or bash-4.0) which do not passes a "failure"
# exit status to the exit trap when a syntax error is found.  Sigh.

# Wrapper around exit, needed to pass the correct exit status to the exit
# trap (see `_cleanup_at_exit()') below.  This function is for INTERNAL USE
# ONLY; test scripts should be terminated using the `testcase_*' functions
# defined below (e.g. `testcase_FATAL_FAILURE' or `testcase_DONE').
_Exit() {
    set +e
    set +x
    testcase_exit_default=$1
    (exit $1); exit $1
}

# Our exit trap: should deal with normal termination, signals, untrapped
# failures and internal errors.  This function is for INTERNAL USE ONLY.
_cleanup_at_exit() {
    testcase_exit_status=$1
    # in case the script is not terminated by `_Exit()' (e.g. an uncatched
    # failure while `set -e' is on, or a terminating signal).
    set +x
    if test -n "${testcase_exit_signal-}"; then
        _testcase_msg "caught signal $testcase_exit_signal"
        testcase_exit_status=$E_HARD
    elif test -z "${testcase_exit_default-}"; then
        _testcase_msg "internal error or untrapped failure"
        testcase_exit_status=$E_HARD
    elif test x"$testcase_exit_default" != x"$testcase_exit_status"; then
        _testcase_msg "exit status mismatch"
        testcase_exit_status=$E_HARD
    fi
    # wheter the temporary directory should be kept regardless of the
    # result of the test script
    case ${keep_testdirs-} in
        ""|0|n|N|no|NO) keep_testdirs=no;;
                     *) keep_testdirs=yes;;
    esac
    # do cleanup, if required
    if test $testcase_exit_status = 77; then
        _testcase_msg -p "CLEANUP" "skip: the test was skipped"
    elif test $testcase_exit_status != 0; then
        _testcase_msg -p "CLEANUP" "skip: the test failed"
    elif test x"$keep_testdirs" = x"yes"; then
        _testcase_msg -p "CLEANUP" "skip, as required by user" \
                                   "(keep_testdirs=yes)"
    else
        _testcase_msg -p "CLEANUP" "remove temporary files"
        set -x # temporarly re-enable traces (disabled in _Exit)
        rm_rf ${testSubDir+"$testSubDir"}
        # Apparently, the `set +x' with redirected stderr is executed in
        # a subshell by Solaris Sh, so repeat it without redirection.
        { set +x; } 2>/dev/null; set +x
        _testcase_msg -p "CLEANUP" "completed"
    fi
    # issue final message to the user, and exit
    _testcase_msg "EXIT $testcase_exit_status"
    exit $testcase_exit_status
}

# Usage: _testcase_process_outcome ACTION [MESSAGE]
# Process the outcome of a single test (there can be many in a test script).
# This function is FOR INTERNAL USE ONLY.
_testcase_process_outcome() {
    # TODO: check that xtrace verbosity is off
    # TODO: maybe we could even inline this function...
    set +x # xtrace verbosity temporarly disabled in this function
    if test $# -eq 0; then
        _testcase_msg \
            -p "_testcase_process_outcome" \
            -p "INTERNAL ERROR" \
            "missing ACTION"
        _Exit $E_HARD
    fi
    testcase_process_outcome_action=`expr x"$1" : 'x-*\(.*\)' | tr '_' '-'`
    testcase_process_outcome_raw_action=$1
    shift
    case $testcase_process_outcome_action in
        reset-success)
            _testcase_msg -p "RESET SUCCESS" ${1+"$@"}
            testcase_outcome=$SUCCESS
            ;;
        register-failure)
            _testcase_msg -p "FAIL" ${1+"$@"}
            testcase_outcome=$FAILURE
            ;;
        fatal-failure)
            _testcase_msg -p "FATAL FAILURE" ${1+"$@"}
            testcase_outcome=$FAILURE
            testcase_DONE
            ;;
        skip)
            _testcase_msg -p "SKIP" ${1+"$@"}
            testcase_outcome=$E_SKIP
            testcase_DONE
            ;;
        hard-error)
            _testcase_msg -p "HARD ERROR" ${1+"$@"}
            testcase_outcome=$E_HARD
            testcase_DONE
            ;;
        *)
            _testcase_msg \
                -p "_testcase_process_outcome" \
                -p "INTERNAL ERROR" \
                "invalid ACTION \`$testcase_process_outcome_raw_action'"
            _Exit $E_HARD
            ;;
    esac
    set -x # restart xtrace verbosity
}

# Usage: testcase_RESET_SUCCESS [WHY]
# Reset the outcome of the testscase to be "success", overriding any
# previously registered failure.
testcase_RESET_SUCCESS() {
    _testcase_process_outcome --reset-success ${1+"$@"}
}

# Usage: testcase_FAIL [WHY]
# Cause the current testcase to register "failure" as its outcome,
# explaining the cause of failure if WHY argument is given.
testcase_FAIL() {
    _testcase_process_outcome --register-failure ${1+"$@"}
}

# Usage: testcase_FATAL_FAILURE [WHY]
# Cause the current testcase to immeditely terminate with failure,
# explaining the cause of failure if WHY argument is given.
testcase_FATAL_FAILURE() {
    _testcase_process_outcome --fatal-failure ${1+"$@"}
}

# Usage: testcase_SKIP [WHY]
# Cause the current testcase to be SKIP'd, explaining why.
testcase_SKIP() {
    _testcase_process_outcome --skip ${1+"$@"}
}

# Usage: testcase_HARDERROR [WHY]
# Cause the current testcase to be aborted with an "hard error",
# explaining the cause of this error.
testcase_HARDERROR() {
    _testcase_process_outcome --hard-error ${1+"$@"}
}

# Usage: testcase_DONE [MESSAGE]
# Report the outcome of the testsuite (ans also the MESSAGE, if given),
# then cause the the script to exit with a proper exit status.
testcase_DONE() {
    set +x # xtrace verbosity temporarly disabled in this function
    _testcase_msg -p "TESTCASE DONE" ${1+"$@"}
    # a test script is successful by default
    testcase_outcome=${testcase_outcome-$SUCCESS}
    case $testcase_outcome in
        $SUCCESS)
            _testcase_msg "TESTCASE PASSED."
            ;;
        $FAILURE)
            _testcase_msg "TESTCASE FAILED."
            ;;
        $E_SKIP)
            _testcase_msg "TESTCASE SKIPPED."
            ;;
        $E_HARD)
            _testcase_msg "TESTCASE ERROR."
            ;;
        *)
            _testcase_msg -p "testcase_DONE" -p "INTERNAL ERROR" \
                          "invalid outcome \`$testcase_outcome'" >&2
            testcase_outcome=$E_HARD
            ;;
    esac
    set -x # restart xtrace verbosity
    _Exit $testcase_outcome
}

# -------------------------------------------------------------------------
# run_command: run a command with bells and whistles.
# -------------------------------------------------------------------------
# Usage:
#   either
#     run_command [-e STATUS] [-m] [--] COMMAND [ARGUMENTS..]
#   or:
#     run_command -c COMMAND [-e STATUS] [-m] [--]  [ARGUMENTS..]
# -------------------------------------------------------------------------
# Run the given COMMAND with ARGUMENTS; returns with failure (return status
# `1') if COMMAND does not exit with STATUS (more details are given below).
# Also, save standard output and standard error from COMMAND, by default
# respectively in files `stdout' and `stderr' (in the current directory),
# or together in the file `stdall' (in the current directory) if the `-m'
# option is given.
# Note that command must be an external command, not a shell function or
# shell builtin.
# If STATUS is numeric, the exit value of COMMAND must match it exactly.
# If STATUS is "RETURN", the exit value of COMMAND is returned by
# run_command.  If STATUS is "FAIL" or "FAILURE", then any exit value of
# COMMAND *but 0* is acceptable.  If STATUS is "IGNORE" or "IGNORED", any
# exit value of COMMAND is acceptable, and run_command returns with
# success regardless of that value.  Default STATUS is `0'.
run_command() {
    set +x # xtrace verbosity temporarly disabled in this function
    run_exitcode_expected=0
    run_mix_stdout_and_stderr=no
    while test $# -gt 0; do
        case $1 in
            -e) run_exitcode_expected=$2; shift;;
            -m) run_mix_stdout_and_stderr=yes;;
            --) shift; break;;
            -*) testcase_HARDERROR "run_commmand: invalid switch \`$1'";;
            *) break;;
        esac
        shift
    done
    case $# in
        0) testcase_HARDERROR "run_command: missing COMMAND argument";;
        *) run_cmd=$1; shift;;
    esac
    if test x"$run_mix_stdout_and_stderr" = x"yes"; then
        run_evald_cmd='"$run_cmd" ${1+"$@"} >stdall 2>&1'
    else
        run_evald_cmd='"$run_cmd" ${1+"$@"} >stdout 2>stderr'
    fi
    #
    # NOTE 1:
    #   We do not use the apparently simpler construct:
    #     if eval "$run_evald_cmd"; then
    #       run_exitcode_got=0
    #     else
    #       run_exitcode_got=$?
    #     fi
    #   as FreeBSD /bin/sh chokes on it when `set -e' is on.
    #
    # NOTE 2:
    #   We use the `env' command since some Korn Shells (e.g. pdksh)
    #   run shell builtins even with `exec'.
    if (eval "exec env $run_evald_cmd"); then
        run_exitcode_got=0
    else
        run_exitcode_got=$?
    fi
    if test x"$run_mix_stdout_and_stderr" = x"yes"; then
        set -x
        cat stdall
    else
        set -x
        cat stderr >&2
        cat stdout
    fi
    # Apparently, the `set +x' with redirected stderr is executed in
    # a subshell by Solaris Sh, so repeat it without redirection.
    { set +x; } 2>/dev/null; set +x
    run_return_status=no
    case $run_exitcode_expected in
        IGNORE|ignore|IGNORED|ignored|$run_exitcode_got)
            run_rc=0
            ;;
        FAIL|fail|FAILURE|failure)
            if test $run_exitcode_got -gt 0; then
                run_rc=0
            else
                run_rc=1
            fi
            ;;
        return|RETURN)
            run_rc=$run_exitcode_got
            run_return_status=yes
            ;;
        *)
            run_rc=1
            ;;
    esac
    if test x"$run_return_status" = x"yes"; then
        echo "run_command: returning exit status $run_exitcode_got"
    else
        echo "run_command: exit status $run_exitcode_got (expecting" \
             "$run_exitcode_expected)"
    fi
    set -x # restart xtrace verbosity
    return $run_rc
}

# "Safer" version of `rm -rf', useful for directories which are either
# unwritable or have unwritable subdirectories.
rm_rf() {
    test $# -gt 0 || return 0  # do nothing if no argument is given
    rm_rf__status=0
    for rm_rf__file in "$@"; do
        # the only interesting exit status is that of rm(1): ignore
        # errors in find(1)
        if test -d "$rm_rf__file"; then
            find "$rm_rf__file" -type d '!' -perm -200 \
                 -exec chmod u+w {} ";" || :
        fi
        rm -rf "$rm_rf__file" || rm_rf__status=1
    done
    return $rm_rf__status
}

# Strip leading and trailing white spaces from input.
ws_strip() {
    $SED -e "s/^$ws0p//" -e "s/$ws0p$//"
}

# Convert any sequence of tab and space chars to a single space char,
# and strip leading and trailing white spaces.
ws_normalize() {
    $SED -e "s/$ws1p/ /g" -e "s/^ *//" -e "s/ *$//"
}

# Escape a string so that it can be used literally in a grep search.
escape_for_grep() {
    case $# in
        0) cat;;
        *) echo "$*";;
    esac | $SED -e 's/\.\|\*\|\?\|\[\|\]\|\\\|\^\|\$/\\&/g'
}
# Escape a string so that it can be used literally in an egrep search.
escape_for_egrep() {
    case $# in
        0) cat;;
        *) echo "$*";;
    esac | {
      # from the GNU grep manpage:
      #  "Traditional  egrep  did  not  support  the { meta-character,
      #   and some egrep implementations support \{ instead, so portable
      #   scripts should avoid { in grep -E patterns and should use [{]
      # to match a literal `{'."
      $SED -e 's/\.\|\*\|\?\|+\|\[\|\]\|\\\|\^\|\$\|)\|(/\\&/g' \
           -e 's/{/[{]/g'
   }
}

# Setup cleanup traps.
trap '_cleanup_at_exit $?' 0
trap 'testcase_exit_signal=SIGHUP;  _Exit $E_HARD' 1
trap 'testcase_exit_signal=SIGINT;  _Exit $E_HARD' 2
trap 'testcase_exit_signal=SIGQUIT; _Exit $E_HARD' 3
trap 'testcase_exit_signal=SIGPIPE; _Exit $E_HARD' 13
trap 'testcase_exit_signal=SIGTERM; _Exit $E_HARD' 15

# Were the tests will run.
rm_rf ./"$me".dir
mkdir ./"$me".dir
cd ./"$me".dir
testSubDir=`pwd` || testcase_HARDERROR # absolute path

# vim: ft=sh ts=4 sw=4 et
