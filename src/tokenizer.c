/* Get tokens for the ratfor preprocessor. */

#include "rat4-common.h"

#include <stdlib.h>
#include <ctype.h>

#include "utils.h"
#include "io.h"
#include "error.h"
#include "lookup.h"
#include "lex-symbols.h"
#include "rat4-global.h"


/*
 *  P R I V A T E  V A R I A B L E S
 */


static const char incl[] = "include";
static const char fncn[] = "function";
static const char defn[] = "define";


/*
 *  P R I V A T E  F U N C T I O N S
 */


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

/*
 * skpblk - skip blanks and tabs in file fp
 *
 */
static void
skpblk(FILE *fp)
{
    char c;

    for (c = ngetch(fp); is_blank(c); c = ngetch(fp))
        /* skip blank characters */;
    putbak(c);
}

/*
 * type - return LETTER, DIGIT or char itslef if it's neither
 *
 */
static int
type(char c)
{
    int t;

    if (isdigit(c))
        t = DIGIT;
    else if (isupper(c) || islower(c))
        t = LETTER;
    else
        t = c;
    return(t);
}

/*
 * relate - convert relational shorthands into long form
 *
 */
static int
relate(char token[], FILE *fp)
{

    if ((token[1] = ngetch(fp)) == EQUALS) {
        token[2] = 'e';
    } else {
        putbak(token[1]);
        token[2] = 't';
    }
    token[3] = PERIOD;
    token[4] = EOS;
    token[5] = EOS; /* for .not. and .and. */
    
    switch(token[0]) {
        case GREATER:
            token[1] = 'g';
            break;
        case LESS:
            token[1] = 'l';
            break;
        case NOT:
        case BANG:
        case CARET:
            if (token[1] != EQUALS) {
                token[2] = 'o';
                token[3] = 't';
                token[4] = PERIOD;
            }
            token[1] = 'n';
            break;
        case EQUALS:
            if (token[1] != EQUALS) { /* variable assignement */
                token[2] = EOS;
                return(0);
            }
            token[1] = 'e';
            token[2] = 'q';
            break;
        case AND:
            if ((token[1] = ngetch(fp)) != AND) /* look for && or & */ 
                putbak(token[1]);
            token[1] = 'a';
            token[2] = 'n';
            token[3] = 'd';
            token[4] = PERIOD;
            break;
        case OR:
            if ((token[1] = ngetch(fp)) != OR) /* look for || or | */
                putbak(token[1]);
            token[1] = 'o';
            token[2] = 'r';
            break;
        default: /* can't happen */ 
            token[1] = EOS;
            break;
    }  /* switch(token[0]) */
    token[0] = PERIOD;
    return(strlen(token)-1);
}

/*
 * gtok - get token for Ratfor
 *
 */
