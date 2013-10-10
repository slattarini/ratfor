/* io.h -- this file is part of C Ratfor.
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

#ifndef RAT4_IO_H 
#  define RAT4_IO_H 1

#include <stdio.h> /* for `*FILE' and `EOF' (at least) */

int ngetch(FILE *);
void put_back_string(const char []);
void put_back_string_cooked(const char []);
void put_back_char(char c);

void outch(char c);
void outstr(const char []);
void outtab(void);
void outdon(void);
void outnum(int);
void output_comment(FILE *);
void output_verbatim(FILE *);

#endif
/* vim: set ft=c ts=4 sw=4 et : */
