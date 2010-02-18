/* parser.c -- this file is part of C Ratfor.
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


/*   R A T F O R   L E X E R   A N D   P A R S E R   */


/*
 * GLOBAL INCLUDES
 */

#include "rat4-common.h"
#include "rat4-global.h"

#include "lex-types.h"
#include "parser.h"
#include "tokenizer.h"
#include "define.h"
#include "include.h"
#include "codegen.h"
#include "error.h"
#include "io.h" /* for `put_back_string()' */

#define MAXSTACK 1024  /* max stack depth for parser */


/*
 * PRIVATE GLOBAL VARIABLES
 */

/* shared among unstak() and parser() */
static int sp = 0;
static int labval[MAXSTACK];
static int lextyp[MAXSTACK];


/*
 * PRIVATE FUNCTIONS
 */

static bool
detected_unusual_error(int toktype)
{
    char buf[MAXTOK];

    if (toktype == LPAREN) {
        toktype = get_nonblank_token(buf, MAXTOK); /* peek at next token */
        put_back_string(buf);
        if (toktype == TOKT_DIGITS) {
            synerr("label following left parenthesis.");
            return(true);
        }
    } else if (toktype == TOKT_DIGITS) {
        toktype = get_nonblank_token(buf, MAXTOK); /* peek at next token */
        put_back_string(buf);
        if (toktype == SEMICOL) {
            synerr("label followed by empty statement.");
            return(true);
        }
    }
    return(false);
}

static void
unstak(int lextype)
{
    for (; sp > 0; sp--) {
        switch(lextyp[sp]) {
            case LBRACE:
            case LEXSWITCH:
                return;
                break;
            case LEXIF:
                if (lextype == LEXELSE)
                    return;
                ifend();
                break;
            case LEXELSE:
                if (sp > 1)
                    sp--;
                ifend();
                break;
            case LEXDO:
                dostat(labval[sp]);
                break;
            case LEXWHILE:
                whiles(labval[sp]);
                break;
            case LEXFOR:
                fors(labval[sp]);
                break;
            case LEXREPEAT:
                untils(labval[sp], lextype);
                break;
        } /* end switch */
    } /* end for */
}

/* Save next input token in buf[], and return its lexical type (if it's
 * a keyword) or its token type (otherwise). */
static int
lex(char buf[], int bufsiz)
{
    int tok;

    /* skip empty lines, get next token, copy it in buf[] */
    do {
        tok = get_nonblank_token(buf, bufsiz);
    } while (is_newline(tok));

    /* if buf[] is a ratfor keyword, update its token type accordingly,
     * else leave it unchanged */
    if (buf[0] == PERCENT && buf[1] == EOS)
        tok = LEXVERBATIM;
    else if (STREQ(buf, "break"))
        tok = LEXBREAK;
    else if (STREQ(buf, "case"))
        tok = LEXCASE;
    else if (STREQ(buf, "default"))
        tok = LEXDEFAULT;
    else if (STREQ(buf, "define"))
        tok = LEXDEFN;
    else if (STREQ(buf, "do"))
        tok = LEXDO;
    else if (STREQ(buf, "else"))
        tok = LEXELSE;
    else if (STREQ(buf, "for"))
        tok = LEXFOR;
    else if (STREQ(buf, "if"))
        tok = LEXIF;
    else if (STREQ(buf, "include"))
        tok = LEXINCLUDE;
    else if (STREQ(buf, "next"))
        tok = LEXNEXT;
    else if (STREQ(buf, "repeat"))
        tok = LEXREPEAT;
    else if (STREQ(buf, "return"))
        tok = LEXRETURN;
    else if (STREQ(buf, "switch"))
        tok = LEXSWITCH;
    else if (STREQ(buf, "until"))
        tok = LEXUNTIL;
    else if (STREQ(buf, "while"))
        tok = LEXWHILE;

    return(tok);
}


/*
 * PUBLIC FUNCTIONS
 */

C_DECL void
parse(void)
{
    char lexstr[MAXTOK];
    int lab, i;
    int lextype; /* lexical type of next token read */

    lextyp[0] = EOF;
    while ((lextype = lex(lexstr, MAXTOK)) != EOF) {
        
        /* some errors, if not treated specially, may give confusing
         * diagnostic, so we try to detect them in an ad-hoc way */
        if (detected_unusual_error(lextype))
            continue;

        /* do code generation */
        switch(lextype) {
            case LEXVERBATIM:
                verbatim(); /* copy direct to output */
                break;
            case LEXINCLUDE:
                process_file_inclusion();
                break;
            case LEXDEFN:
                get_and_install_macro_definition();
                break;
            case LEXIF:
                ifcode(&lab);
                break;
            case LEXDO:
                docode(&lab);
                break;
            case LEXWHILE:
                whilecode(&lab);
                break;
            case LEXFOR:
                forcode(&lab);
                break;
            case LEXREPEAT:
                repcode(&lab);
                break;
            case LEXSWITCH:
                swcode(&lab);
                break;
            case LEXCASE:
            case LEXDEFAULT:
                /* only permit case/default in the immediate block after a
                 * switch statement, not in blocks nested into it */
                for (i = sp; i >= 0 ; i--) {
                    if (lextyp[i] == LBRACE)
                        continue; /* ignore */
                    else
                        break; /* unmatched statement */
                }
                if (i < 0 || lextyp[i] != LEXSWITCH) {
                    if (lextype == LEXCASE)
                        synerr("illegal case");
                    else
                        synerr("illegal default");
                } else {
                    cascode(labval[i], lextype);
                }
                break;
            case TOKT_DIGITS:
                labelc(lexstr);
                break;
            case LEXELSE:
                if (lextyp[sp] == LEXIF)
                    elseifc();
                else
                    synerr("illegal else.");
                break;
        } /* switch token */
        
        /* manange stack of statements */
        switch(lextype) {
            case TOKT_DIGITS:
            case LEXIF:
            case LEXELSE:
            case LEXWHILE:
            case LEXFOR:
            case LEXREPEAT:
            case LEXDO:
            case LEXSWITCH:
            case LBRACE:
                /* begin control-flow statement - will need unstack */
                sp++;
                if (sp > MAXSTACK)
                    synerr_fatal("stack overflow in parser.");
                lextyp[sp] = lextype; /* stack type and value */
                labval[sp] = lab;
                break;
            case LEXCASE:
            case LEXDEFAULT:
            case LEXVERBATIM:
            case LEXINCLUDE:
            case LEXDEFN:
                /* self-contained statement - no need to later unstack */
                break;
            default:
                /* end of statement - prepare to unstack */
                switch(lextype) {
                    case RBRACE:
                        if (lextyp[sp] == LBRACE)
                            sp--;
                        else if (lextyp[sp] == LEXSWITCH)
                            swend(labval[sp--]);
                        else
                            synerr("illegal right brace.");
                        break;
                    case LEXBREAK:
                    case LEXNEXT:
                        brknxt(sp, lextyp, labval, lextype);
                        break;
                    case LEXRETURN:
                        retcode();
                        break;
                    default:
                        otherstmt(lexstr);
                        break;
                }
                lextype = lex(lexstr, MAXTOK); /* peek at next token */
                put_back_string(lexstr);
                unstak(lextype);
        }
    } /* while read next token */
    if (sp != 0)
        synerr_eof(); /* XXX: more detailed error message? */
}

/* vim: set ft=c ts=4 sw=4 et : */