static int
gtok(char lexstr[], int toksiz, FILE *fp)
{
    int i, b, n, tok;
    char c;
    c = lexstr[0] = ngetch(fp);
    if (is_blank(c)) {
        lexstr[0] = BLANK;
        while (is_blank(c)) /* compress many blanks to one */
            c = ngetch(fp);
        if (c == PERCENT) {
            /* XXX: make this lazily done */
            outasis(fp); /* copy direct to output if % */
            c = NEWLINE;
        }
        else if (c == SHARP) {
            dispatch_comment(fp);
            c = NEWLINE;
        }
        if (!is_newline(c))
            putbak(c);
        else
            lexstr[0] = c;
        lexstr[1] = EOS;
        return(lexstr[0]);
    }
    i = 0;
    tok = type(c);
    if (tok == LETTER) { /* alphanumeric token */
        for (i = 0; i < toksiz - 3; i++) {
            lexstr[i+1] = ngetch(fp);
            tok = type(lexstr[i+1]);
            /* Test for DOLLAR added by BM, 7-15-80 */
            if (tok != LETTER && tok != DIGIT && tok != UNDERLINE
                && tok != DOLLAR && tok != PERIOD)
                break;
        }
        putbak(lexstr[i+1]);
        tok = ALPHA;
    }
    else if (tok == DIGIT) { /* number */
        b = c - DIG0; /* in case alternate base number */
        for (i = 0; i < toksiz - 3; i++) {
            lexstr[i+1] = ngetch(fp);
            if (type(lexstr[i+1]) != DIGIT)
                break;
            b = 10*b + lexstr[i+1] - DIG0;
        }
        if (lexstr[i+1] == RADIX && b >= 2 && b <= 36) {
            /* n%ddd... */
            for (n = 0; ; n = b*n + c) {
                c = lexstr[0] = ngetch(fp);
                if (c >= 'a' && c <= 'z')
                    c = c - 'a' + DIG9 + 1;
                else if (c >= 'A' && c <= 'Z')
                    c = c - 'A' + DIG9 + 1;
                if (c < DIG0 || c >= DIG0 + b)
                    break;
                c = c - DIG0;
            }
            putbak(lexstr[0]);
            i = itoc(n, lexstr, toksiz);
        }
        else
            putbak(lexstr[i+1]);
        tok = DIGIT;
    }
    else if (c == SQUOTE || c == DQUOTE) {
        /* XXX: handle escaepd quotes inside a string */
        for (i = 1; (lexstr[i] = ngetch(fp)) != lexstr[0]; i++) {
            if (lexstr[i] == UNDERLINE) {
                c = ngetch(fp);
                if (is_newline(c)) {
                    c = ngetch(fp);
                    lexstr[i] = c;
                } else {
                    putbak(c);
                }
            }
            if (is_newline(lexstr[i]) || i >= toksiz-1) {
                synerr("missing quote.");
                putbak(lexstr[i]);
                lexstr[i] = lexstr[0];
                break;
            }
        }
    }
    else if (c == PERCENT) {
        /* XXX: make this lazily done */
        outasis(fp);  /* direct copy of protected */
        tok = NEWLINE;
    }
    else if (c == SHARP) {
        dispatch_comment(fp);
        tok = lexstr[0] = NEWLINE;
    }
    else if (c == GREATER || c == LESS || c == NOT
             || c == BANG || c == CARET || c == EQUALS
             || c == AND || c == OR)
        i = relate(lexstr, fp);
    
    if (i >= toksiz - 1)
        synerr("token too long.");
    lexstr[i+1] = EOS;

    return(tok);
}

/*
 * getdef (for no arguments) - get name and definition
 *
 */
static void
getdef(char name[], int namesiz, char def[], int defsiz, FILE *fp)
{
    int i, nlpar, t, t2, defn_with_paren;
    char c, ptoken[MAXTOK]; /* temporary buffer for token */

    skpblk(fp);
    if ((t = gtok(ptoken, MAXTOK, fp)) != LPAREN) {
        defn_with_paren = NO; /* define name def */
        pbstr(ptoken);
    } else {
        defn_with_paren = YES; /* define(name,def) */
    }
    skpblk(fp);
    t2 = gtok(name, namesiz, fp); /* name */
    if (!defn_with_paren && is_stmt_ending(t2)) {
        /* stray `define', as in `...; define; ...' */
        synerr_fatal("empty name.");
    } else if (defn_with_paren && t2 == COMMA) {
        /* `define(name, def)' with empty name */
        synerr_fatal("empty name.");
    } else if (t2 != ALPHA) {
        synerr_fatal("non-alphanumeric name.");
    }
    skpblk(fp);
    c = gtok(ptoken, MAXTOK, fp);
    if (!defn_with_paren) { /* define name def */
        pbstr(ptoken);
        i = 0;
        do {
            c = ngetch(fp);
            if (i > defsiz)
                synerr_fatal("definition too long.");
            def[i++] = c;
        } while (c != SHARP && !is_newline(c) && c != EOF && c != PERCENT);
        if (c == SHARP || c == PERCENT)
            putbak(c);
    }
    else { /* define (name, def) */
        if (c != COMMA)
            synerr_fatal("missing comma in define.");
        /* else got (name, */
        nlpar = 0;
        for (i = 0; nlpar >= 0; i++) {
            if (i > defsiz)
                synerr_fatal("definition too long.");
            else if ((def[i] = ngetch(fp)) == EOF)
                synerr_fatal("missing right paren.");
            else if (def[i] == LPAREN)
                nlpar++;
            else if (def[i] == RPAREN)
                nlpar--;
            /* else normal character in def[i] */
        }
    }
    def[i-1] = EOS;
}

