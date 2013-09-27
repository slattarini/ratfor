/* include.h -- this file is part of C Ratfor.
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

#ifndef RAT4_INCLUDE_H
#  define RAT4_INCLUDE_H 1

#define MAXPATH  1024  /* max length of the name of a file included */

BEGIN_C_DECLS

void push_file_stack(const char *);
void pop_file_stack(void);
void process_file_inclusion(void);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
