/* error.h -- this file is part of C Ratfor.
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

#ifndef RAT4_ERROR_H 
#  define RAT4_ERROR_H 1

#include <errno.h>  /* for the `errno' variable */
#include <string.h> /* for `strerror()' function */

BEGIN_C_DECLS

extern int exit_status;

void fatal(const char *, ...);
void synerr(const char *, ...);
void synerr_eof(void);
void synerr_include(const char *, ...);
void synerr_fatal(const char *, ...);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
