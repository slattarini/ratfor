/* Common macros & includes for the ratfor preprocessor */
#ifndef RAT4_COMMON_H
#  define RAT4_COMMON_H 1

#include <config.h>
#include <stdio.h>
#include <string.h>

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

#define MAXFUNCNAME 128     /* max funcname size */
#define MAXTOK      1024    /* max chars in a token */

#define STREQ(x, y) (strcmp((x),(y)) == 0)

/* simulate booleans */
#define NO      0
#define YES     1

static inline int
is_blank(int c)
{
    return((c == BLANK || c == TAB) ? YES : NO);
}

static inline int
is_strict_newline(int c)
{
    return((c == NEWLINE) ? YES : NO);
}

static inline int
is_newline(int c)
{
    /* hack needed to keep the count of line numbers correct even when
     * expansion of multiline macros (defined through the `define'
     * builtin) is involved */
    return((c == NEWLINE || c == FKNEWLINE) ? YES : NO);
}

static inline int
is_stmt_ending(int c)
{
    return((is_newline(c) || c == SEMICOL) ? YES : NO);
}

#endif
/* vim: set ft=c ts=4 sw=4 et : */
