#ifndef RAT4_GLOBAL_H
#  define RAT4_GLOBAL_H 1

#include "rat4-common.h"

char current_function_name[MAXFUNCNAME];  /* text of current function name */

int keep_comments; /* YES if comments should be kept, NO otherwise */

#define NFILES 7  /* max depth of file inclusion */

int level;                      /*  level of file inclusion; init = 1            */
int lineno[NFILES];             /*  line count on input file[level]; init = 1    */
FILE *infile[NFILES];           /*  file number[level]; init infile[1] = STDIN   */
const char *filename[NFILES];   /*  filename of input file[level]                */

#endif
/* vim: set ft=c ts=4 sw=4 et : */
