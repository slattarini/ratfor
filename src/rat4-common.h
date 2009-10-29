/* Common macros & includes for the ratfor preprocessor */
#ifndef RAT4_COMMON_H
#  define RAT4_COMMON_H 1

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define E_USAGE 2

#define ACCENT      '`' /* backtick */
#define AND         '&'
#define ATSIGN      '@'
#define BACKSLASH   '\\'
#define BACKTICK    ACCENT
#define BANG        '!'
#define BAR         '|'
#define BLANK       ' '
#define CARET       '^'
#define COLON       ':'
#define COMMA       ','
#define DIG0        '0'
#define DIG1        '1'
#define DIG2        '2'
#define DIG3        '3'
#define DIG4        '4'
#define DIG5        '5'
#define DIG6        '6'
#define DIG7        '7'
#define DIG8        '8'
#define DIG9        '9'
#define DOLLAR      '$'
#define DQUOTE      '"'
#define EOS         '\0'
#define EQUALS      '='
#define ESCAPE      ATSIGN
#define FKNEWLINE   '\007'  /* "fake" newline */
#define GREATER     '>'
#define LBRACE      '{'
#define LBRACK      '['
#define LESS        '<'
#define LPAREN      '('
#define MINUS       '-'
#define NEWLINE     '\n'
#define NOT         '~'
#define OR          BAR
#define PERCENT     '%'
#define PERIOD      '.'
#define PLUS        '+'
#define QMARK       '?'
#define RBRACE      '}'
#define RBRACK      ']'
#define RPAREN      ')'
#define SEMICOL     ';'
#define SHARP       '#'
#define SLASH       '/'
#define SQUOTE      '\''
#define STAR        '*'
#define TAB         '\t'
#define TILDE       '~'
#define UNDERLINE   '_'

#define RADIX PERCENT /* `%' indicates alternate radix for numbers */

#define MAXFUNCNAME         128     /* max funcname size */
#define MAXTOK              1024    /* max chars in a token */
#define MAX_INCLUDE_DEPTH   30      /* max depth of file inclusion */

#define STREQ(x, y) (strcmp((x),(y)) == 0)

/* help to avoid irrelevant compiler warnings w.r.t. signed/unsigned
 * conversion */
#define SSTRLEN(s) ((int) strlen(s))

/* simulate booleans if not present */
#ifdef HAVE_STDBOOL_H
#  include <stdbool.h>
#else /* !HAVE_STDBOOL_H */
#  ifndef __cplusplus
#    ifdef HAVE__BOOL
       typedef _Bool bool;
#      define false 0
#      define true 1
#    else /* !HAVE__BOOL */
       typedef enum boolvars { false = 0, true = 1 } bool;
#    endif /* HAVE__BOOL */
#  endif /* !__cplusplus */
#endif /* HAVE_STDBOOL_H */

/*
 * We cannot use the <ctype.h> macros like `isalpha()', `isblank()',
 * `isdigit()', etc, since they are locale-dependent.  So we define our
 * own versions, tailored to our needs.
 */

static inline bool
is_upper(int c)
{
    return((c >= 'A' && c <= 'Z') ? true : false);
}

static inline bool
is_lower(int c)
{
    return((c >= 'a' && c <= 'z') ? true : false);
}

static inline bool
is_digit(int c)
{
    return((c >= DIG0 && c <= DIG9) ? true : false);
}

static inline bool
is_strict_alpha(int c)
{
    return((is_upper(c) || is_lower(c)) ? true : false);
}

static inline bool
is_strict_alnum(int c)
{
    return((is_strict_alpha(c) || is_digit(c)) ? true : false);
}

static inline bool
is_rat4_alpha(int c)
{
    if (is_strict_alpha(c) || c == UNDERLINE || c == DOLLAR || c == PERIOD)
        return true;
    else
        return false;
}

static inline bool
is_rat4_alnum(int c)
{
    return((is_rat4_alpha(c) || is_digit(c)) ? true : false);
}

static inline bool
is_blank(int c)
{
    return((c == BLANK || c == TAB) ? true : false);
}

static inline bool
is_strict_newline(int c)
{
    return((c == NEWLINE) ? true : false);
}

static inline bool
is_newline(int c)
{
    /* hack needed to keep the count of line numbers correct even when
     * expansion of multiline macros (defined through the `define'
     * builtin) is involved */
    return((c == NEWLINE || c == FKNEWLINE) ? true : false);
}

static inline bool
is_stmt_ending(int c)
{
    return((is_newline(c) || c == SEMICOL) ? true : false);
}


#endif
/* vim: set ft=c ts=4 sw=4 et : */
