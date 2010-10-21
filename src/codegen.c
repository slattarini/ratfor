/* codegen.c -- this file is part of C Ratfor.
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

#include "lex-types.h"
#include "io.h"
#include "error.h"
#include "tokenizer.h"
#include "labgen.h"
#include "codegen.h"

#define MAXSTRNAME MAXFUNCNAME /* max length for a string name */
#define MAXSWITCH  300         /* max stack for switch statement */
#define MAXFORSTK  200         /* max space for for reinit clauses */

/*
 *  C O D E  G E N E R A T I O N
 */

/*
 *** NOTE on unlabled "continue" ***
 *
 * The unlabeled "continue"s generated in the functions `while_code',
 * `repcode' and `for_code' below are *required* in the unlikely
 * circumstance that the "while"/"for"/"repeat" statement in the
 * ratfor input has been labeled, as in e.g.:
 *   10 while(i > 0) ...
 * Without the unabeled "continue", the result would be something like:
 *   10 23000 if((1))then
 *   ...
 * which is clearly wrong.
 */

/*
 * Private Constant Strings.
 */

static const char scontinue[]   = "continue";
static const char sdo[]         = "do";
static const char sif[]         = "if(";
static const char sifnot[]      = "if(.not.";
static const char sthen[]       = "then";
static const char selse[]       = "else";
static const char sendif[]      = "endif";
static const char sgoto[]       = "goto";
static const char sreturn[]     = "return";
static const char sfunction[]   = "function";
static const char ssubroutine[] = "subroutine";
static const char send[]        = "end";


/*
 * Private Global Variables
 */

static int swtop = 0;   /* switch stack index */
static int swlast = 1;  /* switch stack index */
static int fordep = 0;  /* for stack */
static bool xfer;  /*  true if just made transfer, false otherwise */
                   /* XXX how to initialize it? */

static int swstak[MAXSWITCH];   /* switch information stack */
static char forstk[MAXFORSTK];  /* stack of reinit strings  */


/*
 * Private Functions.
 */

/* Copy string at &from[i] into string at &to[j] */
static void
scopy(const char from[], int i, char to[], int j)
{
    int k1, k2;
    for (k1 = i, k2 = j; (to[k2] = from[k1]) != EOS; k1++, k2++)
        /* empty body */;
}

/*
 * Convert string str[] to integer, skipping leading blanks and stopping at
 * the first non-numeric character in str[].  Does not recognize negative
 * integers.  Return the converted integer.
 */
static int
string_to_integer(const char str[])
{
    int i = 0, n = 0;
    while (is_blank(str[i]))
        i++;
    for (; str[i] != EOS; i++) {
        if (str[i] < DIG0 || str[i] > DIG9)
            break;
        n = 10 * n + (str[i] - DIG0);
    }
    return(n);
}

/*
 *  token_requires_automatic_line_continuation(t) -
 *    - tell if token `t' should trigger an automatic line continuation.
 *
 *  OK, so why we don't attemp (anymore) to trigger line continuation not
 *  only for `+' and `-', but also for `*' and `/' tokens?
 *
 *  The reason is that the `*' and `/' tokens cannot be simply assumed to be
 *  Fortran operators, since they have way too much usages in Fortran (apart
 *  being used as multiplication and divsion operators, of course).
 *
 *  For example, `*' can be used in write statements:
 *      ...
 *      WRITE(*,100) 2
 *  100 FORMAT(I1)
 *      ...
 *  in the declaration of arrays passed as arguments to subprograms:
 *      ...
 *      SUBROUTINE LINREGR(N, X, Y)
 *      INTEGER N
 *      REAL X(*), Y(*)
 *      ...
 *  as part of the exponetiation operator:
 *      ...
 *      X = 2**3
 *      ...
 *  or when declaring constant strings:
 *      ...
 *      CHARACTER STR1*10
 *      CHARACTER STR2*(*)
 *      ...
 *
 *  Similarly, `/' can be used as part of the string-concatenation
 *  operator:
 *      ...
 *      S1 = '123'
 *      S2 = '456'
 *      S3 = S1 // S2
 *      ...
 *  in the `common' statement:
 *      ...
 *      REAL ALPHA, BETA
 *      COMMON /COEFF/ ALPHA, BETA
 *      ...
 *  or in the `data' satatement:
 *      ...
 *      REAL A(10,20)
 *      DATA A/200 * 0.0/
 *      ...
 *  (and note that in the above code snippet the token `*' is used with
 *  still another different meaning than those listed before!)
 *
 *  Thus, we make no assumption on the possible meaning of `/' and `*' in
 *  input, and do not attempt line continuation for them.
 */
