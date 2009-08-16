/*
 * from K&R "The C Programming language"
 * Table lookup routines
 * structure and definitions
 *
 */

/* basic table entry */
struct hashlist {
	char	*name;
	char	*def;
	struct	hashlist *next; /* next in chain */
};

#define HASHMAX	100	/* size of hashtable */

struct hashlist *install(char *name, char *def);
struct hashlist *lookup(char *s);
