/* Get tokens for the ratfor preprocessor. */

#include "rat4-common.h"

#include <ctype.h>

#include "tokenizer.h"
#include "utils.h"
#include "io.h"
#include "xopen.h"
#include "error.h"
#include "lookup.h"
#include "lex-symbols.h"
#include "rat4-global.h"

#define MAXPATH     1024    /* max length of the name of a file included */
#define MAXDEFLEN   2048    /* max length of a ratfor macro's definition */

/* NOTE: due to implementation details, it is pointless to have MAXDEFLEN
         here greater than BUFSIZE in io.c */

/*
 * Private Variables.
 */

static const char KEYWORD_INCLUDE[] = "include";
static const char KEYWORD_FUNCTION[] = "function";
static const char KEYWORD_DEFINE[] = "define";


/*
 * Private Functions.
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

static void
skip_blanks(FILE *fp)
{
    char c;
    for (c = ngetch(fp); is_blank(c); c = ngetch(fp))
        /* skip blank characters */;
    put_back_char(c);
}

/* Return character type -- this "type" is the character itself unless
 * it is a letter or a digit. */
static int
char_type(char c)
{
    int t;
    if (isdigit(c))
        t = DIGIT;
    else if (isupper(c) || islower(c))
        t = LETTER;
    else
        t = c; /*XXX: "OTHER" */
    return(t);
}

/* Convert relational shorthands (e.g. `&&' => `.and.', '<' => '.gt.'),
 * write the resulting string in token[] itslef.  Return the lenght of
 * the resulting string. */
static int
convert_relation_shortand(char token[], FILE *fp)
{
    if ((token[1] = ngetch(fp)) == EQUALS) {
        token[2] = 'e';
    } else {
        put_back_char(token[1]);
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
                return(1);
            }
            token[1] = 'e';
            token[2] = 'q';
            break;
        case AND:
            if ((token[1] = ngetch(fp)) != AND) /* look for && or & */
                put_back_char(token[1]);
            token[1] = 'a';
            token[2] = 'n';
            token[3] = 'd';
            token[4] = PERIOD;
            break;
        case OR:
            if ((token[1] = ngetch(fp)) != OR) /* look for || or | */
                put_back_char(token[1]);
            token[1] = 'o';
            token[2] = 'r';
            break;
        default: /* can't happen */
            token[1] = EOS;
            break;
    }  /* switch(token[0]) */
    token[0] = PERIOD;
    return(SSTRLEN(token));
}

static bool
can_char_be_composed(int c)
{
    switch(c) {
        case SLASH:
        case STAR:
            return true;
    }
    return false;
}


/* read alphanumeric token from fp, save it in lexstr, return the lenght of
 * the token read */
static int
get_alphanumeric_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int i;
    for (i = 0; i < toksiz - 2; i++) {
        lexstr[i] = ngetch(fp);
        switch(char_type(lexstr[i])) {
            case LETTER:
            case DIGIT:
            case UNDERLINE:
            case DOLLAR:
            case PERIOD:
                break;
            default:
                put_back_char(lexstr[i--]);
                goto out;
        }
    }
out:
    return(i+1);
}

/* read numerical token from fp, save it in lexstr, return the lenght of
 * the token read */
static int
get_numerical_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int i, c, b, n;
    b = 0; /* in case alternate base number */
    for (i = 0; i < toksiz - 2; i++) {
        lexstr[i] = ngetch(fp);
        if (char_type(lexstr[i]) != DIGIT)
            break;
        b = 10*b + lexstr[i] - DIG0;
    }
    if (lexstr[i] == RADIX && b >= 2 && b <= 36) {
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
        put_back_char(lexstr[0]);
        i = itoc(n, lexstr, toksiz);
    }
    else {
        put_back_char(lexstr[i--]);
    }
    return(i+1);
}

/* read "quoted string" token from fp, save it in lexstr, return the
 * lenght of the token read */
static int
get_quoted_string_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    /* TODO: handle escaped quotes inside a string */

    int i, c, quote_char;

    /*TODO: assert quote_char == " || quote_char == ' ??? */
    quote_char = lexstr[0] = ngetch(fp);

    for (i = 1; (lexstr[i] = ngetch(fp)) != quote_char; i++) {
        /* XXX: but is not this already done by io.c:ngetch()? */
        if (lexstr[i] == UNDERLINE) {
            c = ngetch(fp);
            if (is_newline(c)) {
                c = ngetch(fp);
                lexstr[i] = c;
            } else {
                put_back_char(c);
            }
        }
        if (is_newline(lexstr[i]) || i >= toksiz - 1) {
            synerr("missing quote.");
            put_back_char(lexstr[i]);
            lexstr[i] = lexstr[0];
            break;
        }
    }
    return(i+1);
}

/*
 * get_raw_token() - get raw ratfor token.
 * Also deal with comments (# COMMENT...) and verbatim lines
 * (% VERBATIME LINE...).
 * Return the type of the token read.
 */