static bool
token_requires_automatic_line_continuation(int t)
{
    switch (t) {
        case COMMA:
        case LPAREN:
        case PLUS:
        case MINUS:
        case EQUALS:
        case TOKT_RELATN:
            return true;
    }
    return false;
}

static const char *
stringyfy_subprog_type(enum subprg_t subprogram_type)
{
    switch (subprogram_type) {
        case SUBPRG_FUNC:
            return("function");
            break;
        case SUBPRG_SUBR:
            return("subroutine");
            break;
        case SUBPRG_NONE:
            return(NULL);
            break;
        default: /* can't happen */
            abort();
            break;
    }
}

/* peek_subprog_name - read function or subroutine name, save it in global
 * variable `current_subprogram_name' */
static void
peek_subprog_name(enum subprg_t subprogram_type)
{
    const char *new_subprogram_str, *old_subprogram_str;
    int t;

    new_subprogram_str = stringyfy_subprog_type(subprogram_type);
    old_subprogram_str = stringyfy_subprog_type(current_subprogram_type);

    /* TODO: assert new_subprogram_str != NULL */

    if (current_subprogram_type != SUBPRG_NONE) {
        synerr("%s defined into %s body.", new_subprogram_str,
               old_subprogram_str);
    }
    current_subprogram_type = subprogram_type;
    t = get_nonblank_token(current_subprogram_name, MAXFUNCNAME);
    put_back_string(current_subprogram_name);
    if (is_stmt_ending(t)) {
        synerr("missing %s name.", new_subprogram_str);
    } else if (t != TOKT_ALPHA) {
        synerr("invalid %s name `%s'", new_subprogram_str,
               current_subprogram_name);
    }
    put_back_char(BLANK);
}

/* balpar - copy balanced paren string */
static void
balpar(void)
{
    char token[MAXTOK];
    int t, nlpar;

    if (get_nonblank_token(token, MAXTOK) != LPAREN) {
        synerr("missing left paren.");
        return;
    }
    outstr(token);
    nlpar = 1;
    do {
        t = get_token(token, MAXTOK);
        if (t == SEMICOL || t == LBRACE || t == RBRACE || t == EOF) {
            put_back_string(token);
            break;
        }
        if (is_newline(t)) /* delete newlines */
            token[0] = EOS;
        else if (t == LPAREN)
            nlpar++;
        else if (t == RPAREN)
            nlpar--;
        /* else nothing special */
        outstr(token);
    } while (nlpar > 0);
    if (nlpar != 0)
        synerr("missing parenthesis in condition.");
}

