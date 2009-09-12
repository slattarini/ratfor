#include "rat4-common.h"
#include "rat4-global.h"

#include <stdarg.h>

#include "utils.h"
#include "error.h"

/* Global program exit status, declared in error.h */
int exit_status = EXIT_SUCCESS;


/*
 *  E R R O R  M E S S A G E S
 */


/* Ugly defines to avoid too much code duplication.  Read the body of the
 * `synerr()' function to understand how this macros work.  By the way,
 * the code would be much simplified if we could use variadic macros, but
 * for the moment we are restraining ourselves to C89.
 */

#define SYNERR_PREAMBLE(lineno_offset) \
    int xlevel = 0; \
    /* account for EOF errors, where level < 0 */ \
    xlevel = (level >= 0) ? level : 0; \
    fflush(stdout); \
    fprintf(stderr, "%s:%d: ", filename[xlevel], \
                    lineno[xlevel] + lineno_offset) /* do not add `;' */

#define SYNERR_POSTAMBLE() \
    fputc('\n', stderr); \
    exit_status = EXIT_FAILURE /* do not add `;' */

#define SYNERR_PREAMBLE_VARIADIC(lineno_offset) \
    va_list ap; \
    SYNERR_PREAMBLE(lineno_offset); \
    va_start(ap, msg); /* do not add `;' */

#define SYNERR_POSTAMBLE_VARIADIC() \
    va_end(ap); \
    SYNERR_POSTAMBLE() /* do not add `;' */

#define SYNERR_PRINTERR_VARIADIC() vfprintf(stderr, msg, ap)

/*
 * fatal - print error message with the parameters, then die.
 */
void
fatal(const char *msg, ...)
{
    va_list ap;
    va_start(ap, msg);
    fflush(stdout);
    vfprintf(stderr, msg, ap);
    va_end(ap);
    exit(EXIT_FAILURE);
}


/*
 * synerr - report Ratfor syntax error, set global exit status to failure.
 */
void
synerr(const char *msg, ...)
{
    SYNERR_PREAMBLE_VARIADIC(0);
    SYNERR_PRINTERR_VARIADIC();
    SYNERR_POSTAMBLE_VARIADIC();
}

/*
 * synerr_include - report error about file inclusion, taking care of not
 *                  having line number in error message off by one.
 */
void
synerr_include(const char *msg, ...)
{
    SYNERR_PREAMBLE_VARIADIC(-1);
    fprintf(stderr, "include: ");
    SYNERR_PRINTERR_VARIADIC();
    SYNERR_POSTAMBLE_VARIADIC();
}


/*
 * synerr_eof() - report Ratfor syntax error about unexpected end-of-file,
 *                taking care of not having line number in error message
 *                off by one.
 */
void
synerr_eof(void)
{
    SYNERR_PREAMBLE(-1);
    fprintf(stderr, "unexpected EOF.");
    SYNERR_POSTAMBLE();
}


/*
 * synerr_fatal - report Ratfor syntax error, then die
 */
void
synerr_fatal(const char *msg, ...)
{
    SYNERR_PREAMBLE_VARIADIC(0);
    SYNERR_PRINTERR_VARIADIC();
    SYNERR_POSTAMBLE_VARIADIC();
    exit(EXIT_FAILURE);
}

/* vim: set ft=c ts=4 sw=4 et : */