static int
get_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int tok;
    int toklen; /* the lenght of the token read */
    int c, nc;
    int i = 0; /* XXX: temporary */
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
            put_back_char(c);
        else
            lexstr[0] = c;
        lexstr[1] = EOS;
        return(lexstr[0]);
    }
    toklen = 1;
    if (char_type(c) == LETTER) {
        put_back_char(c); /* so that we can read back the whole token */
        toklen = get_alphanumeric_raw_token(lexstr, toksiz, fp);
        tok = ALPHA;
    } else if (char_type(c) == DIGIT) {
        put_back_char(c); /* so that we can read back the whole token */
        toklen = get_numerical_raw_token(lexstr, toksiz, fp);
        tok = DIGIT;
    }
    else if (c == SQUOTE || c == DQUOTE) {
        put_back_char(c); /* so that we can read back the whole token */
        toklen = get_quoted_string_raw_token(lexstr, toksiz, fp);
        tok = STRING;
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
             || c == AND || c == OR
    ) {
        toklen = convert_relation_shortand(lexstr, fp);
        tok = c; /*XXX: temporary hack */
    } else if (can_char_be_composed(c)) {
        /* TODO: wrap in a subroutine */
        nc = ngetch(fp); /* peek next character */
        if (c == STAR && nc == STAR) { /* fortran `**' operator */
            lexstr[++i] = STAR;
            tok = OPEREXP;
        } else if (c == SLASH && nc == SLASH) { /* fortran `//' operator */
            lexstr[++i] = SLASH;
            tok = OPERSTRCAT;
        } else { /* nothing special, put back the peeked character */
            put_back_char(nc);
            tok = c;
        }
    } else {
        tok = c;
    }

    if (toklen >= toksiz)
        synerr("token too long.");
    lexstr[toklen] = EOS;
    return(tok);
}

/* Parse definition of ratfor macro. If an error is detected, stop ratfor
 * with a suitable error message, alse save macro name (in `name[]') and
 * macro definition (in `def[]'). */
static void
getdef(char name[], int namesiz, char def[], int defsiz, FILE *fp)
{
    int i, nlpar, t, t2;
    bool defn_with_paren;
    char c, ptoken[MAXTOK]; /* temporary buffer for token */

    skip_blanks(fp);
    if ((t = get_raw_token(ptoken, MAXTOK, fp)) != LPAREN) {
        defn_with_paren = false; /* define name def */
        put_back_string(ptoken);
    } else {
        defn_with_paren = true; /* define(name,def) */
    }
    skip_blanks(fp);
    t2 = get_raw_token(name, namesiz, fp); /* name */
    if (!defn_with_paren && is_stmt_ending(t2)) {
        /* stray `define', as in `...; define; ...' */
        synerr_fatal("empty name.");
    } else if (defn_with_paren && t2 == COMMA) {
        /* `define(name, def)' with empty name */
        synerr_fatal("empty name.");
    } else if (t2 != ALPHA) {
        synerr_fatal("non-alphanumeric name.");
    }
    skip_blanks(fp);
    c = get_raw_token(ptoken, MAXTOK, fp); /*XXX: t2 here instead of c? */
    if (!defn_with_paren) { /* define name def */
        put_back_string(ptoken);
        i = 0;
        do {
            c = ngetch(fp);
            if (i > defsiz)
                synerr_fatal("definition too long.");
            def[i++] = c;
        } while (c != SHARP && !is_newline(c) && c != EOF && c != PERCENT);
        if (c == SHARP || c == PERCENT)
            put_back_char(c);
    }
    else { /* define (name, def) */
        if (c != COMMA) /*XXX: t2 here instead of c? */
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

/* Get token, expanding macro calls and processing macro definitions. */
/* XXX: clearer name? */
static int
deftok(char token[], int toksiz, FILE *fp)
{
    char tkdefn[MAXDEFLEN];
    int i, t;

    while ((t = get_raw_token(token, toksiz, fp)) != EOF) {
        if (t != ALPHA) {
            break;  /* non-alpha */
        } else if (STREQ(token, KEYWORD_DEFINE)) {
            /* get definition for token, save it in tkdefn */
            getdef(token, toksiz, tkdefn, MAXDEFLEN, fp);
            install(token, tkdefn);
        } else if (!look(token, tkdefn)) {
            break;  /* undefined */
        } else {
            /* Push replacement onto input, with newlines substituted
             * by "bell" characters (ascii 007). This hack is needed to
             * keep the count of line in input correct even if expansion
             * of multiline macros is involved. */
            for (i = SSTRLEN(tkdefn) - 1; i >= 0; i--) {
                if (is_newline(tkdefn[i]))
                    put_back_char(FKNEWLINE);
                else
                    put_back_char(tkdefn[i]);
            }
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
    if ((path = strsave(&path[i])) == NULL) {
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
 * Public Functions.
 */

BEGIN_C_DECLS

/* Get token (handling macro expansions, macro definitons and file
 * inclusion). */
int
get_token(char token[], int toksiz)
{
    int t, i;
    int tok;
    char path[MAXPATH];

    while (inclevel >= 0) {
        while ((tok = deftok(token, toksiz, infile[inclevel])) != EOF) {
            if (STREQ(token, KEYWORD_FUNCTION)) {
                skip_blanks(infile[inclevel]);
                t = deftok(current_function_name, MAXFUNCNAME,
                           infile[inclevel]);
                put_back_string(current_function_name);
                if (is_stmt_ending(t))
                    synerr("missing function name.");
                else if (t != ALPHA)
                    synerr("invalid function name `%s'",
                           current_function_name);
                put_back_char(BLANK);
                return(tok);
            }
            if (!STREQ(token, KEYWORD_INCLUDE))
                return(tok);
            /* deal with file inclusion */
            for (i = 0; ; i = SSTRLEN(path)) {
                /* XXX possible segfault here */
                t = deftok(&path[i], MAXPATH, infile[inclevel]);
                if (is_stmt_ending(t))
                    break;
            }
            path[i] = EOS;
            push_file_stack(path);
        }
        /* close include and pop file name stack */
        pop_file_stack();
    }

    /* in case called again after input ended */
    token[0] = EOF;
    token[1] = EOS;
    tok = EOF;
    return(tok);
}

/* Like `get_token()', but skip any leading blanks. */
int
get_nonblank_token(char token[], int toksiz)
{
    int tok;
    skip_blanks(infile[inclevel]);
    tok = get_token(token, toksiz);
    return(tok);
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
