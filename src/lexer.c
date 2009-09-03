#include "rat4-common.h"

#include "io.h"
#include "tokenizer.h"
#include "lexer.h"


/*
 *  K E Y W O R D S
 */

static const char sdo[] = "do";
static const char vdo[] = { LEXDO, EOS };

static const char sif[] = "if";
static const char vif[] = { LEXIF, EOS };

static const char selse[] = "else";
static const char velse[] = { LEXELSE, EOS };

static const char swhile[] = "while";
static const char vwhile[] = { LEXWHILE, EOS };

static const char sbreak[] = "break";
static const char vbreak[] = { LEXBREAK, EOS };

static const char snext[] = "next";
static const char vnext[] = { LEXNEXT, EOS };

static const char sfor[] = "for"; 
static const char vfor[] = { LEXFOR, EOS };

static const char srept[] = "repeat";
static const char vrept[] = { LEXREPEAT, EOS };

static const char suntil[] = "until";
static const char vuntil[] = { LEXUNTIL, EOS };

static const char sswitch[] = "switch";
static const char vswitch[] = { LEXSWITCH, EOS };

static const char scase[] = "case";
static const char vcase[] = { LEXCASE, EOS };

static const char sdefault[] = "default";
static const char vdefault[] = { LEXDEFAULT, EOS };

static const char sret[] = "return";
static const char vret[] = { LEXRETURN, EOS };

static const char sstr[] = "string";
static const char vstr[] = { LEXSTRING, EOS };

/*
 *  L E X I C A L  A N A L Y S E R
 */


/*
 * lex - return lexical type of token, while writing it in lexstr[]
 *
 */
int
lex(char lexstr[])
{

    int tok;

    while ((tok = gnbtok(lexstr, MAXTOK)) == NEWLINE)
            /* skip empty lines */;
    if (tok == EOF || tok == SEMICOL || tok == LBRACE || tok == RBRACE)
        /* nothing to do */;
    else if (tok == DIGIT)
        tok = LEXDIGITS;
    else if (STREQ(lexstr, sif) == YES)
        tok = vif[0];
    else if (STREQ(lexstr, selse) == YES)
        tok = velse[0];
    else if (STREQ(lexstr, swhile) == YES)
        tok = vwhile[0];
    else if (STREQ(lexstr, sdo) == YES)
        tok = vdo[0];
    else if (STREQ(lexstr, sbreak) == YES)
        tok = vbreak[0];
    else if (STREQ(lexstr, snext) == YES)
        tok = vnext[0];
    else if (STREQ(lexstr, sfor) == YES)
        tok = vfor[0];
    else if (STREQ(lexstr, srept) == YES)
        tok = vrept[0];
    else if (STREQ(lexstr, suntil) == YES)
        tok = vuntil[0];
    else if (STREQ(lexstr, sswitch) == YES)
        tok = vswitch[0];
    else if (STREQ(lexstr, scase) == YES)
        tok = vcase[0];
    else if (STREQ(lexstr, sdefault) == YES)
        tok = vdefault[0];
    else if (STREQ(lexstr, sret) == YES)
        tok = vret[0];
    else if (STREQ(lexstr, sstr) == YES)
        tok = vstr[0];
    else
        tok = LEXOTHER;
    return(tok);
}

/* vim: set ft=c ts=4 sw=4 et : */
