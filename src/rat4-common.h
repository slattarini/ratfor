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
is_newline(int c)
{
    return((c == NEWLINE) ? YES : NO);
}

static inline int
is_stmt_ending(int c)
{
    return((is_newline(c) || c == SEMICOL) ? YES : NO);
}

#define MAXCHARS        10      /* characters for outnum */
#define MAXDEF          2048    /* max chars in a defn */
#define MAXSWITCH       300     /* max stack for switch statement */
#define MAXFORSTK       200     /* max space for for reinit clauses */
#define MAXNAME         64      /* filename and funcname size in gettok */
#define MAXSTACK        100     /* max stack depth for parser */
#define MAXTOK          1024    /* max chars in a token */
#define NFILES          7       /* max depth of file inclusion */

#define RADIX PERCENT /* `%' indicates alternate radix for numbers */

#endif
/* vim: set ft=c ts=4 sw=4 et : */
