#include "rat4-common.h"

#include "io.h"
#include "error.h"
#include "tokenizer.h"
#include "lexer.h"
#include "codegen.h"
#include "parser.h"


/*
 * P A R S E R
 */

static void
unstak(int *sp, int lextyp[], int labval[], char token)
{
    int tp;

    for (tp = *sp; tp > 0; tp--) {
        if (lextyp[tp] == LBRACE)
            break;
        if (lextyp[tp] == LEXSWITCH)
            break;
        if (lextyp[tp] == LEXIF && token == LEXELSE)
            break;
        if (lextyp[tp] == LEXIF)
#ifdef F77
            ifend();
#else
            outcon(labval[tp]);
#endif /* F77 */
        else if (lextyp[tp] == LEXELSE) {
            if (*sp > 1)
                tp--;
#ifdef F77
            ifend();
#else
            outcon(labval[tp]+1);
#endif /* F77 */
        }
        else if (lextyp[tp] == LEXDO)
            dostat(labval[tp]);
        else if (lextyp[tp] == LEXWHILE)
            whiles(labval[tp]);
        else if (lextyp[tp] == LEXFOR)
            fors(labval[tp]);
        else if (lextyp[tp] == LEXREPEAT)
            untils(labval[tp], token);
    }
    *sp = tp;
}

void
parse(void)
{
    char lexstr[MAXTOK];
    int lab, labval[MAXSTACK], lextyp[MAXSTACK], sp, i, token;

    sp = 0;
    lextyp[0] = EOF;
    while ((token = lex(lexstr)) != EOF) {
        if (token == LEXIF)
            ifcode(&lab);
        else if (token == LEXDO)
            docode(&lab);
        else if (token == LEXWHILE)
            whilecode(&lab);
        else if (token == LEXFOR)
            forcode(&lab);
        else if (token == LEXREPEAT)
            repcode(&lab);
        else if (token == LEXSWITCH)
            swcode(&lab);
        else if (token == LEXCASE || token == LEXDEFAULT) {
            for (i = sp; i >= 0; i--)
                if (lextyp[i] == LEXSWITCH)
                    break;
            if (i < 0)
                synerr("illegal case or default.");
            else
                cascode(labval[i], token);
        }
        else if (token == LEXDIGITS)
            labelc(lexstr);
        else if (token == LEXELSE) {
            if (lextyp[sp] == LEXIF)
                elseifc(labval[sp]);
            else
                synerr("illegal else.");
        }
        if (token == LEXIF || token == LEXELSE || token == LEXWHILE
            || token == LEXFOR || token == LEXREPEAT
            || token == LEXDO || token == LEXDIGITS
            || token == LEXSWITCH || token == LBRACE)
        {
            sp++; /* beginning of statement */
            if (sp > MAXSTACK)
                synerr_fatal("stack overflow in parser.");
            lextyp[sp] = token; /* stack type and value */
            labval[sp] = lab;
        }
        else if (token != LEXCASE && token != LEXDEFAULT) {
            /*
             * end of statement - prepare to unstack
             */
            if (token == RBRACE) {
                if (lextyp[sp] == LBRACE)
                    sp--;
                else if (lextyp[sp] == LEXSWITCH) {
                    swend(labval[sp--]);
                }
                else
                    synerr("illegal right brace.");
            }
            else if (token == LEXOTHER)
                otherstmt(lexstr);
            else if (token == LEXBREAK || token == LEXNEXT)
                brknxt(sp, lextyp, labval, token);
            else if (token == LEXRETURN)
                retcode();
            else if (token == LEXSTRING)
                strdcl();
            token = lex(lexstr);      /* peek at next token */
            pbstr(lexstr);
            unstak(&sp, lextyp, labval, token);
        }
    }
    if (sp != 0)
        synerr_eof();
}

/* vim: set ft=c ts=4 sw=4 et : */
