#ifndef RAT4_GLOBAL_H
#  define RAT4_GLOBAL_H 1

#include "rat4-common.h"

/* type of current subprogram (function, subroutine, everything else) */
C_DECL enum subprg_t current_subprogram_type;

/* text of current subprogram (function or subroutine) name (it should
   be empty outside functions and subroutines) */
C_DECL char current_subprogram_name[MAXFUNCNAME];

/* true if comments should be kept, false otherwise */
C_DECL bool keep_comments;

/* level of file inclusion; init = 1 */
C_DECL int inclevel;

/* stack of inluded files: line nuber, file name, file pointer */
C_DECL int lineno[MAX_INCLUDE_DEPTH];
C_DECL FILE *infile[MAX_INCLUDE_DEPTH];
C_DECL const char *filename[MAX_INCLUDE_DEPTH];

#endif
/* vim: set ft=c ts=4 sw=4 et : */
