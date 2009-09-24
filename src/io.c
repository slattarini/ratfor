#include "rat4-common.h"
#include "rat4-global.h"
#include "error.h"
#include "io.h"

/*
 *   I / O   W R A P P E R S
 */

#define BUFSIZE 2048        /* mostly arbitrary */

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
#define NGETC_(fp) (bp >= 0 ? buf[bp--] : getc(fp))
    
BEGIN_C_DECLS

/*
 * ngetch - get a (possibly pushed back) character, dealing with line
 *          continuation and keeping the count of line number.
 */
int
ngetch(FILE *fp)
{
    int c;
    
    /* check for a continuation '_\n' */
    while ((c = NGETC_(fp)) == UNDERLINE) {
        c = NGETC_(fp);
        if (!is_strict_newline(c)) {
            put_back_char(c);
            c = UNDERLINE;
            break;
        } else {
            ++lineno[level];
        }
    }
    if (is_strict_newline(c)) {
        ++lineno[level];
    }
    
    return(c);
}

#undef NGETC_

/* put_back_string() - push string back onto input */
void
put_back_string(const char in[])
{
    int i;
    for (i = strlen(in) - 1; i >= 0; i--)
        put_back_char(in[i]);
}

/* put_back_char() - push character back onto input */
void
put_back_char(char c)
{
    if (++bp >= BUFSIZE)
        synerr_fatal("too many characters pushed back.");
    if (is_strict_newline(c))
        --lineno[level];
    buf[bp] = c;
}

END_C_DECLS

/*
 *  O U T P U T  F O R T R A N - C O M P A T I B L E  T E X T
 */

static char outbuf[82]; /*  output lines collected here    */
static int outp = 0;    /*  last position filled in outbuf */

BEGIN_C_DECLS

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
    puts_(outbuf);
    outp = 0;
}

/*
 * outnum - output decimal number
 *
 */
void 
outnum(int n)
{
#define MAXCHARS_ 10
    char chars[MAXCHARS_];
    int i, m;

    m = n > 0 ? (n) : (-n); /* abs value of n */
    i = -1;
    do {
        chars[++i] = (m % 10) + DIG0;
        m = m / 10;
    } while (m > 0 && i < MAXCHARS_);
    if (n < 0)
        outch(MINUS);
    for (; i >= 0; i--)
        outch(chars[i]);
#undef MAXCHARS_
}

/*
 * outasis - copy directly out
 *
 */
void
outasis(FILE * fp)
{
    /* we can't use `outch(c)' here, since we want to output the text
     * really verbatim, with no line wrapping (if the user want to output
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

    comout[0] = 'C';
    comoutp = 1;
    for (c = ngetch(fp); !is_newline(c); c = ngetch(fp)) {
        if (comoutp > 79) {
            comout[80] = NEWLINE;
            comout[81] = EOS;
            puts_(comout);
            comout[0] = 'C';
            comoutp = 1;
        }
        comout[comoutp++] = (c == TAB ? BLANK : c);
    }
    comout[comoutp] = NEWLINE;
    comout[comoutp+1] = EOS;
    puts_(comout);
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
