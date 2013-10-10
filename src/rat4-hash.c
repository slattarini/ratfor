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
#include "error.h"

#define HASHMAX 100 /* size of hashtable */

/* basic table entry */
struct hashlist {
    const char *key;
    const char *val;
    struct hashlist *next; /* next in chain */
};

static struct hashlist *hashtab[HASHMAX];

STATIC_TESTABLE_FUNC int
hash(const char *key)
{
    int i;

    for (i = 0; *key != '\0'; key++)
        i += *key;
    return (i % HASHMAX);
}

const char *
hash_lookup(const char *key)
{
    struct hashlist *np;

    for (np = hashtab[hash(key)]; np != NULL; np = np->next)
        if (STREQ(key, np->key))
            return(np->val);  /* found     */
    return(NULL);             /* not found */
}

void
hash_install(const char *key, const char *val)
{
    int hashed_key;
    struct hashlist *hp;
    const char *oldval;

    if ((oldval = hash_lookup(key)) == NULL) { /* not found */
        /* cast needed to avoid errors with c++ compilers */
        hp = (struct hashlist *) malloc(sizeof(*hp));
        if (hp == NULL)
            fatal("out of memory");
        if ((hp->key = strdup(key)) == NULL)
            fatal("out of memory");
        if ((hp->val = strdup(val)) == NULL)
            fatal("out of memory");
        hashed_key = hash(hp->key);
        hp->next = hashtab[hashed_key];
        hashtab[hashed_key] = hp;
    } else { /* found, must deallocate older value to avoid memory leak */
        /* cast needed to avoid compiler warning */
        free((void *)oldval);
    }
}

/* vim: set ft=c ts=4 sw=4 et : */
