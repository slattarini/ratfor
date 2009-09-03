#include "rat4-common.h"

#include <stdio.h>
#include <stdlib.h>

#include "utils.h"
#include "error.h"
#include "rat4-global.h"

/* Global program exit status, declared in error.h */
int exit_status = 0;

/*
 * E R R O R  M E S S A G E S
 */

/*
 * synerr_ - interla function to report Ratfor syntax error
 */
static void
synerr_(int offset, char *msg)
{
    /* account for EOF errors, where level < 0 */
    int i = (level >= 0) ? level : 0;
    fprintf(stderr, "%s:%d: %s\n", filename[i], linect[i] + offset, msg);
    exit_status = 1;
}

/*
 * error - print error message with one parameter, then die
 */
void
error(char *msg, char *s)
{
    fprintf(stderr, msg, s);
    exit(1);
}

/*
 * synerr - report Ratfor syntax error, set global exit status to failure
 */
void
synerr(char *msg)
{
    synerr_(0, msg);
}

/*
 * synerr_eof() - report Ratfor syntax error about unexpected end-of-file,
 *                taking care of not having line number in error message
 *                off by one
 */
void
synerr_eof(void)
{
    synerr_(-1, "unexpected EOF.");
}


/*
 * baderr - report Ratfor syntax error, then die
 */
void
baderr(char *msg)
{
    synerr(msg);
    exit(1);
}

/* vim: set ft=c ts=4 sw=4 et : */
