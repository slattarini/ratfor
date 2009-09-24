#ifndef RAT4_UTILS_H
#  define RAT4_UTILS_H 1
BEGIN_C_DECLS

char *strsave(const char *);
int ctoi(const char[], int *);
int itoc(int, char[], int size);
void scopy(const char[], int, char[], int);
bool look(const char[], char[]);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
