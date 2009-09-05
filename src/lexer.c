#include "rat4-common.h"

#include "io.h"
#include "tokenizer.h"
#include "lexer.h"


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

    /* ratfor keywords */
    static const char sdo[] = "do";
    static const char sif[] = "if";
    static const char selse[] = "else";
    static const char swhile[] = "while";
    static const char sbreak[] = "break";
    static const char snext[] = "next";
    static const char sfor[] = "for";
    static const char srepeat[] = "repeat";
    static const char suntil[] = "until";
    static const char sswitch[] = "switch";
    static const char scase[] = "case";
    static const char sdefault[] = "default";
    static const char sreturn[] = "return";
    static const char sstring[] = "string";

    do { /* skip empty lines, get next token */;
        tok = gnbtok(lexstr, MAXTOK);
    } while (is_newline(tok));

    if (tok == EOF || is_stmt_ending(tok) || tok == LBRACE || tok == RBRACE)
        /* nothing to do */;
    else if (tok == DIGIT)
        tok = LEXDIGITS;
    else if (STREQ(lexstr, sif) == YES)
        tok = LEXIF;
    else if (STREQ(lexstr, selse) == YES)
        tok = LEXELSE;
    else if (STREQ(lexstr, swhile) == YES)
        tok = LEXWHILE;
    else if (STREQ(lexstr, sdo) == YES)
        tok = LEXDO;
    else if (STREQ(lexstr, sbreak) == YES)
        tok = LEXBREAK;
    else if (STREQ(lexstr, snext) == YES)
        tok = LEXNEXT;
    else if (STREQ(lexstr, sfor) == YES)
        tok = LEXFOR;
    else if (STREQ(lexstr, srepeat) == YES)
        tok = LEXREPEAT;
    else if (STREQ(lexstr, suntil) == YES)
        tok = LEXUNTIL;
    else if (STREQ(lexstr, sswitch) == YES)
        tok = LEXSWITCH;
    else if (STREQ(lexstr, scase) == YES)
        tok = LEXCASE;
    else if (STREQ(lexstr, sdefault) == YES)
        tok = LEXDEFAULT;
    else if (STREQ(lexstr, sreturn) == YES)
        tok = LEXRETURN;
    else if (STREQ(lexstr, sstring) == YES)
        tok = LEXSTRING;
    else
        tok = LEXOTHER;
    return(tok);
}

/* vim: set ft=c ts=4 sw=4 et : */
