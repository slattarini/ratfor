/* define.c -- this file is part of C Ratfor.
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

/* Get tokens for the ratfor preprocessor. */

#include "rat4-common.h"
#include "rat4-global.h"

#include "tokenizer.h"
#include "define.h"
#include "io.h"
#include "error.h"
#include "hash.h"

/* Skip next blanks in current input stream, _without_ accounting for
 * macro expansions. */
static inline void
skip_raw_blanks(void)
{
    char c;
    do {
        c = ngetch(infile[inclevel]);
    } while (is_blank(c));
    put_back_char(c);
}


/* Parse definition of ratfor macro.  If an error is detected, stop ratfor
 * with a suitable error message, else save macro name (in `name[]') and
 * macro definition (in `def[]'). */
static void
get_macro_definition(char name[], int namesiz, char def[], int defsiz)
{
    int i, j, nlpar, t, t2;
    bool defn_with_paren;
    char buf[MAXTOK];

    /* hackish internal macro to avoid code duplication below */
#   define EXTEND_DEFN_WITH_TOKEN_(buf_) \
        for (j = 0; buf_[j] != EOS; i++, j++) { \
            if (i >= defsiz) \
                synerr_fatal("definition too long."); \
            def[i] = buf_[j]; \
        }

    skip_raw_blanks();
    if ((t = get_unexpanded_token(buf, MAXTOK)) != LPAREN) {
        defn_with_paren = false; /* define name def */
        put_back_string(buf);
    } else {
        defn_with_paren = true; /* define(name,def) */
        reading_parenthesized_macro_definition = true;
    }
    skip_raw_blanks();
    t2 = get_unexpanded_token(name, namesiz); /* name */
    if (!defn_with_paren && is_stmt_ending(t2)) {
        /* stray `define', as in `...; define; ...' */
        synerr_fatal("empty name."); /* XXX: better diagnostic? */
    } else if (defn_with_paren && t2 == COMMA) {
        /* `define(name, def)' with empty name */
        synerr_fatal("empty name."); /* XXX: better diagnostic? */
    } else if (t2 != TOKT_ALPHA) {
        synerr_fatal("non-alphanumeric name.");
    }
    skip_raw_blanks();
    if (!defn_with_paren) { /* define name def */
        i = 0;
        for (;;) {
            t2 = get_unexpanded_token(buf, MAXTOK);
            if (is_newline(t2) || t2 == EOF) {
                put_back_string(buf);
                break;
            }
            EXTEND_DEFN_WITH_TOKEN_(buf);
        }
        def[i] = EOS;
    } else { /* define (name, def) */
        if (get_unexpanded_token(buf, MAXTOK) != COMMA)
            synerr_fatal("missing comma in define.");
        /* else got (name, */
        for (i = 0, nlpar = 0; nlpar >= 0; /* empty clause */) {
            t2 = get_unexpanded_token(buf, MAXTOK);
            if (t2 == EOF)
                synerr_fatal("missing right paren.");
            else if (t2 == LPAREN)
                nlpar++;
            else if (t2 == RPAREN)
                nlpar--;
            EXTEND_DEFN_WITH_TOKEN_(buf);
        }
        /* TODO: assert def[i - 1] == ')' */
        def[i - 1] = EOS;
        /* The macro definition has been read completely, so it's not
         * true anymore that we are scanning it. */
        reading_parenthesized_macro_definition = false;
    }
    
    /* get rid of temporary internal macro */
#   undef EXTEND_DEFN_WITH_TOKEN_
}

/* Read and save macro definition. */
C_DECL void
get_and_install_macro_definition(void)
{
    char body[MAXDEFLEN], name[MAXTOK];

    /* get definition for name, save it in body ... */
    get_macro_definition(name, MAXTOK, body, MAXDEFLEN);
    /* ... and install it. */
    hash_install(name, body);
}

/* vim: set ft=c ts=4 sw=4 et : */