/* eatup - process rest of statement; interpret automatic continuations */
static void
eatup(void)
{
    char ptoken[MAXTOK], token[MAXTOK];
    int nlpar, t;

    for (nlpar = 0; nlpar >= 0; outstr(token)) {
        t = get_token(token, MAXTOK);
        if (is_stmt_ending(t)) {
            break;
        } else if (t == RBRACE || t == LBRACE) {
            put_back_string(token);
            break;
        } else if (t == EOF) {
            synerr_eof();
            put_back_string(token);
            break;
        }
        if (token_requires_automatic_line_continuation(t)) {
            /* a token that automatically enable a line continuation */
            while (is_newline(get_token(ptoken, MAXTOK)))
                /* empty body */;
            put_back_string(ptoken);
        }
        /* keep track of pairs of open/closed parentheses */
        if (t == LPAREN) {
            nlpar++;
            continue;
        } else if (t == RPAREN) {
            nlpar--;
            continue;
        }
        /* keep track of current function/subroutine */
        if (STREQ(token, sfunction)) {
            peek_subprog_name(SUBPRG_FUNC);
        } else if (STREQ(token, ssubroutine)) {
            peek_subprog_name(SUBPRG_SUBR);
        } else if (STREQ(token, send)) {
            current_subprogram_name[0] = EOS;
            current_subprogram_type = SUBPRG_NONE;
        }
    }

    if (nlpar != 0)
        synerr("unbalanced parentheses.");
}


/* is_all_digits - return true if str[] contins only digit characters */
static bool
is_all_digits(const char str[])
{
    int i;

    if (str[0] == EOS)
        return(false);
    for (i = 0; str[i] != EOS; i++)
        if (str[i] < DIG0 || str[i] > DIG9)
            return(false);
    return(true);
}

/* outgo - output "goto  n" */
static void
outgo(int n)
{
    if (xfer)
        return;
    outtab();
    outstr(sgoto);
    outch(BLANK);
    outnum(n);
    outdon();
}

/*
 * Public Functions.
 */

BEGIN_C_DECLS

/* verbatim - copy directly to output until the next newline character */
void
verbatim(void)
{
    outasis(infile[inclevel]);
}

/* brknxt - generate code for break n and next n; n = 1 is default */
void
brknxt(int sp, int lextyp[], int labval[], int token)
{
    int n, i;
    char t, ptoken[MAXTOK];
    const char *kword;

    if (token == LEXBREAK)
        kword = "break";
    else if (token == LEXNEXT)
        kword = "next";
    else
        abort();

    n = 0;
    t = get_nonblank_token(ptoken, MAXTOK);
    if (is_all_digits(ptoken)) { /* "break n" or "next n" */
        if ((n = string_to_integer(ptoken) - 1) < 0)
            synerr("illegal %s argument.", kword);
    } else {
        if (!is_stmt_ending(t))
            synerr("illegal %s argument.", kword);
        put_back_string(ptoken);
    }
    for (i = sp; i >= 0; i--) {
        if (lextyp[i] == LEXWHILE || lextyp[i] == LEXDO
            || lextyp[i] == LEXFOR || lextyp[i] == LEXREPEAT) {
            if (n > 0) {
                n--;
                continue; /* seek proper level */
            } else if (token == LEXBREAK) {
                outgo(labval[i]+1);
            } else {
                outgo(labval[i]);
            }
/* original value
            xfer = true;
*/
            xfer = false;
            return;
        }
    }
    synerr("illegal %s.", kword);
}

/* do_code - generate code for beginning of do */
void
do_code(int *lab)
{
    xfer = false;
    outtab();
    outstr(sdo);
    outch(BLANK);
    *lab = labgen(2);
    outnum(*lab);
    eatup();
    outdon();
}

/* do_end - generate code for end of do statement */
void
do_end(int lab)
{
    outcon(lab);
    outcon(lab+1);
}

/* elseif_code - generate code for end of if before else */
void
elseif_code(void)
{
    outtab();
    outstr(selse);
    outdon();
}

