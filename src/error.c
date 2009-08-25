/* $Id$ */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "error.h"
#include "ratdef.h"
#include "ratcom.h" /* this must go last */

/* Global program exit status, declared in error.h */
int exit_status = 0;

/*
 * E R R O R  M E S S A G E S
 */

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
    char lc[MAXCHARS];
    int i;

    fprintf(stderr, "error at line ");
    if (level >= 0)
        i = level;
    else
        i = 0;   /* for EOF errors */
    itoc(linect[i], lc, MAXCHARS);
    fprintf(stderr, lc);
    for (i = fnamp - 1; i > 1; i = i - 1)
        if (fnames[i-1] == EOS) {   /* print file name */
            fprintf(stderr, " in ");
            fprintf(stderr, &fnames[i]);
            break;
        }
    fprintf(stderr,": \n      %s\n", msg);
    exit_status = 1;
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
