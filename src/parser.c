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

#include "rat4-common.h"
#include "rat4-global.h"

#include "io.h"         /* for `put_back_string()' */
#include "error.h"      /* for `synerr*()' */
#include "tokenizer.h"
#include "codegen.h"
#include "parser.h"

#define MAXSTACK 1024  /* max stack depth for parser */

/*
 *  P A R S E R
 */

/* shared among unstak() and parser() */
static int sp = 0;
static int labval[MAXSTACK];
static int lextyp[MAXSTACK];

static bool
detected_unusual_error(int token)
{
    char tmptok[MAXTOK];

    if (token == LPAREN) {
        token = get_nonblank_token(tmptok, MAXTOK); /* peek at next token */
        put_back_string(tmptok);
        if (token == TOKT_DIGITS) {
            synerr("label following left parenthesis.");
            return(true);
        }
    } else if (token == TOKT_DIGITS) {
        token = get_nonblank_token(tmptok, MAXTOK); /* peek at next token */
        put_back_string(tmptok);
        if (token == SEMICOL) {
            synerr("label followed by empty statement.");
            return(true);
        }
    }
    return(false);
}

static void
unstak(char token)
{
    for (; sp > 0; sp--) {
        switch(lextyp[sp]) {
            case LBRACE:
            case LEXSWITCH:
                return;
                break;
            case LEXIF:
                if (token == LEXELSE)
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
                untils(labval[sp], token);
                break;
        } /* end switch */
    } /* end for */
}

C_DECL void
parse(void)
{
    char lexstr[MAXTOK];
    int lab, i, token;

    lextyp[0] = EOF;
    while ((token = lex(lexstr, MAXTOK)) != EOF) {
        
        /* some errors, if not treated specially, may give confusing
         * diagnostic, so we try to detect them in an ad-hoc way */
        if (detected_unusual_error(token))
            continue;

        /* do code generation */
        switch(token) {
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
                    if (token == LEXCASE)
                        synerr("illegal case");
                    else
                        synerr("illegal default");
                } else {
                    cascode(labval[i], token);
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
        switch(token) {
            case TOKT_DIGITS:
            case LEXIF:
            case LEXELSE:
            case LEXWHILE:
            case LEXFOR:
            case LEXREPEAT:
            case LEXDO:
            case LEXSWITCH:
            case LBRACE:
                sp++; /* beginning of statement */
                if (sp > MAXSTACK)
                    synerr_fatal("stack overflow in parser.");
                lextyp[sp] = token; /* stack type and value */
                labval[sp] = lab;
                break;
            case LEXCASE:
            case LEXDEFAULT:
                /* do nothing */
                break;
            default:
                /* end of statement - prepare to unstack */
                switch(token) {
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
                        brknxt(sp, lextyp, labval, token);
                        break;
                    case LEXRETURN:
                        retcode();
                        break;
                    default:
                        otherstmt(lexstr);
                        break;
                }
                token = lex(lexstr, MAXTOK); /* peek at next token */
                put_back_string(lexstr);
                unstak(token);
        }
    } /* while read next token */
    if (sp != 0)
        synerr_eof(); /* XXX: more detailed error message? */
}

/* vim: set ft=c ts=4 sw=4 et : */
