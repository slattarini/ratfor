/*
 *  timer -- Run a command in the given time, abort it if times out.
 *  Usage: timer [-t TIME] COMMAND [ARGS]
 *  Run the given COMMAND, until the given TIME (measured in seconds) is
 *  expired, then terminate it.
 *  COMMAND can be an absolute path, a relative path or a command name; in
 *  this last case, a PATH search is performed.
 *  If TIME is not specified, it defaults to 10 seconds.
 *  Return status of `timer' is 99 on usage errors, 127 if COMMAND was not
 *  found, 126 if COMMAND was not executable, 128+SIGNUM if COMAMND was
 *  terminated by signal number SIGNUM, 254 if COMMAND timed out, 255 on
 *  unexpected errors (e.g. failures in fork()), and the return status of
 *  COMMAND otherwise.
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/wait.h>
#include <errno.h>

#define GRACE_TIME  1

#define E_USAGE     99
#define E_NOTEXEC   126
#define E_NOTFOUND  127
#define E_SIGNAL(n) (128+(n))
#define E_TIMEOUT   254
#define E_UNEXPECT  255

struct exitinfo {
    unsigned short status;
    unsigned short signal;
};

static pid_t cpid; /* PID of child process */

static void 
fatal(int status, const char *msg,  ...)
{
    va_list ap;
    va_start(ap, msg);
    fflush(stdout);
    fprintf(stderr, "timer: ");
    vfprintf(stderr, msg, ap);
    fputs("\n", stderr);
    va_end(ap);
    exit(status);
}

static void
myexec(char *const argv[])
{
    const char *msg;
    int errstat;
    if (execvp(argv[0], argv) < 0) {
        if (errno == EACCES)
            msg = "permission denied", errstat = E_NOTEXEC;
        else
            msg = "command not found", errstat = E_NOTFOUND;
        fatal(errstat, "%s: cannot exec: %s", argv[0], msg);
    }
    /* CANTHAPPEN */
    fatal(E_UNEXPECT, "still alive after successful exec");
}

static struct exitinfo
handle_child()
{
    int wstatus;
    struct exitinfo exit_info;
    waitpid(cpid, &wstatus, 0);
    alarm(0); /* once the child has completed, cancel any pending alarm */
    if (WIFEXITED(wstatus)) {
        exit_info.status = WEXITSTATUS(wstatus);
        exit_info.signal = 0;
    } else if (WIFSIGNALED(wstatus)) {
        exit_info.status = -1;
        exit_info.signal = WTERMSIG(wstatus);
    } else {
        /* CANTHAPPEN */
        fatal(E_UNEXPECT, "bogus exit status from child");
    }
    return exit_info;
}

static int
xkill(int pid, int signum)
{
    if (kill(cpid, signum) == 0)
        return 0;
    
    switch(errno) {
    case EPERM:
        fatal(E_UNEXPECT, "cannot send signals to child process %d", cpid);
        break;
    case ESRCH:
        /* child already terminated, nothing to do */
        return 1;
        break;
    default:
        /* CANTHAPPEN */
        fatal(E_UNEXPECT, "unexpected error (errno %d) while signaling"
                          "child process %d", errno, cpid);
        break;
    }
}

#define is_pid_alive(pid) (!xkill(pid, 0))

static void
timeout(int signum)
{
    if (signum != SIGALRM)
        /* CANTHAPPEN */
        fatal(E_UNEXPECT, "unexpected signal passed to timeout():"
                          "signal %d", signum);
    /* try to make sure that alarm(2) and sleep(3) do not interfere */
    if (signal(SIGALRM, SIG_DFL) < 0)
        fatal(E_UNEXPECT, "couldn't shutdown timer: signal() failed");
    /* kill child if it's still alive */
    xkill(cpid, SIGTERM);
    sleep(GRACE_TIME);
    if (is_pid_alive(cpid)) {
        fprintf(stderr, "process %d didn't die with SIGTERM, killing it"
                        " with SIGKILL...\n", cpid);
        /* give child he time to terminate gracefully */
        sleep(2 * GRACE_TIME);
        xkill(cpid, SIGKILL);
    }
    fatal(E_TIMEOUT, "command timed out");
}

static void
setup_timer(unsigned int time)
{
    struct sigaction sa;
    sa.sa_handler = timeout;
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGALRM, &sa, NULL) < 0)
        fatal(E_UNEXPECT, "couldn't setup timer: sigaction() failed");
    if (alarm(time) != 0) /* there should be no previous alarm */
        fatal(E_UNEXPECT, "couldn't setup timer: previous alarm found");
}

int
main(int argc, char **argv)
{
    unsigned int timeout = 10;
    struct exitinfo exit_info;
    int c, had_posixly_correct = 0, errflag = 0;
    
    /* Sigh! By default, GNU getopt(3) is not POSIX-compatible, and it
       seems that the only way to turn on that compatiblity is setting
       and exporting the environment variable `POSIXLY_CORRECT'. */
    if (getenv("POSIXLY_CORRECT") != NULL)
        had_posixly_correct = 1;
    else
        setenv("POSIXLY_CORRECT", "1", 1); /* TODO: check for errors */
    while ((c = getopt(argc, argv, "t:")) != -1) {
        switch (c) {
        case 't':
            timeout = atoi(optarg);
            break;
        case '?':
            errflag++;
            break;
        default:
            /* CANTHAPPEN */
            abort();
            break;
        }
    }
    if (!had_posixly_correct)
        unsetenv("POSIXLY_CORRECT"); /* TODO: check for errors */
    if (!errflag && argc == optind) {
        fprintf(stderr, "%s: missing argument COMMAND\n", argv[0]);
        errflag++;
    }
    if (errflag) {
        fprintf(stderr, "Usage: timer [-t TIME] COMMAND [ARGS]\n");
        exit(E_USAGE);
    }
    
    setup_timer(timeout);
    
    cpid = fork();
    if (cpid < 0)
        /* fork failed */
        fatal(E_UNEXPECT, "fork failed");
    else if (cpid == 0)
        /* child process */
        myexec(&argv[optind]);
    else
        /* parent process */
        exit_info = handle_child();
    
    if (exit_info.signal)
        exit(E_SIGNAL(exit_info.signal));
    else
        exit(exit_info.status);
}

/* vim: set ft=c ts=4 sw=4 et: */