/* for_code - beginning of for statement */
void
for_code(int *lab)
{
    char t, token[MAXTOK];
    int i, j, nlpar,tlab;

    tlab = *lab;
    tlab = labgen(3);
    /* Output an unlabeled "continue", in case there was a label.  See
     * the "NOTE on unlabled continue" above for more information. */
    outcon(0);
    if (get_nonblank_token(token, MAXTOK) != LPAREN) {
        synerr("missing left paren.");
        return;
    }
    if (get_nonblank_token(token, MAXTOK) != SEMICOL) { /* real init clause */
        put_back_string(token);
        outtab();
        eatup();
        outdon();
    }
    if (get_nonblank_token(token, MAXTOK) == SEMICOL) /* empty condition */
        outcon(tlab);
    else { /* non-empty condition */
        put_back_string(token);
        outnum(tlab);
        outtab();
        outstr(sifnot);
        outch(LPAREN);
        nlpar = 0;
        while (nlpar >= 0) {
            t = get_token(token, MAXTOK);
            if (t == SEMICOL)
                break;
            if (t == LPAREN)
                nlpar++;
            else if (t == RPAREN)
                nlpar--;
            if (t == EOF) {
                put_back_string(token);
                return;
            }
            if (!is_newline(t) && t != UNDERLINE) /* XXX: underline? */
                outstr(token);
        }
        outch(RPAREN);
        outch(RPAREN);
        outgo((tlab)+2);
        if (nlpar < 0)
            synerr("invalid for clause.");
    }
    fordep++; /* stack reinit clause */
    for (j = 0, i = 1; i < fordep; i++) /* find end *** should i = 1 ??? *** */
        j += SSTRLEN(&forstk[j]) + 1;
    forstk[j] = EOS; /* null, in case no reinit */
    nlpar = 0;
    t = get_nonblank_token(token, MAXTOK);
    put_back_string(token);
    while (nlpar >= 0) {
        t = get_token(token, MAXTOK);
        if (t == LPAREN)
            nlpar++;
        else if (t == RPAREN)
            nlpar--;
        if (t == EOF) {
            put_back_string(token);
            break;
        }
        if (nlpar >= 0 && !is_newline(t) && t != UNDERLINE) { /* XXX: underline? */
            if ((j + SSTRLEN(token)) >= MAXFORSTK)
                synerr_fatal("for clause too long.");
            scopy(token, 0, forstk, j);
            j += SSTRLEN(token);
        }
    }
    tlab++; /* label for next's */
    *lab = tlab;
}

/* for_end - process end of for statement */
void
for_end(int lab)
{
    int i, j;

    xfer = false;
    outnum(lab);
    for (j = 0, i = 1; i < fordep; i++)
        j += SSTRLEN(&forstk[j]) + 1;
    if (forstk[j] != EOS) {
        outtab();
        outstr(&forstk[j]);
        outdon();
    }
    outgo(lab-1);
    outcon(lab+1);
    fordep--;
}

/* if_not_then_go_code - generate "if(.not.(...))goto lab" */
void
if_not_goto_code(int lab)
{
    outtab();       /* get to column 7 */
    outstr(sifnot); /* " if(.not." */
    balpar();       /* collect and output condition */
    outch(RPAREN);  /* " ) " */
    outgo(lab);     /* " goto lab " */
}

/* if_code - generate initial code for if */
void
if_code(int *lab)
{
    xfer = false;
    *lab = labgen(1);
    ifthenc();
}

/* if_end - generate code for end of if */
void
if_end(void)
{
    outtab();
    outstr(sendif);
    outdon();
}

/* ifthenc - generate "if((...))then" */
void
ifthenc(void)
{
    outtab();       /* get to column 7 */
    outstr(sif);    /* " if( " */
    balpar();       /* collect and output condition */
    outch(RPAREN);  /* " ) " */
    outstr(sthen);  /* " then " */
    outdon();
}

/* label_code - output statement number */
void
label_code(char lexstr[])
{

    xfer = false;  /* can't suppress goto's now */
    if (can_label_conflict(atoi(lexstr)))
        synerr("possible label conflict.");
    outstr(lexstr);
    outtab();
}

/* otherstmt - output ordinary Fortran statement */
void
otherstmt(char lexstr[])
{
    xfer = false;
    outtab();
    put_back_string(lexstr);    /* so that we can eat it up again ... */
    eatup();                    /* ... now, with the rest of the line */
    outdon();
}

/* outcon - output "lab   continue" */
void
outcon(int lab)
{
    xfer = false;
#if 0
    if (lab <= 0 && outp == 0)
        return; /* don't need unlabeled continues */
#endif
    if (lab > 0)
        outnum(lab);
    outtab();
    outstr(scontinue);
    outdon();
}

