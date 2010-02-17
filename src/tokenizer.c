/* tokenizer.c -- this file is part of C Ratfor.
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

/* Get tokens for the ratfor preprocessor. */

#include "rat4-common.h"

#include "tokenizer.h"
#include "io.h"
#include "xopen.h"
#include "error.h"
#include "define.h" /*XXX*/
#include "hash.h" /*XXX*/
#include "rat4-global.h"

#define MAXPATH  1024  /* max length of the name of a file included */

/*
 * PRIVATE FUNCTIONS.
 */

/* Like `!is_rat4_alnum(c)', but can be assigned to a function pointer. */
static bool
is_not_rat4_alnum(int c)
{
    return(is_rat4_alnum(c) ? false : true);
}

/* Convert integer n to a string, up to bufsiz - 1 characters, and save it
 * in buffer buf[].  Negative integers are supported.  Return the lenght
 * of the converted string.
 */
static int
integer_to_string(int n, char buf[], int bufsiz)
{
    int i, j, k, sign;
    char c;

    sign = (n < 0 ? -1 : 1);
    n *= sign; /* turn n into its absolute value */
    i = 0;
    do {
        buf[i++] = n % 10 + DIG0;
    } while ((n /= 10) > 0 && i < bufsiz - 2);
    if (sign < 0 && i < bufsiz - 1)
        buf[i++] = '-';
    buf[i] = EOS;
    /* reverse the string and plug it back in */
    for (j = 0, k = SSTRLEN(buf) - 1; j < k; j++, k--) {
        c = buf[j];
        buf[j] = buf[k];
        buf[k] = c;
    }
    return(i - 1);
}

/* Read characters from input stream `fp' and save them in buf[] array,
 * until a character satisying the `must_stop' boolean function is found.
 * Die with the given error message if writing in buf[] leads to an
 * overflow.
 */
static void
read_until_char_match(char buf[], int bufsiz, FILE *fp,
                      bool (*must_stop) (int),
                      const char *error_message_for_overflow)
{
    int i;
    for (i = 0; i < bufsiz - 1; i++) {
        buf[i] = ngetch(fp);
        if (must_stop(buf[i])) {
            put_back_char(buf[i--]);
            break;
        }
    }
    /* the string must be properly terminated by a NUL character */
    if (i >= bufsiz - 1) {
        synerr(error_message_for_overflow);
        buf[i] = EOS;
    } else {
        buf[++i] = EOS;
    }
}

/* Look-up definition of name[] in user-defined macros. If it's not found,
 * return false, else and copy the definition in defn[] and return true. */
static bool
defn_lookup(const char name[], char defn[])
{
    struct hashlist *p;
    if ((p = hash_lookup(name)) == NULL)
        return(false);
    strcpy(defn, p->def); /* XXX potential overflow here! */
    return(true);
}

static void
dispatch_comment(FILE *fp)
{
    int i;
    if (keep_comments)
        outcmnt(fp); /* copy comments to output */
    else
        for (i = ngetch(fp); !is_newline(i); i = ngetch(fp))
            /* strip comments */;
}

/* Convert relational shorthands (e.g. `&&' --> `.and.', '<' --> '.lt.'),
 * write the resulting string in buf[] itslef.  Return the lenght of
 * the resulting string. */
static int
convert_relational_shortand(char buf[], int bufsiz, FILE *fp)
{
#ifdef NDEBUG
    /* TODO: assert bufsiz >= 6 */
#else
    /* pacify compiler warnings */
    (void) bufsiz;
#endif
    buf[0] = ngetch(fp);
    if ((buf[1] = ngetch(fp)) == EQUALS) {
        buf[2] = 'e';
    } else {
        put_back_char(buf[1]);
        buf[2] = 't';
    }
    buf[3] = PERIOD;
    buf[4] = EOS;
    buf[5] = EOS; /* for .not. and .and. */

    switch(buf[0]) {
        case GREATER:
            buf[1] = 'g';
            break;
        case LESS:
            buf[1] = 'l';
            break;
        case NOT:
        case BANG:
        case CARET:
            if (buf[1] != EQUALS) {
                buf[2] = 'o';
                buf[3] = 't';
                buf[4] = PERIOD;
            }
            buf[1] = 'n';
            break;
        case EQUALS:
            if (buf[1] != EQUALS) { /* variable assignement */
                buf[1] = EOS;
                return(EQUALS); /* buf `=' isn't a relational shortand */
            }
            buf[1] = 'e';
            buf[2] = 'q';
            break;
        case AND:
            if ((buf[1] = ngetch(fp)) != AND) /* look for && or & */
                put_back_char(buf[1]);
            buf[1] = 'a';
            buf[2] = 'n';
            buf[3] = 'd';
            buf[4] = PERIOD;
            break;
        case OR:
            if ((buf[1] = ngetch(fp)) != OR) /* look for || or | */
                put_back_char(buf[1]);
            buf[1] = 'o';
            buf[2] = 'r';
            break;
        default: /* can't happen */
            abort();
            break;
    }  /* switch(buf[0]) */
    buf[0] = PERIOD;
    return(TOKT_RELATN);
}

