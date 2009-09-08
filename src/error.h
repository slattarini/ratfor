#ifndef RAT4_ERROR_H 
#  define RAT4_ERROR_H 1
void fatal(const char *, ...);
void synerr(const char *, ...);
void synerr_eof(void);
void synerr_include(const char *, ...);
void synerr_fatal(const char *, ...);
extern int exit_status;
#endif
/* vim: set ft=c ts=4 sw=4 et : */
