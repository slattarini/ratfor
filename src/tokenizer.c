/* Get tokens for the ratfor preprocessor. */

#include "rat4-common.h"

#include "tokenizer.h"
#include "io.h"
#include "xopen.h"
#include "error.h"
#include "hash.h"
#include "rat4-global.h"

#define MAXPATH     1024    /* max length of the name of a file included */
#define MAXDEFLEN   2048    /* max length of a ratfor macro's definition */

/* NOTE: due to implementation details, it is pointless to have MAXDEFLEN
         here greater than BUFSIZE in io.c */

/*
 * PRIVATE VARIABLES.
 */

static const char KEYWORD_INCLUDE[] = "include";
static const char KEYWORD_FUNCTION[] = "function";
static const char KEYWORD_DEFINE[] = "define";


/*
 * PRIVATE FUNCTIONS.
 */

/*
 * Convert integer n to a string, upd to size - 1 characters, and save it
 * in buffer str[].  Negative integers are supported.  Return the lenght
 * of the converted string.
 */
static int
integer_to_string(int n, char str[], int size)
{
    int i, j, k, sign;
    char c;

    sign = (n < 0 ? -1 : 1);
    n *= sign; /* turn n into its absolute value */
    i = 0;
    do {
        str[i++] = n % 10 + DIG0;
    } while ((n /= 10) > 0 && i < size-2);
    if (sign < 0 && i < size - 1)
        str[i++] = '-';
    str[i] = EOS;
    /* reverse the string and plug it back in */
    for (j = 0, k = SSTRLEN(str) - 1; j < k; j++, k--) {
        c = str[j];
        str[j] = str[k];
        str[k] = c;
    }
    return(i-1);
}

/* Look-up definition of name[] in user-defined macros. If it's not found,
   return false, else and copy the definition in defn[] and return true. */
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

static void
skip_blanks(FILE *fp)
{
    char c;
    for (c = ngetch(fp); is_blank(c); c = ngetch(fp))
        /* skip blank characters */;
    put_back_char(c);
}

/* Convert relational shorthands (e.g. `&&' --> `.and.', '<' --> '.lt.'),
 * write the resulting string in token[] itslef.  Return the lenght of
 * the resulting string. */
static int
convert_relation_shortand(char token[], int toksiz, FILE *fp)
{
#ifdef NDEBUG
    /* TODO: assert toksiz >= 6 */
#else
    /* pacify compiler warnings */
    (void) toksiz;
#endif
    token[0] = ngetch(fp);
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
                token[1] = EOS;
                goto out;
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
            abort();
            break;
    }  /* switch(token[0]) */
    token[0] = PERIOD;
out:
    return(TOKT_RELATN);
}

/* read alphanumeric token from fp, save it in lexstr, return the lenght of
 * the token read */
static int
get_alphanumeric_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int i;
    for (i = 0; i < toksiz - 1; i++) {
        lexstr[i] = ngetch(fp);
        if (is_rat4_alnum(lexstr[i]))
            continue;
        put_back_char(lexstr[i--]);
        break;
    }
    if (i >= toksiz - 1) {
        synerr("alphanumeric token too long.");
        lexstr[i] = EOS;
    } else {
        lexstr[++i] = EOS;
    }
    return(TOKT_ALPHA);
}

/* read numerical token from fp, save it in lexstr, return the lenght of
 * the token read */
static int
get_numerical_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int i, c, c2, b, n;
    b = 0; /* in case alternate base number */
    for (i = 0; i < toksiz - 1; i++) {
        lexstr[i] = ngetch(fp);
        if (!is_digit(lexstr[i]))
            break;
        b = 10*b + (lexstr[i] - DIG0);
    }
    if (lexstr[i] == RADIX && b >= 2 && b <= 36) {
        /* n%ddd... */
        for (n = 0; ; n = b*n + c) {
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
        i = integer_to_string(n, lexstr, toksiz);
    } else {
        put_back_char(lexstr[i--]);
    }
    if (i >= toksiz - 1) {
        synerr("numeric constant too long.");
        lexstr[i] = EOS;
    } else {
        lexstr[++i] = EOS;
    }
    lexstr[i] = EOS;
    return(TOKT_DIGITS);
}

/* read "quoted string" token from fp, save it in lexstr, return the
 * lenght of the token read */
static int
get_quoted_string_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    /* TODO: handle escaped quotes inside a string */

    int i, quote_char;

    /*TODO: assert quote_char == " || quote_char == ' ??? */
    quote_char = lexstr[0] = ngetch(fp);

    for (i = 1; (lexstr[i] = ngetch(fp)) != quote_char; i++) {
        if (is_newline(lexstr[i])) {
            synerr("missing quote.");
        } else if (i >= toksiz - 2) {
            /* we might still have space for closing quote, but will
               surely lack space for string terminating character EOF */
            synerr("string too long.");
        } else {
            continue; /* no err, go to next iteration */
        }
        put_back_char(lexstr[i]);
        lexstr[i] = lexstr[0];
        break;
    }
    lexstr[++i] = EOS;
    return(TOKT_STRING);
}


/* read an operator token (accounting for multi-charcater operators), save
 * it in lexstr, and return the lenght of the token read */
static int
get_operator_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int i, c, nc;
#ifdef NDEBUG
    /* TODO: assert toksiz >= 2 */
#else
    /* pacify compiler warnings */
    (void) toksiz;
#endif
    i = 0;
    c = lexstr[0] = ngetch(fp);
    if (c == STAR || c == SLASH) {
        nc = ngetch(fp); /* peek next character */
        if (c == nc) { /* fortran `**' or `//' operator */
            lexstr[++i] = c;
        } else {
            put_back_char(nc);
        }
    }
    lexstr[++i] = EOS;
    return(TOKT_OPERATOR);
}

