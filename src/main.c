/* $Id$ */

/*< 
  ratfor77
 
Usage
  ratfor77 < input.r >output.f pars
 
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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ratdef.h"
#include "main.h"
#include "error.h"

int
main(int argc, char *argv[])
{
    int c, errflg = NO;
    char *progname = argv[0];
    FILE *infile;
    
    extern void init(int, int, FILE *); /* XXX: move out */
    extern void parse(void); /* XXX: move out */
    
    int startlab = 23000; /* default start label */
    int leaveC = NO;
    while ((c = getopt(argc, argv, "Cn:o:")) != EOF) {
        switch (c) {
            case 'C':
                leaveC = YES; /* keep comments in src */
                break;
            case 'l': /* user sets label */
                startlab = atoi(optarg);
                break;
            case 'o':
                if ((freopen(optarg, "w", stdout)) == NULL)
                    error("%s: cannot open for writing\n", optarg);
                break;
            default:
                errflg = YES;
        }
    }

    if (errflg) {
        fprintf(stderr,
                "Usage: %s [-C] [-l STARTLAB] [-o OUTFILE] [INFILE...]\n",
                progname);
        exit(2);
    }

    /*
     * present version can only process one file, sadly.
     */
    if (optind >= argc)
        infile = stdin;
    else if ((infile = fopen(argv[optind], "r")) == NULL)
        error("%s: cannot open for reading\n", argv[optind]);

    init(startlab, leaveC, infile);
    printf("C Output from Public Domain Ratfor, version %s\n",
           PACKAGE_VERSION);
    parse(); /* call parser and do the real work */
    
    /* return program global exit status, declared in error.h */
    exit(exit_status); /* TODO: check stdout for write errors? */
}

/* vim: set ft=c ts=4 sw=4 et : */
