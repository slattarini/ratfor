/* Common macros for the ratfor preprocessor */
#ifndef RAT4_RATDEF_H
#  define RAT4_RATDEF_H 1

#include <string.h>
#define STREQ(x, y) (strcmp((x),(y)) == 0)

#define ACCENT  96
#define AND     38
#define APPEND  /*empty*/
#define ATSIGN  64
#define BACKSLASH  92
#define BANG    33
#define BAR     124
#define BLANK   32
#define CARET   94
#define COLON   58
#define COMMA   44
#define CRLF    13
#define DIG0    48
#define DIG1    49
#define DIG2    50
#define DIG3    51
#define DIG4    52
#define DIG5    53
#define DIG6    54
#define DIG7    55
#define DIG8    56
#define DIG9    57
#define DOLLAR  36
#define DQUOTE  34
#define EOS     0
#define EQUALS  61
#define ESCAPE  ATSIGN
#define GREATER 62
#define LBRACE  123
#define LBRACK  91
#define LESS    60
#define LPAREN  40
#define MINUS   45
#define NEWLINE 10
#define NOT     126
#define OR      BAR /* same as | */
#define PERCENT 37
#define PERIOD  46
#define PLUS    43
#define QMARK   63
#define RBRACE  125
#define RBRACK  93
#define RPAREN  41
#define SEMICOL 59
#define SHARP   35
#define SLASH   47
#define SQUOTE  39
#define STAR    42
#define TAB     9
#define TILDE   126
#define UNDERLINE  95

/* simulate booleans */
#define NO      0
#define YES     1

#define MAXCHARS        10      /* characters for outnum */
#define MAXDEF          2048    /* max chars in a defn */
#define MAXSWITCH       300     /* max stack for switch statement */
#define CUTOFF          3       /* min number of cases necessary to generate
                                 * a dispatch table */
#define DENSITY         2
#define MAXFORSTK       200     /* max space for for reinit clauses */
#define MAXNAME         64      /* file name size in gettok */
#define MAXSTACK        100     /* max stack depth for parser */
#define MAXTOK          1024    /* max chars in a token */
#define NFILES          7       /* max depth of file inclusion */

#define RADIX PERCENT /* `%' indicates alternate radix for numbers */

#endif
/* vim: set ft=c ts=4 sw=4 et : */
