/* io.c -- this file is part of C Ratfor.
 * Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "rat4-common.h"
#include "rat4-global.h"
#include "error.h"
#include "io.h"


/*
 *   I / O   W R A P P E R S
 */

#define BUFSIZE 2048        /* mostly arbitrary */

static char buf[BUFSIZE];   /* pushed-back chars buffer */
static int bp = -1;         /* pushed-back chars buffer offset */

/* The three following subroutines are part of an hack needed to keep
 * the count of line numbers correct even when expansion multiline macros
 * (defined through the `define' builtin) is involved. */

static inline void
putc_(const char c)
{
    putchar((c == FKNEWLINE) ? NEWLINE : c);
}

static inline void
puts_(const char *s)
{
    while (*s != EOS)
        putc_(*s++);
}

static inline void
pbstr_(const char str[], const bool cooked)
{
    const char *s;
    for (s = str + strlen(str) - 1; s >= str; s--)
        if (!cooked)
            put_back_char(*s);
        else
            put_back_char(is_newline(*s) ? FKNEWLINE: *s);
}


/* Get next char (either pushed back or new from the stream). */
#define NGETC_(fp) (bp >= 0 ? buf[bp--] : getc(fp))

/* Get a (possibly pushed back) character, dealing with line continuation
 * and keeping the count of line number. */
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
            ++lineno[inclevel];
        }
    }
    if (is_strict_newline(c))
        ++lineno[inclevel];

    return(c);
}

#undef NGETC_

/* Push character back onto input. */
void
put_back_char(char c)
{
    if (++bp >= BUFSIZE)
        synerr_fatal("too many characters pushed back.");
    if (is_strict_newline(c))
        --lineno[inclevel];
    buf[bp] = c;
}

/* Push string back onto input. */
void
put_back_string(const char str[])
{
    pbstr_(str, false);
}

/* Push string back onto input, replacing newline characters with
 * FKNEWLINE characters */
void
put_back_string_cooked(const char str[])
{
    pbstr_(str, true);
}


/*
 *  O U T P U T  F O R T R A N - C O M P A T I B L E  T E X T
 */

static char outbuf[82]; /*  output lines collected here    */
static int outp = 0;    /*  last position filled in outbuf */

/* Put one char into output card, with automatic line wrapping and
 * continuation. */
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

/* Put a string into output card, with automatic line wrapping and
 * continuation. */
void
outstr(const char str[])
{
    const char *s = str;
    while (*s != EOS)
        outch(*s++);
}

/* Get past column 6 in output card. */
void
outtab(void)
{
    while (outp < 6)
        outch(BLANK);
}

/* Finish off an output line. */
void
outdon(void)
{
    outbuf[outp] = NEWLINE;
    outbuf[outp+1] = EOS;
    puts_(outbuf);
    outp = 0;
}

/* Write a decimal number in output card. */
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

/* Copy directly to output until the next newline character.  No line
 * wrapping, no line continuation, no sanitazation: just a real verbatim
 * copy. */
void
output_verbatim(FILE * fp)
{
    /* We can't use `outch(c)' here, since we want to output the text
     * really verbatim, with no line wrapping (if the user want to output
     * raw fortran code, we expect him to know what he's doing, so we
     * must be as unobtrusive as possible). */
    char c;
    for (c = ngetch(fp); !is_newline(c); c = ngetch(fp))
        putc_(c);
    putc_(NEWLINE);
}

/* Copy comment to output card, dealing with line wrapping if necessary. */
void
output_comment(FILE * fp)
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

/* vim: set ft=c ts=4 sw=4 et : */
