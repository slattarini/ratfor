#ifndef RAT4_TOKENIZER_H
#  define RAT4_TOKENIZER_H 1

#include "tok-types.h"

BEGIN_C_DECLS

int get_token(char[], int);
int get_nonblank_token(char[], int);
int lex(char[], int);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
