#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "error.h"
#include "ratdef.h"
#include "ratcom.h" /* this must go last */

/*
 * E R R O R  M E S S A G E S
 */

/*
 * error - print error message with one parameter, then die
 */
void
error(S_CHAR *msg, S_CHAR *s)
{
    fprintf(stderr, msg, s);
    exit(1);
}

/*
 * synerr - report Ratfor syntax error
 */
void
synerr(S_CHAR *msg)
{
    S_CHAR lc[MAXCHARS];
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
}

/*
 *  baderr - print error message, then die
 */
void
baderr(S_CHAR *msg)
{
    synerr(msg);
    exit(1);
}