/* Read alphanumeric token from input stream fp, save it in buf[], and
 * return the type of the token read (always TOKT_ALPHA). */
static int
get_alphanumeric_raw_token(char buf[], int bufsiz, FILE *fp)
{
    read_until_char_match(buf, bufsiz, fp, is_not_rat4_alnum,
                          "alphanumeric token too long.");
    return(TOKT_ALPHA);
}

/* Read numerical token from input stream fp, save it in buf[], and
 * return the type of the token read (always TOKT_DIGITS). */
static int
get_numerical_raw_token(char buf[], int bufsiz, FILE *fp)
{
    int i, c, c2, b, n;
    b = 0; /* in case alternate base number */
    for (i = 0; i < bufsiz - 1; i++) {
        buf[i] = ngetch(fp);
        if (!is_digit(buf[i]))
            break;
        b = 10 * b + (buf[i] - DIG0);
    }
    if (buf[i] == RADIX && b >= 2 && b <= 36) {
        /* n%ddd... */
        for (n = 0; ; n = b * n + c) {
            c = c2 = ngetch(fp);
            if (is_lower(c))
                c = c - 'a' + DIG9 + 1;
            else if (is_upper(c))
                c = c - 'A' + DIG9 + 1;
            if (c < DIG0 || c >= DIG0 + b)
                break;
            c = c - DIG0;
        }
        put_back_char(c2);
        i = integer_to_string(n, buf, bufsiz);
    } else {
        put_back_char(buf[i--]);
    }
    if (i >= bufsiz - 1) {
        synerr("numeric constant too long.");
        buf[i] = EOS;
    } else {
        buf[++i] = EOS;
    }
    buf[i] = EOS;
    return(TOKT_DIGITS);
}

/* Read "quoted string" token from input stream fp, save it in buf[],
 * return the type of the token read (always `TOKT_STRING'). */
static int
get_quoted_string_raw_token(char buf[], int bufsiz, FILE *fp)
{
    /* TODO: handle escaped quotes inside a string */

    int i, quote_char;

    /*TODO: assert quote_char == " || quote_char == ' ??? */
    quote_char = buf[0] = ngetch(fp);

    for (i = 1; (buf[i] = ngetch(fp)) != quote_char; i++) {
        if (is_newline(buf[i])) {
            synerr("missing quote.");
        } else if (i >= bufsiz - 2) {
            /* we might still have space for closing quote, but will
               surely lack space for string terminating character EOF */
            synerr("string too long.");
        } else {
            continue; /* no err, go to next iteration */
        }
        put_back_char(buf[i]);
        buf[i] = buf[0];
        break;
    }
    buf[++i] = EOS;
    return(TOKT_STRING);
}

/* Read a non-alphanumeric token from input stream fp (accounting for
 * composed tokens), save it in buf[], and return the type of the token
 * read. */
static int
get_non_alphanumeric_raw_token(char buf[], int bufsiz, FILE *fp)
{
    int tok;
    /* TODO: assert bufsiz >= 3 */
    buf[0] = ngetch(fp);
    put_back_char(buf[0]);
    switch (buf[0]) {
        case GREATER:
        case LESS:
        case EQUALS:
        case NOT:
        case BANG:
        case CARET:
        case AND:
        case OR:
            /* maybe a ratfor relational shorthand */
            tok = convert_relational_shortand(buf, bufsiz, fp);
            break;
        case SHARP:
            /* # ratfor comment */
            if (!keep_comments || !reading_parenthesized_macro_definition) {
                (void) ngetch(fp); /* TODO: assert == buf[0] */
                dispatch_comment(fp);
                tok = buf[0] = NEWLINE;
                buf[1] = EOS;
            } else {
                tok = SHARP;
                read_until_char_match(buf, bufsiz, fp, is_newline,
                                      "comment in define(...) too long.");
            }
            break;
        default:
            /* everything else */
            (void) ngetch(fp); /* TODO: assert == buf[0] */
            tok = buf[0];
            buf[1] = EOS;
            break;
    }
    return(tok);
}

/* Get a "raw" ratfor token from input stream fp (i.e. without performing
 * macro expansion), and save it in buf[].  Also deal with ratfor comments
 * (# COMMENT...) and verbatim escapes (% VERBATIM..).  Return the type of
 * the token read. */
