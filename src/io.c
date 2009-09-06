#include "rat4-common.h"
#include "rat4-global.h"
#include "error.h"
#include "io.h"

/*
 *   I / O   W R A P P E R S
 */

#define BUFSIZE 2048 /* mostly arbitrary */

static char buf[BUFSIZE];   /* pushed-back chars */
static int bp = -1;         /* pushback buffer pointer */

/*
 * The two following subroutines are part of an hack needed to keep the
 * count of line numbers correct even when expansion multiline macros
 * (defined through the `define' builtin) is involved .
 */

static inline
void putc_(const char c)
{
    putchar((c == FKNEWLINE) ? NEWLINE : c);
}

static inline
void puts_(const char *s)
{
    while (*s != EOS)
        putc_(*s++);
}

/* get next char (either pushed back or new from the stream) */
#define ngetc_(fp) (bp >= 0 ? buf[bp--] : getc(fp))
    
/*
 * ngetch - get a (possibly pushed back) character, dealing with line
 *          continuation and keeping the count of line number.
 */
int
ngetch(FILE *fp)
{
    int c;
    
    /* check for a continuation '_\n' */
    while ((c = ngetc_(fp)) == UNDERLINE) {
        c = ngetc_(fp);
        if (!is_strict_newline(c)) {
            putbak(c);
            c = UNDERLINE;
            break;
        } else {
            ++linect[level];
        }
    }
    if (is_strict_newline(c)) {
        ++linect[level];
    }
    
    return(c);
}

#undef ngetc_

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
    if (++bp >= BUFSIZE)
        baderr("too many characters pushed back.");
    if (is_strict_newline(c))
        --linect[level];
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
    puts_( outbuf);
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
    /* we can't use `outch(c)' here, since we want to output the text
     * really verbatim, with no line folding (if the user want to output
     * raw fortran code, we expect him to know what he's doing, so we
     * must be as unobtrusive as possible) */
    char c;
    for (c = ngetch(fp); !is_newline(c); c = ngetch(fp))
        putc_(c);
    putc_(NEWLINE);
}

/*
 * outcmnt - copy comment to output
 *
 */
void
outcmnt(FILE * fp)
{
    char c;
    char comout[82];
    int comoutp = 0;

    comout[0] = 'c';
    comoutp = 1;
    for (c = ngetch(fp); !is_newline(c); c = ngetch(fp)) {
        if (comoutp > 79) {
            comout[80] = NEWLINE;
            comout[81] = EOS;
            puts_(comout);
            comout[0] = 'c';
            comoutp = 1;
        }
        comout[comoutp++] = (c == TAB ? BLANK : c);
    }
    comout[comoutp] = NEWLINE;
    comout[comoutp+1] = EOS;
    puts_(comout);
}

/* vim: set ft=c ts=4 sw=4 et : */
