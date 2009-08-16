#include <stdio.h>  /* for getc() */
#include <string.h> /* for strlen() */

#include "ratdef.h"
#include "error.h"
#include "io.h"

/*
 *   I / O   W R A P P E R S
 */

#define BUFSIZE 512

static char buf[BUFSIZE];  /*  pushed-back chars */
static int bp = -1;  /* pushback buffer pointer */

/*
 * ngetch - get a (possibly pushed back) character
 *
 */
int
ngetch(char *c, FILE *fd)
{
    if (bp >= 0) {
        *c = buf[bp];
        bp--;
    } else {
        *c = getc(fd);
    }

   /* check for a continuation '_\n';  also removes UNDERLINES from
    * variable names */
    while (*c == UNDERLINE) {
        if (bp >= 0) {
            *c = buf[bp];
            bp--;
        } else {
            *c = getc(fd);
        }
        if (*c != NEWLINE) {
            putbak(*c);
            *c = UNDERLINE;
            break;
        } else {
            while(*c == NEWLINE) {
                if (bp >= 0) {
                    *c = buf[bp];
                    bp--;
                } else {
                    *c = (char) getc(fd);
                }
            }
        }
    }

    return(*c);
}

/*
 * pbstr - push string back onto input
 *
 */
void
pbstr(char in[])
{
    int i;
    for (i = strlen(in) - 1; i >= 0; i--)
        putbak(in[i]);
}

/*
 * putbak - push char back onto input
 *
 */
void
putbak(char c)
{
    bp++;
    if (bp > BUFSIZE)
        baderr("too many characters pushed back.");
    buf[bp] = c;
}
