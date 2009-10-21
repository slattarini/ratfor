#ifndef RAT4_IO_H 
#  define RAT4_IO_H 1

#include <stdio.h> /* for `*FILE' and `EOF' (at least) */

BEGIN_C_DECLS

int ngetch(FILE *);
void put_back_string(const char []);
void put_back_string_cooked(const char []);
void put_back_char(char c);

void outch(char c);
void outstr(const char []);
void outtab(void);
void outdon(void);
void outnum(int);
void outcmnt(FILE *);
void outasis(FILE *);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
