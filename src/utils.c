#include "rat4-common.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "utils.h"
#include "lookup.h"


/*
 * U T I L I T Y  R O U T I N E S
 */


/*
 * strsave - save string s somewhere, by malloc'ing memory
 */
char
*strsave(const char *s)
{
    char *p;
    if ((p = malloc(strlen(s)+1)) != NULL)
        strcpy(p, s); /* XXX strncpy? */
    return(p);
}

/*
 * ctoi - convert string at in[*i] to int, increment i
 */
int
ctoi(const char in[], int *i)
{
    int k, j;

    j = *i;
    while (is_blank(in[j]))
        j++;
    for (k = 0; in[j] != EOS; j++) {
        if (in[j] < DIG0 || in[j] > DIG9)
            break;
        k = 10 * k + in[j] - DIG0;
    }
    *i = j;
    return(k);
}

/*
 * itoc - special version of itoa
 */
int
itoc(int n, char str[], int size)
{
    int i, j, k, sign;
    char c;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + DIG0;
    } while ((n /= 10) > 0 && i < size-2);
    if (sign < 0 && i < size-1)
        str[i++] = '-';
    str[i] = EOS;
    /*
     * reverse the string and plug it back in
     */
    for (j = 0, k = strlen(str) - 1; j < k; j++, k--) {
        c = str[j];
        str[j] = str[k];
        str[k] = c;
    }
    return(i-1);
}

/*
 * fold - convert alphabetic token to single case
 *
 */
void
fold(char token[])
{
    int i;
    for (i = 0; token[i] != EOS; i++)
        token[i] = tolower(token[i]);
}

/*
 * scopy - copy string at from[i] to to[j]
 *
 */
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

/*
 * look - look-up definition of `name', copy it in defn
 *
 */
int
look(const char name[], char defn[])
{
    struct hashlist *p;
    if ((p = lookup(name)) == NULL)
        return(NO);
    strcpy(defn, p->def); /* XXX potential overflow here! */
    return(YES);
}

/* vim: set ft=c ts=4 sw=4 et : */
