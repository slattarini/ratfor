/* hash.c -- this file is part of C Ratfor.
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

/* Table lookup routines. */

#include "rat4-common.h"
#include "rat4-hash.h"

static struct hashlist *hashtab[HASHMAX];

static int
hash(const char *s)
{
    int hashval;

    for (hashval = 0; *s != '\0';)
        hashval += *s++;
    return (hashval % HASHMAX);
}

C_DECL struct hashlist *
hash_lookup(const char *s)
{
    struct hashlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (STREQ(s, np->name))
            return(np); /* found     */
    return(NULL);       /* not found */
}

C_DECL struct hashlist*
hash_install(const char *name, const char *def)
{
    int hashval;
    struct hashlist *np;

    if ((np = hash_lookup(name)) == NULL) { /* not found */
        /* cast needed to avoid errors with c++ compilers */
        np = (struct hashlist *) malloc(sizeof(*np));
        if (np == NULL)
            return(NULL);
        if ((np->name = strdup(name)) == NULL)
            return(NULL);
        hashval = hash(np->name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* found */
        /* cast needed to avoid compiler warning */
        free((void *)np->def);
    }
    if ((np->def = strdup(def)) == NULL)
        return(NULL);
    return(np);
}

/* vim: set ft=c ts=4 sw=4 et : */