static int
get_raw_token(char buf[], int bufsiz, FILE *fp)
{
    int tok;
    int c;
    c = buf[0] = ngetch(fp);
    if (is_blank(c)) {
        buf[0] = BLANK;
        while (is_blank(c)) /* compress many blanks to one */
            c = ngetch(fp);
        if (c == SHARP) {
            /* Special handling of `#' to avoid leaving extra white
             * spaces in output. */
            goto non_blank; /* fallthrough */
        }
        if (!is_newline(c))
            put_back_char(c);
        else
            buf[0] = c;
        buf[1] = EOS;
        return(buf[0]);
    }
non_blank:
    put_back_char(c); /* so that we can read back the whole token */
    if (is_rat4_alpha(c)) {
        tok = get_alphanumeric_raw_token(buf, bufsiz, fp);
    } else if (is_digit(c)) {
        tok = get_numerical_raw_token(buf, bufsiz, fp);
    } else if (c == SQUOTE || c == DQUOTE) {
        tok = get_quoted_string_raw_token(buf, bufsiz, fp);
    } else {
        tok = get_non_alphanumeric_raw_token(buf, bufsiz, fp);
    }
    return(tok);
}

/* Get token and save it in buf[], expanding macro calls. */
static int
get_expanded_token(char buf[], int bufsiz)
{
    char defn[MAXDEFLEN];
    int t;

    while ((t = get_unexpanded_token(buf, bufsiz)) != EOF) {
        if (t != TOKT_ALPHA) {
            break; /* non-alpha */
        } else if (!defn_lookup(buf, defn)) {
            break; /* not a macro name */
        } else {
            /* Push replacement onto input, with newlines properly
             * substituted by "fake newlines". This hack is needed
             * to keep the count of line in input correct even if
             * expansion of multiline macros is involved. */
            put_back_string_cooked(defn);
        }
    }
    return(t);
}

/* Open path and push it on the input files stack. Deal with errors. */
/* XXX: re-add support for quoted filenames, sooner or later */
static void
push_file_stack(const char *path)
{
    int i;
    FILE *fp;

    if (inclevel >= MAX_INCLUDE_DEPTH - 1) {
        synerr_include("includes nested too deeply.");
        return;
    }
    /* skip leading white space in path */
    for (i = 0; is_blank(path[i]); i++)
        /* empty body */;
    if ((path = strdup(&path[i])) == NULL) {
        synerr_include("memory error.");
        return;
    }
    if (*path == EOS) {
        synerr_include("missing filename.");
        return;
    }
    if ((fp = xopen(path, IO_MODE_READ, synerr_include)) == NULL)
        return;
    ++inclevel;
    lineno[inclevel] = 1;
    filename[inclevel] = path;
    infile[inclevel] = fp;
}

/* Pop the input files stack. */
static void
pop_file_stack(void)
{
    /* XXX: assert inclevel >= 0? */
    if (inclevel > 0) {
        fclose(infile[inclevel]); /* XXX: check return status? */
        infile[inclevel] = NULL; /* just to be sure */
        free((void *)filename[inclevel]);
        filename[inclevel] = NULL; /* just to be sure */
    }
    inclevel--;
}


/*
 * PUBLIC FUNCTIONS.
 */

BEGIN_C_DECLS

/* Get "unpreprocessed" (i.e. without performing macro expansion) token
 * from ratfor current global input stream, and save it in buf[].
 * Also deal with ratfor comments (# COMMENT...) and verbatim escapes
 * (% VERBATIM..).  Return the type of the token read. */
int
get_unexpanded_token(char buf[], int bufsiz)
{
    int tok;

    while (inclevel >= 0) {
        if ((tok = get_raw_token(buf, bufsiz, infile[inclevel])) != EOF)
            return(tok);
        /* close include and pop file name stack */
        pop_file_stack();
    }
    /* in case called again after input ended */
    buf[0] = EOF;
    buf[1] = EOS;
    tok = EOF;
    return(tok);
}

/* Get token from ratfor current global input stream (handling macro
 * expansions, macro definitons and file inclusion), and save it in
 * buf[]. */
int
get_token(char buf[], int bufsiz)
{
    int t, i;
    int tok;
    char path[MAXPATH];

    for(;;) {
        tok = get_expanded_token(buf, bufsiz);
        if (!STREQ(buf, "include"))
            return(tok);
        /* FIXME: file inclusion to be moved out to parser */
        /* deal with file inclusion */
        for (i = 0; ; i = SSTRLEN(path)) {
            if (i >= MAXPATH)
                synerr_fatal("name of included file too long.");
            t = get_expanded_token(&path[i], MAXPATH);
            if (is_stmt_ending(t))
                break;
        }
        path[i] = EOS;
        push_file_stack(path);
    }
}

/* Like `get_token()', but skip any leading blanks. */
int
get_nonblank_token(char buf[], int bufsiz)
{
    int tok;
    do {
        tok = get_token(buf, bufsiz);
    } while(is_blank(tok));
    return(tok);
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
