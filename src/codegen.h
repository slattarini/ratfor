/* codegen.h -- this file is part of C Ratfor.
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

#ifndef RAT4_CODEGEN_H
#  define RAT4_CODEGEN_H 1
BEGIN_C_DECLS

/***  Subroutines and macros for code generation  ***/

/* verbatim - copy directly to output until the next newline character */
#define verbatim() output_verbatim(infile[inclevel])

void if_code(int *);
void elseif_code(void);
void if_end(void);

void do_code(int *);
void do_end(int);

void for_code(int *);
void for_end(int);

void repeat_code(int *);
void repeat_end(int);
void until_code(int);

void while_code(int *);
void while_end(int);

void break_and_next_code(int, int [], int [], int);

void case_code(int, int);
void switch_end(int);
void switch_code(int *);

void label_code(char []);
void return_code(void);
void otherstmt(char lexstr[]);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
