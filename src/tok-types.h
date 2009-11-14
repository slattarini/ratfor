#ifndef TOK_TYPES_H
#  define TOK_TYPES_H 1

/*
 *  T O K E N I Z E R  S Y M B O L S
 *
 */

/* 
 * WARNING: the values of all these macros must be smaller than the
 * numerical value of any ascii character that might be used in the
 * input.
 */
#define TOKT_DIGITS      1
#define TOKT_ALPHA       2
#define TOKT_STRING      3
#define TOKT_RELATN      4
#define LEXBREAK        10
#define LEXDO           11
#define LEXELSE         12
#define LEXFOR          13
#define LEXIF           14
#define LEXNEXT         15
#define LEXREPEAT       16
#define LEXUNTIL        17
#define LEXWHILE        18
#define LEXRETURN       19
#define LEXEND          20
#define LEXSTOP         21
#define LEXSWITCH       22
#define LEXCASE         23
#define LEXDEFAULT      24

#endif
/* vim: set ft=c ts=4 sw=4 et : */
