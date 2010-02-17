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

/* Tell if a macro definition is being scanned.  Used to share state between
 * `getdef' and functions of the `get_raw_token' family. */
C_DECL bool reading_parenthesized_macro_definition;

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