/*
 * deftok - get token; process macro calls and invocations
 *
 */
static int
deftok(char token[], int toksiz, FILE *fp)
{
    char tkdefn[MAXDEF];
    int i, t;

    while ((t = gtok(token, toksiz, fp)) != EOF) {
        if (t != ALPHA) {
            break;  /* non-alpha */
        } else if (STREQ(token, defn)) {
            /* get definition for token, save it in tkdefn */
            getdef(token, toksiz, tkdefn, MAXDEF, fp);
            install(token, tkdefn);
        } else if (look(token, tkdefn) == NO) {
            break;  /* undefined */
        } else {
            /* Push replacement onto input, with newlines substituted
             * by "bell" characters (ascii 007). This hack is needed to
             * keep the count of line in input correct even if expansion
             * of multiline macros is involved. */
            for (i = strlen(tkdefn) - 1; i >= 0; i--) {
                if (is_newline(tkdefn[i]))
                    putbak(FKNEWLINE);
                else
                    putbak(tkdefn[i]);
            }
        }
    }
    /* XXX: better to move this in fortran-generating code, or something
     * like that... */
    if (t == ALPHA) /* convert to single case */
        fold(token);
    return(t);
}


/*
 *  P U B L I C  F U N C T I O N S
 */


/*
 * gettok - get token; handles file inclusion and fix line numbers
 *
 */
int
gettok(char token[], int toksiz)
{
    int t, i, j;
    int tok;
    char name[MAXNAME];

    for ( ; level >= 0; level--) {
        while((tok = deftok(token, toksiz, infile[level])) != EOF) {
            if (STREQ(token, fncn)) {
                skpblk(infile[level]);
                t = deftok(fcname, MAXNAME, infile[level]);
                pbstr(fcname);
                if (t != ALPHA)
                    synerr("missing function name.");
                putbak(BLANK);
                return(tok);
            }
            if (!STREQ(token, incl)) {
                return(tok);
            }
            /* deal with file inclusion */
            for (i = 0; ; i = strlen(name)) {
                t = deftok(&name[i], MAXNAME, infile[level]);
                if (is_stmt_ending(t))
                    break;
            }
            name[i] = EOS;
            if (level >= NFILES - 1)
                synerr_include("includes nested too deeply.");
            else {
/*XXX re-add support for quoted filenames, sooner or later
                name[i-1]=EOS;
                infile[level+1] = fopen(&name[2], "r");
*/
                /* skip leading white space in name */
                for (j = 0; is_blank(name[j]); j++)
                    /* empty body */;
                filename[level+1] = strsave(&name[j]);
                if (filename[level+1] == NULL) {
                    synerr_include("memory error.");
                    goto include_done;
                }
                if (*filename[level+1] == EOS) {
                    synerr_include("missing filename.");
                    goto include_done;
                }
                infile[level+1] = fopen(filename[level+1], "r");
                if (infile[level+1] == NULL) {
                    synerr_include("I/O error.");
                    goto include_done;
                }
                lineno[level+1] = 1;
                ++level;
include_done:
                /* nothing else to do */;
            }
        }
        if (level > 0) {  /* close include and pop file name stack */
            fclose(infile[level]);
            infile[level] = NULL; /* just to be sure */
            free((void *)filename[level]);
            filename[level] = NULL; /* just to be sure */
        }
    }
    token[0] = EOF;   /* in case called more than once */
    token[1] = EOS;
    tok = EOF;
    return(tok);
}

/*
 * gnbtok - get nonblank token
 *
 */
int
gnbtok(char token[], int toksiz)
{
    int tok;
    skpblk(infile[level]);
    tok = gettok(token, toksiz);
    return(tok);
}

/* vim: set ft=c ts=4 sw=4 et : */
