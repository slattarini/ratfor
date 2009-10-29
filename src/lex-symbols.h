#ifndef RAT4_LEX_H
#  define RAT4_LEX_H 1

/*
 * L E X I C A L  A N A L Y S E R  S Y M B O L S
 *
 */

/* 
 * WARNING: the values of all these four macros must be smaller than the
 * numerical value of any ascii character that might be used in the input.
 */
#define TOKT_LETTER      1
#define TOKT_DIGIT       2
#define TOKT_ALPHA       3
#define TOKT_STRING      4
#define TOKT_OPEREXP     5
#define TOKT_OPERSTRCAT  6

#define LEXBREAK        10
#define LEXDIGITS       11
#define LEXDO           12
#define LEXELSE         13
#define LEXFOR          14
#define LEXIF           15
#define LEXNEXT         16
#define LEXOTHER        17
#define LEXREPEAT       18
#define LEXUNTIL        19
#define LEXWHILE        20
#define LEXRETURN       21
#define LEXEND          22
#define LEXSTOP         23
#define LEXSWITCH       24
#define LEXCASE         25
#define LEXDEFAULT      26

#endif
/* vim: set ft=c ts=4 sw=4 et : */
