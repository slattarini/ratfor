/* include.c -- this file is part of C Ratfor.
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

/*   D E A L   W I T H   F I L E   I N C L U S I O N   */

/*
 * GLOBAL INCLUDES.
 */

#include "rat4-common.h"
#include "rat4-global.h"

#include "tokenizer.h"
#include "include.h"
#include "xopen.h"
#include "error.h"

#define MAXPATH  1024  /* max length of the name of a file included */

/*
 * PUBLIC FUNCTIONS.
 */

BEGIN_C_DECLS

/* Open path and push it on the input files stack. Deal with errors. */
/* FIXME: re-add support for quoted filenames, sooner or later */
void
push_file_stack(const char *path)
{
    int i;
    FILE *fp;

    if (inclevel >= MAX_INCLUDE_DEPTH - 1) {
        synerr_include("includes nested too deeply.");
        return;
    }
    /* skip leading white space in path */
    for (i = 0; is_blank(path[i]); i++)
        /* empty body */;
    if ((path = strdup(&path[i])) == NULL) {
        synerr_include("memory error.");
        return;
    }
    if (*path == EOS) {
        synerr_include("missing filename.");
        return;
    }
    if ((fp = xopen(path, IO_MODE_READ, synerr_include)) == NULL)
        return;
    ++inclevel;
    lineno[inclevel] = 1;
    filename[inclevel] = path;
    infile[inclevel] = fp;
}

/* Pop the input files stack. */
void
pop_file_stack(void)
{
    /* TODO: assert inclevel >= 0? */
    if (inclevel > 0) {
        fclose(infile[inclevel]); /* TODO: check return status? */
        infile[inclevel] = NULL; /* just to be sure */
        free((void *)filename[inclevel]);
        filename[inclevel] = NULL; /* just to be sure */
    }
    inclevel--;
}

/* Process file inclusion statement in current input stream. */
void
process_file_inclusion(void)
{
    int tok;
    int i;
    char path[MAXPATH];

    for (i = 0; ; i = SSTRLEN(path)) {
        if (i >= MAXPATH)
            synerr_fatal("name of included file too long.");
        tok = get_expanded_token(&path[i], MAXPATH);
        if (is_stmt_ending(tok))
            break;
    }
    path[i] = EOS;
    push_file_stack(path);
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
