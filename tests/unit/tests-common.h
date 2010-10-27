/* test-macros.h -- Common macros used by ratfor unit tests.
 * Copyright (C) 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
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

#include <stdio.h>
#include <stdlib.h>

#ifndef ASSERT_STREAM
# define ASSERT_STREAM stderr
#endif

#define ASSERT(expr) \
    do { \
        if (!(expr)) { \
          fprintf(ASSERT_STREAM, "%s:%d: assertion failed\n", \
                  __FILE__, __LINE__); \
          fflush(ASSERT_STREAM); \
          abort(); \
        } \
    } while (0)

/* vim: set ft=c ts=4 sw=4 et : */
