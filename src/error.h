/* $Id$ */
#ifndef RAT4_ERROR_H 
#  define RAT4_ERROR_H 1
void error(char *, char *);
void synerr(char *);
void synerr_eof(void);
void baderr(char *);
extern int exit_status;
#endif
/* vim: set ft=c ts=4 sw=4 et : */
