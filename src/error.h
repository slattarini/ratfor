#ifndef RAT4_ERROR_H 
#  define RAT4_ERROR_H 1
BEGIN_C_DECLS

#include <errno.h>  /* for the `errno' variable */
#include <string.h> /* for `strerror()' function */

extern int exit_status;

void fatal(const char *, ...);
void synerr(const char *, ...);
void synerr_eof(void);
void synerr_include(const char *, ...);
void synerr_fatal(const char *, ...);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
