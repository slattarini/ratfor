/* rat4-global.h -- this file is part of C Ratfor.
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

#ifndef RAT4_GLOBAL_H
#  define RAT4_GLOBAL_H 1

#include "rat4-common.h"

/* True if comments should be (properly) copied to output, false
 * otherwise. */
bool keep_comments;

/* Name of current subprogram (function or subroutine).  It should
 * be empty outside functions and subroutines. */
char current_subprogram_name[MAXFUNCNAME];

/* Type of current subprogram (function, subroutine, everithing else). */
enum subprg_t current_subprogram_type;

/* Level of file inclusion (init = 1). */
int inclevel;

/* Stack of included files: line nuber, file name, file pointer */
int lineno[MAX_INCLUDE_DEPTH];
FILE *infile[MAX_INCLUDE_DEPTH];
const char *filename[MAX_INCLUDE_DEPTH];

/* Tell if a macro definition is being scanned.  Used to share state
 * between function `get_macro_definition' in file `define.c' and
 * function `get_non_alphanumeric_raw_token' in file `tokenizer.c'. */
bool reading_parenthesized_macro_definition;

#endif
/* vim: set ft=c ts=4 sw=4 et : */
