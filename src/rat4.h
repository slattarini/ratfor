#ifndef RAT4_RAT4_H
#  define RAT4_RAT4_H 1
/* subroutines for code generation */

void brknxt(int, int [], int [], int);
void outcon(int);
void labelc(char []);
void otherstmt(char lexstr[]);

void docode(int *);
void dostat(int);

void forcode(int *);
void fors(int);

void ifcode(int *);
void ifthenc(void);
void ifend(void);
void elseifc(int);

void repcode(int *);
void untils(int, int);

void whilecode(int *);
void whiles(int);

void retcode(void);
void cascode(int, int);
void swend(int);
void strdcl(void);
void swcode(int *);

#endif
/* vim: set ft=c ts=4 sw=4 et : */
