#include "rat4-common.h"
#include "rat4-global.h"

#include "error.h"  /* for `fatal()' */
#include "xopen.h"
#include "labgen.h" /* we need to set the starting label */
#include "init.h"

/*
 *   I N I T I A L I Z E   P R E P R O C E S S O R   S T A T E
 */

#ifdef __cplusplus

/* DEFINE HERE global variables DECLARED IN "rat4-global.h".
   This is REQUIRED by C++ compilers. */

/* text of current function name */
char current_function_name[MAXFUNCNAME];

/* true if comments should be kept, false otherwise */
bool keep_comments;

/*  level of file inclusion; init = 1 */
int level;

/* stack of inluded files: line nuber, file name, file pointer */
int lineno[MAX_INCLUDE_DEPTH];
FILE *infile[MAX_INCLUDE_DEPTH];
const char *filename[MAX_INCLUDE_DEPTH];

#endif /* C++ */


/* use `x_foo' names to avoid colliding with similarly named global
 * variables */
C_DECL void
init(int x_startlab, int x_keep_comments, const char *x_filename)
{
    FILE *x_infile;

    x_infile = xopen(x_filename, IO_MODE_READ, fatal);
    if (STREQ(x_filename, "-"))
        x_filename = "(stdin)"; /* this is clearer in error messages */

    level = 0;                  /* file control */
    lineno[0] = 1;              /* line count of first file */
    filename[0] = x_filename;   /* filename of first file */
    infile[0] = x_infile;       /* file handle of first file */
    keep_comments = x_keep_comments;
    current_function_name[0] = EOS;
    set_starting_label(x_startlab);
}

/* vim: set ft=c ts=4 sw=4 et : */
