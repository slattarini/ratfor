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

/* subroutines for code generation */

void brknxt(int, int [], int [], int);
void outcon(int);
void labelc(char []);
void otherstmt(char lexstr[]);

void docode(int *);
void dostat(int);

void for_code(int *);
void for_end(int);

void ifcode(int *);
void ifthenc(void);
void ifend(void);
void elseifc(void);
void ifngo(int);

void repcode(int *);
void repeats(int);
void untilcode(int);

void while_code(int *);
void while_end(int);

void retcode(void);
void cascode(int, int);
void swend(int);
void strdcl(void);
void swcode(int *);

void verbatim(void);

END_C_DECLS
#endif
/* vim: set ft=c ts=4 sw=4 et : */
