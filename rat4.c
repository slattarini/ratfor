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

#include <stdio.h>
#include <stdlib.h>

#include "ratdef.h"
#include "ratcom.h"
#include "getopt.h"

/*
 * initialise global variables
 */
void
initvars(void)
{
    int i;

    outp = 0;       /* output character pointer */
    level = 0;      /* file control */
    linect[0] = 1;  /* line count of first file */
    fnamp = 0;
    fnames[0] = EOS;
    bp = -1;        /* pushback buffer pointer */
    fordep = 0;     /* for stack */
    swtop = 0;      /* switch stack index */
    swlast = 1;     /* switch stack index */
    
    for (i = 0; i <= 126; i++)
        tabptr[i] = 0;
    
    /* default definitions */
    static char deftyp[] = { DEFTYPE, EOS };
    install("define", deftyp);
    install("DEFINE", deftyp);
    
    fcname[0] = EOS;  /* current function name */
    label = startlab; /* next generated label */
}

/* main subroutine */
int
main(argc,argv)
int argc;
char *argv[];
{
    int c, errflg = 0;
    char *progname = argv[0];
    
    extern void parse(void); /* XXX: move out */
    
    /* init global vars that can be changed by options */
    startlab = 23000; /* default start label */
    leaveC = NO;
    while ((c = our_getopt(argc, argv, "Chn:o:6:")) != EOF)
    switch (c) {
        case 'C':
            leaveC = YES; /* keep comments in src */
            break;
        case 'h':
            /* not written yet */
            break;
        case 'l': /* user sets label */
            startlab = atoi(optarg);
            break;
        case 'o':
            if ((freopen(optarg, "w", stdout)) == NULL)
                error("can't write %s\n", optarg);
            break;
        case '6':
            /* not written yet */
            break;
        default:
            ++errflg;
    }

    if (errflg) {
        fprintf(stderr,
                "usage: %s [-C][-hx][-l n][-o file][-6x] [file...]\n",
                progname);
        exit(2);
    }

    /*
     * present version can only process one file, sadly.
     */
    if (optind77 >= argc)
        infile[0] = stdin;
    else if ((infile[0] = fopen(argv[optind77], "r")) == NULL)
        error("cannot read %s\n", argv[optind77]);

    initvars();
    printf("C Output from Public domain Ratfor, version 1.0\n");
    parse(); /* call parser.. */

    exit(0); /* TODO: check stdout for write errors? */
}
