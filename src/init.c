/* init.c -- this file is part of C Ratfor.
 * Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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

/* Define here ALL the global variables declared in "rat4-global.h".
   This is REQUIRED by C++ compilers. */

/* text of current subprogram (function or subroutine) name */
char current_subprogram_name[MAXFUNCNAME];

/* true if comments should be kept, false otherwise */
bool keep_comments;

/* type of current subprogram (function, subroutine, everithing else) */
enum subprg_t current_subprogram_type;

/* level of file inclusion; init = 1 */
int inclevel;

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

    inclevel = 0;               /* count of inclusion levels */
    lineno[0] = 1;              /* line count of first file */
    filename[0] = x_filename;   /* filename of first file */
    infile[0] = x_infile;       /* file handle of first file */
    keep_comments = x_keep_comments;
    current_subprogram_name[0] = EOS;
    current_subprogram_type = SUBPRG_NONE;
    set_starting_label(x_startlab);
}

/* vim: set ft=c ts=4 sw=4 et : */
