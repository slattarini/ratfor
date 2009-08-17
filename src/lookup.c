/* $Id$ */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lookup.h"

static struct hashlist *hashtab[HASHMAX];

/*
 * from K&R "The C Programming language"
 * Table lookup routines
 */

/*
 * strsave - save string s somewhere
 *
 */
static char
*strsave(char *s)
{
    char *p;

    if ((p = malloc(strlen(s)+1)) != NULL)
        strcpy(p, s);
    return(p);
}

/*
 * hash - for a hash value for string s
 *
 */
static int
hash(char *s)
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
struct hashlist
*lookup(char *s)
{
    struct hashlist *np;

    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (strcmp(s, np->name) == 0)
            return(np); /* found     */
    return(NULL);       /* not found */
}

/*
 * install - install a string name in hashtable and its value def
 *
 */
struct hashlist
*install(char *name, char *def)
{
    int hashval;
    struct hashlist *np, *lookup();

    if ((np = lookup(name)) == NULL) { /* not found.. */
        np = (struct hashlist *) malloc(sizeof(*np));
        if (np == NULL)
            return(NULL);
        if ((np->name = strsave(name)) == NULL)
            return(NULL);
        hashval = hash(np->name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    } else /* found..     */
        free(np->def); /* free prev.  */
    if ((np->def = strsave(def)) == NULL)
        return(NULL);
    return(np);
}

/* vim: set ft=c ts=4 sw=4 et : */