/* repeat_code - generate code for beginning of repeat */
void repeat_code(int *lab)
{
    /* Output an unlabeled "continue", in case there was a label.  See
     * the "NOTE on unlabled continue" above for more information. */
    outcon(0);
    *lab = labgen(3);
    outcon(*lab);   /* start the loop */
    *lab += 1;      /* label to go on next's */
}

/* until_code - generate code for "until(COND)" after a "repeat STMT". */
void
until_code(int lab)
{
    xfer = false;
    outcon(lab);                /* where to go on `next` */
    if_not_goto_code(lab-1);    /* collect COND, if it's false start
                                   then loop again */
    outcon(lab+1);              /* end loop */
}

/* repeat_end - generate code for end of bare "repeat STMT" */
void
repeat_end(int lab)
{
    xfer = false;
    outcon(lab);    /* where to go on `next` */
    outgo(lab-1);   /* go back to the beginning of the loop */
    outcon(lab+1);  /* end loop */
}

/* while_code - generate code for beginning of while */
void
while_code(int *lab)
{
    /* Output an unlabeled "continue", in case there was a label.  See
     * the "NOTE on unlabled continue" above for more information. */
    outcon(0);
    *lab = labgen(2); /* see while_end() to know why we need 2 labels */
    outnum(*lab);
    ifthenc();
}

/* while_end - generate code for end of while */
void
while_end(int lab)
{
    outgo(lab);
    if_end();
    outcon(lab+1); /* needed by e.g. break */
}

/* return_code - generate code for return */
void
return_code(void)
{
    char token[MAXTOK], t;

    if (current_subprogram_type == SUBPRG_NONE) {
        synerr("return statement outside any function or subroutine");
        goto gen_raw_return_stmt;
    }
    t = get_nonblank_token(token, MAXTOK);
    if (!is_stmt_ending(t) && t != RBRACE) {
        put_back_string(token);
        if (current_subprogram_type == SUBPRG_SUBR) {
            synerr("return statement with argument inside a subroutine");
            goto gen_raw_return_stmt;
        } else {
            outtab();
            outstr(current_subprogram_name);
            outch(EQUALS);
            eatup();
            outdon();
            goto gen_raw_return_stmt;
        }
    } else if (t == RBRACE) {
        put_back_string(token);
    }
gen_raw_return_stmt:
    outtab();
    outstr(sreturn);
    outdon();
    xfer = true;
}

/* caslab - get one case label */
static int
caslab (int *n, int *t)
{
    char tok[MAXTOK];
    int sign;

    *t = get_nonblank_token (tok, MAXTOK);
    while (is_newline(*t))
        *t = get_nonblank_token (tok, MAXTOK);
    if (*t == EOF)
        return(*t);
    sign = ((tok[0] == MINUS && tok[1] == EOS) ? -1 : 1);
    if ((tok[0] == MINUS || tok[0] == PLUS) && tok[1] == EOS)
        *t = get_nonblank_token (tok, MAXTOK);
    if (*t != TOKT_DIGITS) {
        synerr ("invalid case label.");
        *n = 0;
    } else {
        *n = sign * string_to_integer(tok);
    }
    do { /* ignore blank lines */;
        *t = get_nonblank_token (tok, MAXTOK);
    } while (is_newline(*t));
    return(0); /* expected to be ignored */
}

