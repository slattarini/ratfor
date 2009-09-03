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

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "error.h"
#include "parse.h"
#include "ratdef.h"

static void init(int, int, char *); /* XXX: move out */

int
main(int argc, char *argv[])
{
    int c, errflg = NO;
    char *progname = argv[0], *infile = "-";
    
    int startlab = 23000; /* default start label */
    int leaveC = NO;
    while ((c = getopt(argc, argv, "Cl:o:")) != EOF) {
        switch (c) {
            case 'C':
                leaveC = YES; /* keep comments in src */
                break;
            case 'l': /* user sets label */
                startlab = atoi(optarg);
                break;
            case 'o':
                /* XXX make this "lazily" executed later */
                if ((freopen(optarg, "w", stdout)) == NULL)
                    error("%s: cannot open for writing\n", optarg); /*XXX: perror*/
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

    /* TODO error if two or more args given */
    
    if (optind < argc)
        infile = argv[optind];
    init(startlab, leaveC, infile); /* intialize preprocessor status */
    printf("C Output from Public Domain Ratfor, version %s\n",
           PACKAGE_VERSION);
    parse(); /* call parser and do the real work */
    
    /* return program global exit status, declared in error.h */
    exit(exit_status); /* TODO: check stdout for write errors? */
}

#include "rat4-global.h"

/*
 * initialisation
 */
static void
init(int xstartlab, int xleaveC, char *xfilename)
{
    int i;
    FILE *in;
    
    startlab = xstartlab;
    leaveC = xleaveC;
    
    /* XXX wrap this in a function */
    if (STREQ(xfilename, "-"))
        in = stdin;
    else if ((in = fopen(xfilename, "r")) == NULL)
        error("%s: cannot open for reading\n", xfilename); /*XXX: perror?*/

    level = 0;                  /* file control */
    linect[0] = 1;              /* line count of first file */
    filename[0] = xfilename;    /* filename of first file */
    infile[0] = in;             /* file handle of first file */
    fordep = 0;                 /* for stack */
    swtop = 0;                  /* switch stack index */
    swlast = 1;                 /* switch stack index */
    
    for (i = 0; i <= 126; i++)
        tabptr[i] = 0;
    
    fcname[0] = EOS;  /* current function name */
    label = startlab; /* next generated label */
}


/* vim: set ft=c ts=4 sw=4 et : */
