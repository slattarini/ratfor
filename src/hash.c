/*
 * from K&R "The C Programming language"
 * Table lookup routines
 */
#include "rat4-common.h"

#include "utils.h"
#include "hash.h"

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
        if ((np->name = strsave(name)) == NULL)
            return(NULL);
        hashval = hash(np->name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else { /* found */
        /* cast needed to avoid compiler warning */
        free((void *)np->def);
    }
    if ((np->def = strsave(def)) == NULL)
        return(NULL);
    return(np);
}

/* vim: set ft=c ts=4 sw=4 et : */
