/* define.h -- this file is part of C Ratfor.
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
#ifndef RAT4_DEFINE_H
#  define RAT4_DEFINE_H 1

/* NOTE: due to implementation details, it is pointless to have MAXDEFLEN
         here greater than BUFSIZE in io.c */

#define MAXDEFLEN  2048  /* max length of a ratfor macro's body */

C_DECL void get_and_install_macro_definition(void);
C_DECL const char *macro_definition_lookup(const char *);

#endif
/* vim: set ft=c ts=4 sw=4 et : */
