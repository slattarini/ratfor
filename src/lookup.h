#ifndef RAT4_LOOKUP_H
#  define RAT4_LOOKUP_H 1

/*
 * from K&R "The C Programming language"
 * Table lookup routines structure and definitions
 *
 */

/* basic table entry */
struct hashlist {
    const char *name;
    const char *def;
    struct hashlist *next; /* next in chain */
};

#define HASHMAX 100 /* size of hashtable */

struct hashlist *install(const char *, const char *);
struct hashlist *lookup(const char *);

#endif
/* vim: set ft=c ts=4 sw=4 et : */
