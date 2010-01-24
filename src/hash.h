/* hash.h -- this file is part of C Ratfor.
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

#ifndef RAT4_LOOKUP_H
#  define RAT4_LOOKUP_H 1

#define HASHMAX 100 /* size of hashtable */

BEGIN_C_DECLS

/* Table lookup routines structure and definitions. */

/* basic table entry */
struct hashlist {
    const char *name;
    const char *def;
    struct hashlist *next; /* next in chain */
};

struct hashlist *hash_install(const char *, const char *);
struct hashlist *hash_lookup(const char *);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
