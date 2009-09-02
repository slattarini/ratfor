#include <config.h>

#include <stdio.h>  /* for getc() */
#include <string.h> /* for strlen() */

#include "ratdef.h"
#include "error.h"
#include "io.h"

/*
 *   I / O   W R A P P E R S
 */

#define BUFSIZE 512 /* mostly arbitrary */

static char buf[BUFSIZE];   /* pushed-back chars */
static int bp = -1;         /* pushback buffer pointer */

/* get next char (either pushed back or new from the stream) */
#define ngetch_(fp) (bp >= 0 ? buf[bp--] : getc(fp))
    
/*
 * ngetch - get a (possibly pushed back) character
 *
 */
int
ngetch(FILE *fp)
{
    int c;
    
    /* check for a continuation '_\n';  also removes UNDERLINES from
     * variable names */
    if ((c = ngetch_(fp)) == UNDERLINE) {
        c = ngetch_(fp);
        if (c != NEWLINE) {
            putbak(c);
            c = UNDERLINE;
        } else {
            c = ngetch_(fp);
        }
    }
    
    return(c);
}

#undef ngetch_

/*
 * pbstr - push string back onto input
 *
 */
void
pbstr(const char in[])
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
    if (++bp > BUFSIZE)
        baderr("too many characters pushed back.");
    buf[bp] = c;
}

/*
 *  O U T P U T  F O R T R A N - C O M P A T I B L E  T E X T
 */

static char outbuf[82]; /*  output lines collected here    */
static int outp = 0;    /*  last position filled in outbuf */

/*
 * outch - put one char into output buffer
 *
 */
void
outch(char c)
{
    int i;

    if (outp >= 72) {  /* continuation card */
        outdon();
        for (i = 0; i < 6; i++)
            outbuf[i] = BLANK;
        outbuf[5] = '*';
        outp = 6;
    }
    outbuf[outp++] = c;
}

/*
 * outstr - output string
 *
 */
void
outstr(const char str[])
{
    const char *s = str;
    while (*s != EOS)
        outch(*s++);
}

/*
 * outtab - get past column 6
 *
 */
void
outtab(void)
{
    while (outp < 6)
        outch(BLANK);
}

/*
 * outdon - finish off an output line
 *
 */
void
outdon(void)
{

    outbuf[outp] = NEWLINE;
    outbuf[outp+1] = EOS;
    printf("%s", outbuf);
    outp = 0;
}

/*
 * outnum - output decimal number
 *
 */
void 
outnum(int n)
{

    char chars[MAXCHARS];
    int i, m;

    m = n > 0 ? (n) : (-n); /* abs value of n */
    i = -1;
    do {
        chars[++i] = (m % 10) + DIG0;
        m = m / 10;
    } while (m > 0 && i < MAXCHARS);
    if (n < 0)
        outch(MINUS);
    for (; i >= 0; i--)
        outch(chars[i]);
}

/*
 * outasis - copy directly out
 *
 */
void
outasis(FILE * fp)
{
    char c;
    while ((c = ngetch(fp)) != NEWLINE)
        outch(c);
    outdon();
}

/*
 * outcmnt - copy comment to output
 *
 */
void
outcmnt(FILE * fp)
{
    char c;
    char comout[81];
    int i, comoutp = 0;

    comout[0] = 'c';
    comoutp = 1;
    while ((c = ngetch(fp)) != NEWLINE) {
        if (comoutp > 79) {
            comout[80] = NEWLINE;
            comout[81] = EOS;
            printf("%s", comout);
            comout[0] = 'c';
            comoutp = 1;
        }
        comout[comoutp++] = (c == TAB ? BLANK : c);
    }
    comout[comoutp] = NEWLINE;
    comout[comoutp+1] = EOS;
    printf("%s", comout);
}

/* vim: set ft=c ts=4 sw=4 et : */