/* cascode - generate code for case or default label */
void
cascode(int lab, int token)
{
    int t, l, lb, ub, i, j, junk;
    char scrtok[MAXTOK];

    if (swtop <= 0) {
        synerr ("illegal case or default.");
        return;
    }
    outgo(lab + 1); /* # terminate previous case */
    xfer = true;
    l = labgen(1);
    if (token == LEXCASE) { /* # case n[,n]... : ... */
        while (caslab (&lb, &t) != EOF) {
            ub = lb;
            if (t == MINUS)
                junk = caslab (&ub, &t);
            if (lb > ub) {
                synerr ("illegal range in case label.");
                ub = lb;
            }
            if (swlast + 3 > MAXSWITCH)
                synerr_fatal ("switch table overflow.");
            for (i = swtop + 3; i < swlast; i += 3)
                if (lb <= swstak[i])
                    break;
                else if (lb <= swstak[i+1])
                    synerr ("duplicate case label.");
            if (i < swlast && ub >= swstak[i])
                synerr ("duplicate case label.");
            for (j = swlast; j > i; j--) /* # insert new entry */
                swstak[j+2] = swstak[j-1];
            swstak[i] = lb;
            swstak[i+1] = ub;
            swstak[i+2] = l;
            swstak[swtop + 1]++;
            swlast = swlast + 3;
            if (t == COLON)
                break;
            else if (t != COMMA)
                synerr ("illegal case syntax.");
        }
    }
    else { /* # default : ... */
        t = get_nonblank_token (scrtok, MAXTOK);
        if (swstak[swtop + 2] != 0)
            synerr_fatal ("multiple defaults in switch statement.");
        else
            swstak[swtop + 2] = l;
    }

    if (t == EOF)
        synerr_eof();
    else if (t != COLON)
        synerr_fatal ("missing colon in case or default label.");

    xfer = false;
    outcon (l);
}

/* swvar  - output switch variable Innn, where nnn = lab */
static void
swvar(int lab)
{
    outch('I');
    outnum(lab);
}

/* swcode - generate code for switch stmt. */
void
swcode(int *lab)
{
    char scrtok[MAXTOK];

    *lab = labgen (2);
    if (swlast + 3 > MAXSWITCH)
        synerr_fatal ("switch table overflow.");
    swstak[swlast] = swtop;
    swstak[swlast + 1] = 0;
    swstak[swlast + 2] = 0;
    swtop = swlast;
    swlast = swlast + 3;
    xfer = false;
    outtab(); /* # Innn=(e) */
    swvar(*lab);
    outch(EQUALS);
    balpar();
    outdon();
    outgo(*lab); /* # goto L */
    xfer = true;
    while (is_newline(get_nonblank_token(scrtok, MAXTOK)))
        /* skip empty lines, get next token */;
    if (scrtok[0] != LBRACE) {
        synerr ("missing left brace in switch statement.");
        put_back_string(scrtok);
    }
}

/* swend  - finish off switch statement; generate dispatch code */
void
swend(int lab)
{
    int lb, ub, n, i;

    lb = swstak[swtop + 3];
    ub = swstak[swlast - 2];
    n = swstak[swtop + 1];
    outgo(lab + 1); /* # terminate last case */
    if (swstak[swtop + 2] == 0)
        swstak[swtop + 2] = lab + 1; /* # default default label */
    xfer = false;
    outcon (lab); /*  L continue */
    /* output branch table */
    if (n > 0) { /* # output linear search form */
        for (i = swtop + 3; i < swlast; i = i + 3) {
            outtab(); /* # if (Innn */
            outstr (sif);
            swvar  (lab);
            if (swstak[i] == swstak[i+1]) {
                outstr (".eq."); /* #   .eq....*/
                outnum (swstak[i]);
            }
            else {
                outstr (".ge."); /* #   .ge.lb.and.Innn.le.ub */
                outnum (swstak[i]);
                outstr (".and.");
                swvar  (lab);
                outstr (".le.");
                outnum (swstak[i + 1]);
            }
            outch (RPAREN); /* #    ) goto ... */
            outgo (swstak[i + 2]);
        }
        if (lab + 1 != swstak[swtop + 2])
            outgo (swstak[swtop + 2]);
    }
    outcon (lab + 1); /* # L+1  continue */
    swlast = swtop;   /* # pop switch stack */
    swtop = swstak[swtop];
}

END_C_DECLS

/* vim: set ft=c ts=4 sw=4 et : */
