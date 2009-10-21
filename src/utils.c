#include "rat4-common.h"
#include "utils.h"
#include "lookup.h"

/*
 * U T I L I T Y  R O U T I N E S
 */

BEGIN_C_DECLS

/* strsave - save string s somewhere, by malloc'ing memory */
char
*strsave(const char *s)
{
    char *p;
    /* cast needed to avoid errors with c++ compilers */
    if ((p = (char *) malloc(strlen(s)+1)) != NULL)
        strcpy(p, s); /* XXX strncpy? */
    return(p);
}

/* scopy - copy string at from[i] to to[j] */
void
scopy(const char from[], int i, char to[], int j)
{
    int k1, k2;
    for (k1 = i, k2 = j; (to[k2] = from[k1]) != EOS; k1++, k2++)
        /* empty body */;
}

/* look - look-up definition of `name', copy it in defn */
bool
look(const char name[], char defn[])
{
    struct hashlist *p;
    if ((p = lookup(name)) == NULL)
        return(false);
    strcpy(defn, p->def); /* XXX potential overflow here! */
    return(true);
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
