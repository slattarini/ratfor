#ifndef RAT4_IO_H 
#  define RAT4_IO_H 1

int ngetch(char *, FILE *);
void pbstr(const char []);
void putbak(char c);

void outch(char c);
void outstr(const char []);
void outtab(void);
void outdon(void);
void outcmnt(FILE *);
void outasis(FILE *);

#endif
/* vim: set ft=c ts=4 sw=4 et : */
