#ifndef RAT4_LOOKUP_H
#  define RAT4_LOOKUP_H 1

#define HASHMAX 100 /* size of hashtable */

BEGIN_C_DECLS

/*
 * from K&R "The C Programming language"
 * Table lookup routines structure and definitions
 */

/* basic table entry */
struct hashlist {
    const char *name;
    const char *def;
    struct hashlist *next; /* next in chain */
};

struct hashlist *hash_install(const char *, const char *);
struct hashlist *hash_lookup(const char *);

END_C_DECLS

#endif
/* vim: set ft=c ts=4 sw=4 et : */
