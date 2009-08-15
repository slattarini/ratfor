#include <string.h>
#include "utils.h"
#include "lookup.h"
#include "ratdef.h"

/*
 * U T I L I T Y  R O U T I N E S
 */

/*
 * ctoi - convert string at in[i] to int, increment i
 */
int
ctoi(S_CHAR in[], int *i)
{
    int k, j;

    j = *i;
    while (in[j] == BLANK || in[j] == TAB)
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
itoc(int n, S_CHAR str[], int size)
{
    int i, j, k, sign;
    S_CHAR c;

    if ((sign = n) < 0)
        n = -n;
    i = 0;
    do {
        str[i++] = n % 10 + '0';
    }
    while ((n /= 10) > 0 && i < size-2);
    if (sign < 0 && i < size-1)
        str[i++] = '-';
    str[i] = EOS;
    /*
     * reverse the string and plug it back in
     */
    for (j = 0, k = strlen((char *) (&str[0])) - 1; j < k; j++, k--) {
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
fold(S_CHAR token[])
{
    int i;
    /* WARNING - this routine depends heavily on the fact that letters
     * have been mapped into internal right-adjusted ascii. God help
     * you if you have subverted this mechanism. */
    for (i = 0; token[i] != EOS; i++)
        if (token[i] >= BIGA && token[i] <= BIGZ)
            token[i] = token[i] - BIGA + LETA;
}

/*
 * equal - compare str1 to str2; return YES if equal, NO if not
 *
 */
int
equal(S_CHAR str1[], S_CHAR str2[])
{
    int i;

    for (i = 0; str1[i] == str2[i]; i++)
        if (str1[i] == EOS)
            return(YES);
    return(NO);
}

/*
 * scopy - copy string at from[i] to to[j]
 *
 */
void
scopy(S_CHAR from[], int i, S_CHAR to[], int j)
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
look(S_CHAR name[], S_CHAR defn[])
{
    struct hashlist *p;
    if ((p = lookup(name)) == NULL)
        return(NO);
    (void) strcpy((char *) (&defn[0]),(char *) (&((p->def)[0])));
    return(YES);
}

