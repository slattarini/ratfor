/*< 
  ratfor77
 
Usage
  ratfor77 -o output.f input.r pars
 
Input Parameters
  l - flag [-l n]  user sets strating label n
  o - flag [-o output]  specify output file, otherwise it is stdout
  C - flag [-C] keep comments in (useful for compiler directives)

 Description 
  A ratfor pre-processor in C.

Comments
  Derived from a pre-processor distributed by the University of
  Arizona. Closely corresponds to the pre-processor described in
  the "SOFTWARE TOOLS" book.

Category
 Tools

Compile Level
 DISTR

Copyright
 This code is in the public domain. In other words, all rights are
 granted to all recipients, "public" at large.

>*/

#include "rat4-common.h"

#include <stdlib.h>
#include <unistd.h> /* for getopt */

#include "error.h"
#include "parser.h"

int
main(int argc, char *argv[])
{
    int c, errflg = NO;
    const char *progname = argv[0], *infile = "-", *outfile = "-";

    int startlab = 23000; /* default start label */
    int keep_comments = NO;
    while ((c = getopt(argc, argv, "Cl:o:")) != EOF) {
        switch (c) {
            case 'C':
                keep_comments = YES; /* keep comments in src */
                break;
            case 'l': /* user sets label */
                startlab = atoi(optarg);
                break;
            case 'o':
                outfile = optarg;
                break;
            default:
                errflg = YES;
                break;
        }
    }

    if (errflg) {
        fprintf(stderr,
                "Usage: %s [-C] [-l STARTLAB] [-o OUTFILE] [INFILE...]\n",
                progname);
        exit(2);
    }

    /* TODO error if two or more args given */

    if (optind < argc)
        infile = argv[optind];

    /* intialize preprocessor status */
    init(startlab, keep_comments, infile);

    /* open output file for writing */
    if (!STREQ(outfile, "-"))
        if ((freopen(outfile, "w", stdout)) == NULL)
            fatal("%s: cannot open for writing: %s\n", outfile,
                  strerror(errno));

    /* leave our fingerprint in the generated output */
    printf("C Output from Public Domain Ratfor, version %s\n",
           PACKAGE_VERSION);
    /* call parser and do the real work */
    parse();

    /* return program global exit status, declared in error.h */
    exit(exit_status); /* TODO: check stdout for write errors? */
}

/* vim: set ft=c ts=4 sw=4 et : */
