#ifndef RAT4_LEX_H
#  define RAT4_LEX_H 1

/*
 *  T O K E N I Z E R  S Y M B O L S
 *
 */

/* 
 * WARNING: the values of all these five macros must be smaller than the
 * numerical value of any ascii character that might be used in the input.
 */
#define TOKT_DIGITS      1
#define TOKT_ALPHA       2
#define TOKT_STRING      3
#define TOKT_OPERATOR    4
#define TOKT_RELATN      5

#endif
/* vim: set ft=c ts=4 sw=4 et : */
