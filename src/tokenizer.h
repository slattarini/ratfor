#ifndef RAT4_TOKENIZER_H
#  define RAT4_TOKENIZER_H 1
BEGIN_C_DECLS

int get_token(char[], int);
int get_nonblank_token(char token[], int toksiz);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