/* read a non-alphanumeric token from fp (accounting for composed tokens),
 * save it in lexstr, save its type in *ptok, return the lenght of the
 * token read */
static int
get_non_alphanumeric_raw_token(char lexstr[], int toksiz, FILE *fp)
{
    int tok;
    /* TODO: assert toksiz >= 2 */
    lexstr[0] = ngetch(fp);
    put_back_char(lexstr[0]);
    switch (lexstr[0]) {
        case PERCENT:
        case SHARP:
            /* comment or verbatim string */
            (void) ngetch(fp); /* TODO: assert == lexstr[0] */
            if (lexstr[0] == SHARP) {
                dispatch_comment(fp);
            } else {
                outasis(fp); /* copy direct to output if % */
            }
            tok = lexstr[0] = NEWLINE;
            lexstr[1] = EOS;
            break;
        case GREATER:
        case LESS:
        case EQUALS:
        case NOT:
        case BANG:
        case CARET:
        case AND:
        case OR:
            /* maybe a ratfor relational shorthand */
            tok = convert_relation_shortand(lexstr, toksiz, fp);
            break;
        case PLUS:
        case MINUS:
        case STAR:
        case SLASH:
            /* fortran opertor */
            tok = get_operator_raw_token(lexstr, toksiz, fp);
            break;
        default:
            /* everything else */
            (void) ngetch(fp); /* TODO: assert == lexstr[0] */
            tok = lexstr[0];
            lexstr[1] = EOS;
            break;
    }
    return(tok);
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
    int c;
    c = lexstr[0] = ngetch(fp);
    if (is_blank(c)) {
        lexstr[0] = BLANK;
        while (is_blank(c)) /* compress many blanks to one */
            c = ngetch(fp);
        if (c == PERCENT) {
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
    put_back_char(c); /* so that we can read back the whole token */
    if (is_rat4_alpha(c)) {
        tok = get_alphanumeric_raw_token(lexstr, toksiz, fp);
    } else if (is_digit(c)) {
        tok = get_numerical_raw_token(lexstr, toksiz, fp);
    } else if (c == SQUOTE || c == DQUOTE) {
        tok = get_quoted_string_raw_token(lexstr, toksiz, fp);
    } else {
        tok = get_non_alphanumeric_raw_token(lexstr, toksiz, fp);
    }
    return(tok);
}

/* Parse definition of ratfor macro. If an error is detected, stop ratfor
 * with a suitable error message, alse save macro name (in `name[]') and
 * macro definition (in `def[]'). */
static void
getdef(char name[], int namesiz, char def[], int defsiz, FILE *fp)
{
    int i, j, nlpar, t, t2;
    bool defn_with_paren;
    char ptoken[MAXTOK]; /* temporary buffer for token */

    /* hackish internal macro to avoid code duplication below */
#   define EXTEND_DEFN_WITH_TOKEN_(token_) \
        for (j = 0; token_[j] != EOS; i++, j++) { \
            if (i >= defsiz) \
                synerr_fatal("definition too long."); \
            def[i] = token_[j]; \
        }

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
    } else if (t2 != TOKT_ALPHA) {
        synerr_fatal("non-alphanumeric name.");
    }
    skip_blanks(fp);
    if (!defn_with_paren) { /* define name def */
        i = 0;
        for (;;) {
            t2 = get_raw_token(ptoken, MAXTOK, fp);
            if (is_newline(t2) || t2 == EOF) {
                put_back_string(ptoken);
                break;
            }
            EXTEND_DEFN_WITH_TOKEN_(ptoken);
        }
        def[i] = EOS;
    } else { /* define (name, def) */
        if (get_raw_token(ptoken, MAXTOK, fp) != COMMA)
            synerr_fatal("missing comma in define.");
        /* else got (name, */
        for (i = 0, nlpar = 0; nlpar >= 0; /* empty clause */) {
            t2 = get_raw_token(ptoken, MAXTOK, fp);
            if (t2 == EOF)
                synerr_fatal("missing right paren.");
            else if (t2 == LPAREN)
                nlpar++;
            else if (t2 == RPAREN)
                nlpar--;
            EXTEND_DEFN_WITH_TOKEN_(ptoken);
        }
        /* TODO: assert def[i-1] == ')' */
        def[i-1] = EOS;
    }
    /* get rid of temporary internal macro */
#   undef EXTEND_DEFN_WITH_TOKEN_
}

/* Get token, expanding macro calls and processing macro definitions. */
/* XXX: clearer name? */
static int
deftok(char token[], int toksiz, FILE *fp)
{
    char tkdefn[MAXDEFLEN];
    int t;

    while ((t = get_raw_token(token, toksiz, fp)) != EOF) {
        if (t != TOKT_ALPHA) {
            break; /* non-alpha */
        } else if (STREQ(token, KEYWORD_DEFINE)) {
            /* get definition for token, save it in tkdefn */
            getdef(token, toksiz, tkdefn, MAXDEFLEN, fp);
            hash_install(token, tkdefn);
        } else if (!defn_lookup(token, tkdefn)) {
            break; /* undefined */
        } else {
            /* Push replacement onto input, with newlines properly
             * substituted by "fake newlines". This hack is needed
             * to keep the count of line in input correct even if
             *expansion of multiline macros is involved. */
            put_back_string_cooked(tkdefn);
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
                else if (t != TOKT_ALPHA)
                    synerr("invalid function name `%s'",
                           current_function_name);
                put_back_char(BLANK);
                return(tok);
            }
            if (!STREQ(token, KEYWORD_INCLUDE))
                return(tok);
            /* deal with file inclusion */
            for (i = 0; ; i = SSTRLEN(path)) {
                if (i >= MAXPATH)
                    synerr_fatal("name of included file too long.");
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
