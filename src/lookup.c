/*
 * from K&R "The C Programming language"
 * Table lookup routines
 */
#include "rat4-common.h"

#include <stdlib.h>

#include "utils.h"
#include "lookup.h"

static struct hashlist *hashtab[HASHMAX];

/*
 * hash - for a hash value for string s
 *
 */
static int
hash(const char *s)
{
    int hashval;

    for (hashval = 0; *s != '\0';)
        hashval += *s++;
    return (hashval % HASHMAX);
}

/*
 * lookup - lookup for a string s in the hash table
 *
 */
struct hashlist *
lookup(const char *s)
{
    struct hashlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (STREQ(s, np->name))
            return(np); /* found     */
    return(NULL);       /* not found */
}

/*
 * install - install a string name in hashtable and its value def
 *
 */
struct hashlist*
install(const char *name, const char *def)
{
    int hashval;
    struct hashlist *np;

    if ((np = lookup(name)) == NULL) { /* not found.. */
        np = malloc(sizeof(*np));
        if (np == NULL)
            return(NULL);
        if ((np->name = strsave(name)) == NULL)
            return(NULL);
        hashval = hash(np->name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* found.. */
        /* cast needed to avoid compiler warning */
        free((void *)np->def); /* free prev.. */
    }
    if ((np->def = strsave(def)) == NULL)
        return(NULL);
    return(np);
}

/* vim: set ft=c ts=4 sw=4 et : */
