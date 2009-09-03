#ifndef RAT4_LEX_H
#  define RAT4_LEX_H 1

/*
 * L E X I C A L  A N A L Y S E R  S Y M B O L S
 *
 */

/* 
 * WARNING: the values of all these three macros must be smaller than
 * the numerical value of any ascii character that might be used in the
 * input.
 */
#define LETTER          1
#define DIGIT           2
#define ALPHA           3

#define LEXBREAK        4
#define LEXDIGITS       5
#define LEXDO           6
#define LEXELSE         7
#define LEXFOR          8
#define LEXIF           9
#define LEXNEXT         10
#define LEXOTHER        11
#define LEXREPEAT       12
#define LEXUNTIL        13
#define LEXWHILE        14
#define LEXRETURN       15
#define LEXEND          16
#define LEXSTOP         17
#define LEXSTRING       18
#define LEXSWITCH       19
#define LEXCASE         20
#define LEXDEFAULT      21

#endif
/* vim: set ft=c ts=4 sw=4 et : */
