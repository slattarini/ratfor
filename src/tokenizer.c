/* Get tokens for the ratfor preprocessor. */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ratdef.h"
#include "ratcom.h"
#include "utils.h"
#include "lookup.h"
#include "error.h"
#include "io.h"


/*
 *  P R I V A T E  V A R I A B L E S
 */

static const char incl[] = "include";
static const char fncn[]  = "function";


/*
 *  P R I V A T E  F U N C T I O N S
 */


/*
 * skpblk - skip blanks and tabs in file fp
 *
 */
static void
skpblk(FILE *fp)
{
    char c;

    for (c = ngetch(fp); c == BLANK || c == TAB; c = ngetch(fp))
        /* empty body */;
    putbak(c);
}

/*
 * type - return LETTER, DIGIT or char
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

    if ((token[1] = ngetch(fp)) != EQUALS) {
        putbak(token[1]);
        token[2] = 't';
    } else {
        token[2] = 'e';
    }
    token[3] = PERIOD;
    token[4] = EOS;
    token[5] = EOS; /* for .not. and .and. */
    if (token[0] == GREATER)
        token[1] = 'g';
    else if (token[0] == LESS)
        token[1] = 'l';
    else if (token[0] == NOT || token[0] == BANG || token[0] == CARET) {
        if (token[1] != EQUALS) {
            token[2] = 'o';
            token[3] = 't';
            token[4] = PERIOD;
        }
        token[1] = 'n';
    }
    else if (token[0] == EQUALS) {
        if (token[1] != EQUALS) {
            token[2] = EOS;
            return(0);
        }
        token[1] = 'e';
        token[2] = 'q';
    }
    else if (token[0] == AND) { /* look for && or & */
        if ((token[1] = ngetch(fp)) != AND)
            putbak(token[1]);
        token[1] = 'a';
        token[2] = 'n';
        token[3] = 'd';
        token[4] = PERIOD;
    }
    else if (token[0] == OR) {
        if ((token[1] = ngetch(fp)) != OR) /* look for || or | */
            putbak(token[1]);
        token[1] = 'o';
        token[2] = 'r';
    }
    else   /* can't happen */
        token[1] = EOS;
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
    if (c == BLANK || c == TAB) {
        lexstr[0] = BLANK;
        while (c == BLANK || c == TAB) /* compress many blanks to one */
            c = ngetch(fp);
        if (c == PERCENT) {
            outasis(fp); /* copy direct to output if % */
            c = NEWLINE;
        }
        if (c == SHARP) {
            if (leaveC == YES) {
              outcmnt(fp); /* copy comments to output */
              c = NEWLINE;
            }
            else
                while ((c = ngetch(fp)) != NEWLINE) /* strip comments */
                    /* empty body */;
        }
/*
        if (c == UNDERLINE)
            if ((c = ngetch(fp)) == NEWLINE)
                while ((c = ngetch(fp)) == NEWLINE)
                    ;
            else
            {
                putbak(c);
                c = UNDERLINE;
            }
*/
        if (c != NEWLINE)
            putbak(c);
        else
            lexstr[0] = NEWLINE;
        lexstr[1] = EOS;
        return((int)lexstr[0]);
    }
    i = 0;
    tok = type(c);
    if (tok == LETTER) { /* alpha */
        for (i = 0; i < toksiz - 3; i++) {
            lexstr[i+1] = ngetch(fp);
            tok = type(lexstr[i+1]);
            /* Test for DOLLAR added by BM, 7-15-80 */
            if (tok != LETTER && tok != DIGIT
                && tok != UNDERLINE && tok!=DOLLAR
                && tok != PERIOD)
                break;
        }
        putbak(lexstr[i+1]);
        tok = ALPHA;
    }
    else if (tok == DIGIT) { /* digits */
        b = c - DIG0; /* in case alternate base number */
        for (i = 0; i < toksiz - 3; i++) {
            lexstr[i+1] = ngetch(fp);
            if (type(lexstr[i+1]) != DIGIT)
                break;
            b = 10*b + lexstr[i+1] - DIG0;
        }
        if (lexstr[i+1] == RADIX && b >= 2 && b <= 36) {
            /* n%ddd... */
            for (n = 0;; n = b*n + c) {
                c= lexstr[0] = ngetch(fp);
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
#ifdef SQUAREB
    else if (c == LBRACK) { /* allow [ for { */
        lexstr[0] = LBRACE;
        tok = LBRACE;
    }
    else if (c == RBRACK) { /* allow ] for } */
        lexstr[0] = RBRACE;
        tok = RBRACE;
    }
#endif
    else if (c == SQUOTE || c == DQUOTE) {
        for (i = 1; (lexstr[i] = ngetch(fp)) != lexstr[0]; i++) {
            if (lexstr[i] == UNDERLINE)
                if ((c = ngetch(fp)) == NEWLINE) {
                    while (c == NEWLINE || c == BLANK || c == TAB)
                        c = ngetch(fp);
                    lexstr[i] = c;
                }
                else
                    putbak(c);
            if (lexstr[i] == NEWLINE || i >= toksiz-1) {
                synerr("missing quote.");
                lexstr[i] = lexstr[0];
                putbak(NEWLINE);
                break;
            }
        }
    }
    else if (c == PERCENT) {
        outasis(fp);        /* direct copy of protected */
        tok = NEWLINE;
    }
    else if (c == SHARP) {
        if(leaveC == YES)
          outcmnt(fp);      /* copy comments to output */
        else
          while ((lexstr[0] = ngetch(fp)) != NEWLINE)
            /* strip comments */;
          tok = NEWLINE;
    }
    else if (c == GREATER || c == LESS || c == NOT
             || c == BANG || c == CARET || c == EQUALS
             || c == AND || c == OR)
        i = relate(lexstr, fp);
    if (i >= toksiz-1)
        synerr("token too long.");
    lexstr[i+1] = EOS;
    if (lexstr[0] == NEWLINE)
        linect[level] = linect[level] + 1;

    /* cray cannot compare char and ints, since EOF is an int we check
       with feof */
    if (feof(fp))
        tok = EOF;

    return(tok);
}

/*
 * getdef (for no arguments) - get name and definition
 *
 */
static void
getdef(char token[], int toksiz, char defn[], int defsiz, FILE *fp)
{
    int i, nlpar, t, t2;
    char c, ptoken[MAXTOK];

    skpblk(fp);
    /*
     * define(name,defn) or
     * define name defn
     *
     */
    if ((t = gtok(ptoken, MAXTOK, fp)) != LPAREN) {;
        t = BLANK; /* define name defn */
        pbstr(ptoken);
    }
    skpblk(fp);
    t2 = gtok(token, toksiz, fp); /* name */
    if (t == BLANK && (t2 == NEWLINE || t2 == SEMICOL)) {
        /* stray `define', as in `...; define; ...' */
        baderr("empty name.");
    } else if (t == LPAREN && t2 == COMMA) {
        /* `define (name, defn)' with empty name */
        baderr("empty name.");
    } else if (t2 != ALPHA) {
        baderr("non-alphanumeric name.");
    }
    skpblk(fp);
    c = gtok(ptoken, MAXTOK, fp);
    if (t == BLANK) { /* define name defn */
        pbstr(ptoken);
        i = 0;
        do {
            c = ngetch(fp);
            if (i > defsiz)
                baderr("definition too long.");
            defn[i++] = c;
        } while (c != SHARP && c != NEWLINE && c != EOF && c != PERCENT);
        if (c == SHARP || c == PERCENT)
            putbak(c);
    }
    else if (t == LPAREN) { /* define (name, defn) */
        if (c != COMMA)
            baderr("missing comma in define.");
        /* else got (name, */
        nlpar = 0;
        for (i = 0; nlpar >= 0; i++)
            if (i > defsiz)
                baderr("definition too long.");
            else if ((defn[i] = ngetch(fp)) == EOF)
                baderr("missing right paren.");
            else if (defn[i] == LPAREN)
                nlpar++;
            else if (defn[i] == RPAREN)
                nlpar--;
        /* else normal character in defn[i] */
    }
    else
        baderr("getdef is confused.");
    defn[i-1] = EOS;
}

/*
 * deftok - get token; process macro calls and invocations
 *
 */
static int
deftok(char token[], int toksiz, FILE *fp)
{
    char defn[MAXDEF];
    int t;

    for (t=gtok(token, toksiz, fp); t!=EOF; t=gtok(token, toksiz, fp)) {
        if (t != ALPHA) /* non-alpha */
            break;
        if (look(token, defn) == NO) /* undefined */
            break;
        if (defn[0] == DEFTYPE) { /* get definition */
            getdef(token, toksiz, defn, MAXDEF, fp);
            install(token, defn);
        }
        else
            pbstr(defn); /* push replacement onto input */
    }
    if (t == ALPHA) /* convert to single case */
        fold(token);
    return(t);
}


/*
 *  P U B L I C  F U N C T I O N S
 */


/*
 * gettok - get token. Handles file inclusion and line numbers
 *
 */
int
gettok(char token[], int toksiz)
{
    int t, i, j;
    int tok;
    char name[MAXNAME];

    for ( ; level >= 0; level--) {
        for (tok = deftok(token, toksiz, infile[level]); tok != EOF;
             tok = deftok(token, toksiz, infile[level]))
        {
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
                if (t == NEWLINE || t == SEMICOL) {
                    pbstr(&name[i]);
                    break;
                }
            }
            name[i] = EOS;
/*WSB 6-25-91
            if (name[1] == SQUOTE) {
                outtab();
                outstr(token);
                outstr(name);
                outdon();
                eatup();
                return(tok);
            }
*/
            if (level >= NFILES)
                synerr("includes nested too deeply.");
            else {
/*XXX re-add support for quoted filenames, sooner or later
                name[i-1]=EOS;
                infile[level+1] = fopen(&name[2], "r");
*/
                /* skip leading white space in name */
                for (j = 0; name[j] == BLANK || name[j] == TAB; j++)
                    /* empty body */;
                filename[level+1] = strsave(&name[j]);
                if (filename[level+1] == NULL) {
                    synerr("cannot open include: memory error."); /*XXX improve errmsg */
                    goto include_done;
                }
                infile[level+1] = fopen(filename[level+1], "r");
                if (infile[level+1] == NULL) {
                    synerr("cannot open include: I/O error."); /*XXX improve errmsg */
                    goto include_done;
                }
                linect[level+1] = 1;
                ++level;
include_done:
                /* nothing else to do */;
            }
        }
        if (level > 0) {  /* close include and pop file name stack */
            fclose(infile[level]);
            infile[level] = NULL; /* just to be sure */
            free(filename[level]);
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
