#!/bin/sh
#
# timer -- Run a command in the given time, abort it if times out.
#          Part of The Ratfor Testsuite
#
# Usage: timer [-t TIME] COMMAND [ARGS]
#
# Run the given COMMAND, until the given TIME (measured in seconds) is
# expired, then terminate it.
# COMMAND can be an absolute path, a relative path or a command name; in
# this last case, a PATH search is performed.
# If TIME is not specified, it defaults to 10 seconds.
# Return status of `timer' is 99 on usage errors, 127 if COMMAND was not
# found, 126 if COMMAND was not executable, 128+SIGNUM if COMAMND was
# terminated by signal number SIGNUM, 254 if COMMAND timed out, 255 on
# unexpected errors (e.g. failures in fork()), and the return status of
# COMMAND otherwise.
#

#--------------------------------------------------------------------------

##
##  Shell initializations and settings.
##

# Be more Bourne compatible
DUALCASE=1; export DUALCASE # for MKS sh
if test -n "${ZSH_VERSION+set}" && (emulate sh) >/dev/null 2>&1; then
    emulate sh
    NULLCMD=:
    # Pre-4.2 versions of Zsh do word splitting on ${1+"$@"}, which
    # is contrary to our usage.  Disable this feature.
    alias -g '${1+"$@"}'='"$@"'
    setopt NO_GLOB_SUBST
else
    case `(set -o) 2>/dev/null` in *posix*) set -o posix;; esac
fi

TAB='	'
NEWLINE='
'
IFS=" ${TAB}${NEWLINE}" # space tab newline

set -u  # make expanding uninitialized variable a fatal error
set -f  # disable file globbing

# output redirection don't clobber existing files (not portable)
(set -C) >/dev/null 2>&1 && set -C

##
##  Constants.
##

# These are AC_SUBST'd
SED='@SED@'
GREP='@GREP@'
EGREP='@EGREP@'
FGREP='@FGREP@'
SHELL='@SHELL@'

progname=`echo "$0" | $SED 's,.*/,,'`

GRACE_TIME=1

E_USAGE=99
E_NOTEXEC=126
E_NOTFOUND=127
E_TIMEOUT=254
E_UNEXPECT=255

PROCESS_SIGNALED_CORRECTLY=10
PROCESS_ALREADY_TERMINATED=20

##
##  Shell Functions.
##

print_usage() {
    echo "Usage: $progname [-t TIME] COMMAND [ARGS]"
}

warn() {
    echo "$progname: $*" >&2
}

fatal() {
    case $1 in
        [0-9]|[0-9][0-9]|[0-9][0-9][0-9]) e=$1; shift;;
        *) e=$E_UNEXPECT;;
    esac
    warn ${1+"$@"}
    exit $e
}

usage_error() {
    [ $# -gt 0 ] && warn ${1+"$@"}
    print_usage >&2
    exit $E_USAGE
}

# Usage: background COMMAND [ARGS]
# Run COMMAND in background with the given ARGS. COMMAND must be an
# external script or executable, it can't be a shell builtin or a
# shell function.
background() {
    # The `env' ensures that cmd is an external program, not a shell
    # function or shell builtin.
    env "$@" &
}

# Usage: xkill SIGNUM PID
# Return 1 if the process have been correctly signaled, and 2 if the
# process was already terminated
xkill() {
    sig=$1
    pid=$2
    s=`kill -$sig $pid 2>&1`
    case $?,$s in
        0,*)
            # Signal delivered correctly
            return $PROCESS_SIGNALED_CORRECTLY;;
        *,*[nN]o*[pP]rocess*)
            # Process already terminated.
            return $PROCESS_ALREADY_TERMINATED;;
    esac
    # Normalize error message.
    [ -n "${BASH_VERSION-}" ] && s=`echo X"$s" | $SED 's/^.*) - */:/'`
    s=`expr "X$s" : 'X.*: *\([^:]*\)'` 
    # We couldn't send signals to the process. This shouldn't
    # happen: abort.
    fatal "cannot send signal $sig to child process $pid: $s"
}

# Usage: is_pid_alive PID
is_pid_alive() {
    xkill 0 "$1"
    [ $? != $PROCESS_ALREADY_TERMINATED ]
}

# Usage: timeout
# Terminate the COMMAND running in the background, then exit with
# status 254.
timeout() {
    # Try to terminate the process gracefully with SIGTERM, so that it
    # can do cleanup if needed
    xkill 15 $cmd_pid
    if [ $? -eq $PROCESS_ALREADY_TERMINATED ]; then
        # Process already terminated by its own. Good!
        return 0
    fi
    # Give the process time to die, if it's still alive.
    sleep $GRACE_TIME
    if is_pid_alive $cmd_pid; then
        warn "process $cmd_pid didn't die with SIGTERM, killing it" \
             "with SIGKILL..."
        # First, give the process some more time to die, in case it has
        # catched the signal and is doing a longer cleanup before exiting.
        sleep $GRACE_TIME; sleep $GRACE_TIME
        # Go ahead and kill off the process.
        xkill 9 $cmd_pid
    fi
    fatal $E_TIMEOUT "command \`$cmd' timed out"
}

# Usage: start_timer TIMEOUT
start_timer() {
    t=$1
    trap 'timeout' 14
    background "$SHELL" -c "
        set -u
        ## so that we can stop timer
        trap '
            if test -n \"\${sleep_pid-}\"; then
                kill \$sleep_pid;
            fi;
            exit
        ' 15
        ## wait for timeout
        sleep $t & sleep_pid=\$!; wait
        ## signal timeout to parent
        kill -14 $$
    " "$SHELL"
    timer_pid=$!
}

# Usage: stop_timer
stop_timer() {
    trap "" 14
    xkill 15 $timer_pid
}

##
##  Option Parsing.
##

time=10  # default timeout (in seconds)

while [ $# -gt 0 ]; do
    case "$1" in
        -t) 
            if [ $# -ge 2 ]; then
                time=$2; shift;
            else
                usage_error "'-t': option requires an argument"
            fi
            ;;
        -t*)
            time=`expr "X$1" : 'X-t\(.*\)'`
            ;;
        --) 
            shift; break
            ;;
        -*) 
            usage_error "'$1': invalid option"
            ;;
        *) 
            break
            ;;
    esac
    shift
done

(test 0 -lt "$time") >/dev/null 2>&1 \
  || usage_error "'$time': invalid timeout"

##
##  Main code.
##

[ $# -gt 0 ] || usage_error "missing argument"
cmd="$1"
shift

start_timer $time

background "$cmd" ${1+"$@"}
cmd_pid=$!
wait $cmd_pid
exit_status=$?

stop_timer

exit $exit_status

# vim: ft=sh ts=4 sw=4 et
