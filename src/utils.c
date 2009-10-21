#include "rat4-common.h"

#include "utils.h"
#include "lookup.h"

#include <ctype.h>


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

/* Convert (signed) integer n to a string, saving it in `str'.
 * Return the legnth of the converted string. */
int
itoc(int n, char str[], int size)
{
    int i, j, k, sign;
    char c;

    sign = (n < 0 ? -1 : 1);
    n *= sign; /* turn n into its absolute value */
    i = 0;
    do {
        str[i++] = n % 10 + DIG0;
    } while ((n /= 10) > 0 && i < size-2);
    if (sign < 0 && i < size - 1)
        str[i++] = '-';
    str[i] = EOS;
    /* reverse the string and plug it back in
       NOTE: the cast to int avoid spurious compiler warnings */
    for (j = 0, k = ((int) strlen(str)) - 1; j < k; j++, k--) {
        c = str[j];
        str[j] = str[k];
        str[k] = c;
    }
    return(i-1);
}

/* scopy - copy string at from[i] to to[j] */
void
scopy(const char from[], int i, char to[], int j)
{
    int k1, k2;

    k2 = j;
    for (k1 = i; from[k1] != EOS; k1++) {
        to[k2] = from[k1];
        k2++;
    }
    to[k2] = EOS;
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
