/* main.c -- this file is part of C Ratfor.
 * Copyright (C) 2009, 2010 Stefano Lattarini <stefano.lattarini@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/*< 

C Ratfor
 
Usage
  ratfor [options] [-o output.f] [input.r]
 
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

>*/

#include "rat4-common.h"

#include "error.h"
#include "init.h"
#include "parser.h"
#include <unistd.h> /* for getopt */

static const char *progname = NULL;

static void
usage_error(const char *msg)
{
    if (msg != NULL)
        fprintf(stderr, "%s: %s\n", progname, msg);
    fprintf(stderr, "Usage: %s [-C] [-l STARTLAB] [-o OUTFILE] [INFILE]\n",
                    progname);
    exit(E_USAGE);
}

/* useful to avoid code duplication in option parsing code */
#define OPTERROR(msg) \
    do { \
        fprintf(stderr, "%s: `-%c': " msg "\n", progname, optopt); \
        errflg = true; \
    } while (false);

C_DECL int
main(int argc, char *argv[])
{
    int c, startlab;
    bool errflg, keep_comments;
    const char *infile = "-", *outfile = "-";
    
    progname = argv[0];

    errflg = false;
    keep_comments = false;
    startlab = 23000; /* default start label */
    opterr = 0; /* we report option parsing errors by ourselves */
    while ((c = getopt(argc, argv, ":Cl:o:")) != -1) {
        switch (c) {
        case 'C':
            keep_comments = true; /* keep comments in src */
            break;
        case 'l': /* user sets label */
            startlab = atoi(optarg);
            break;
        case 'o':
            outfile = optarg;
            break;
        case '?':
            OPTERROR("invalid option");
            break;
        case ':':
            OPTERROR("option requires an argument");
            break;
        default:
            abort(); /* can't happen */
            break;
        }
    }

    if (errflg)
        usage_error(NULL);

    if (argc == optind + 1)
        infile = argv[optind];
    else if (argc > optind + 1)
        usage_error("too many arguments.");

    /* intialize preprocessor status, open input file */
    init(startlab, keep_comments, infile);

    /* open output file for writing */
    if (!STREQ(outfile, "-"))
        if ((freopen(outfile, "w", stdout)) == NULL)
            fatal("%s: cannot open for writing: %s\n", outfile,
                  strerror(errno));

    /* leave our fingerprint in the generated output */
    printf("C Generated by %s\n", PACKAGE_STRING);
    /* call parser and do the real work */
    parse();

    /* return program global exit status, declared in error.h */
    exit(exit_status); /* TODO: check stdout for write errors? */
}

/* vim: set ft=c ts=4 sw=4 et : */
