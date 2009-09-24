#ifndef RAT4_GLOBAL_H
#  define RAT4_GLOBAL_H 1

#include "rat4-common.h"

/* text of current function name */
C_DECL char current_function_name[MAXFUNCNAME];

/* true if comments should be kept, false otherwise */
C_DECL bool keep_comments;

/*  level of file inclusion; init = 1 */
C_DECL int level;

/* stack of inluded files: line nuber, file name, file pointer */
C_DECL int lineno[NFILES];
C_DECL FILE *infile[NFILES];
C_DECL const char *filename[NFILES];

#endif
/* vim: set ft=c ts=4 sw=4 et : */
