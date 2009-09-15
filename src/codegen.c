#include "rat4-common.h"
#include "rat4-global.h"

#include "utils.h"
#include "io.h"
#include "error.h"
#include "tokenizer.h"
#include "lex-symbols.h"
#include "labgen.h"
#include "codegen.h"

#define MAXSTRNAME MAXFUNCNAME /* max length for a string name */
#define MAXSWITCH  300         /* max stack for switch statement */
#define MAXFORSTK  200         /* max space for for reinit clauses */

/*
 *  C O N S T A N T  S T R I N G S
 */

static const char scontinue[]   = "continue";
static const char sdata[]       = "data";
static const char sdo[]         = "do";
static const char sif[]         = "if(";
static const char sifnot[]      = "if(.not.";
#ifdef F77
static const char sthen[]       = "then";
static const char sendif[]      = "endif";
#endif /* F77 */
static const char sgoto[]       = "goto ";
static const char sreturn[]     = "return";

/*
 *  G L O B A L  V A R I A B L E S
 */

static int swtop = 0;   /* switch stack index */
static int swlast = 1;  /* switch stack index */
static int fordep = 0;  /* for stack */
static bool xfer;  /*  true if just made transfer, false otherwise */
                   /* XXX how to initialize it? */

static int swstak[MAXSWITCH];   /* switch information stack */
static char forstk[MAXFORSTK];  /* stack of reinit strings  */

/*
 *  C O D E  G E N E R A T I O N
 */


/*
 * balpar - copy balanced paren string
 *
 */
static void
balpar(void)
{
    char token[MAXTOK];
    int t, nlpar;

    if (gnbtok(token, MAXTOK) != LPAREN) {
        synerr("missing left paren.");
        return;
    }
    outstr(token);
    nlpar = 1;
    do {
        t = gettok(token, MAXTOK);
        if (t == SEMICOL || t == LBRACE || t == RBRACE || t == EOF) {
            pbstr(token);
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

/*
 * eatup - process rest of statement; interpret continuations
 *
 */
static void
eatup(void)
{

    char ptoken[MAXTOK], token[MAXTOK];
    int nlpar, t;

    nlpar = 0;
    do {
        t = gettok(token, MAXTOK);
        if (is_stmt_ending(t))
            break;
        if (t == RBRACE || t == LBRACE) {
            pbstr(token);
            break;
        }
        if (t == EOF) {
            synerr_eof();
            pbstr(token);
            break;
        }
        /* check for tokens that automatically enable a line
         * continuation */
        if (t == COMMA || t == PLUS || t == MINUS || t == STAR
            || t == LPAREN || t == AND || t == BAR || t == BANG
            || t == EQUALS || t == UNDERLINE)
        {
            while (is_newline(gettok(ptoken, MAXTOK)))
                /* empty body */;
            pbstr(ptoken);
            if (t == UNDERLINE) /* XXX: ??? */
                token[0] = EOS;
        }
        if (t == LPAREN)
            nlpar++;
        else if (t == RPAREN)
            nlpar--;
        outstr(token);

    } while (nlpar >= 0);

    if (nlpar != 0)
        synerr("unbalanced parentheses.");
}


/*
 *  alldig - return true if str is all digits
 *
 */
static bool
alldig(const char str[])
{
    int i;

    if (str[0] == EOS)
        return(false);
    for (i = 0; str[i] != EOS; i++)
        if (str[i] < DIG0 || str[i] > DIG9)
            return(false);
    return(true);
}

/*
 * outgo - output "goto  n"
 *
 */
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
 * brknxt - generate code for break n and next n; n = 1 is default
 */
void
brknxt(int sp, int lextyp[], int labval[], int token)
{
    int i, n;
    char t, ptoken[MAXTOK];

    n = 0;
    t = gnbtok(ptoken, MAXTOK);
    if (alldig(ptoken)) { /* have break n or next n */
        i = 0;
        n = ctoi(ptoken, &i) - 1;
    } else if (t != SEMICOL) /* default case */
        pbstr(ptoken);
    for (i = sp; i >= 0; i--)
        if (lextyp[i] == LEXWHILE || lextyp[i] == LEXDO
            || lextyp[i] == LEXFOR || lextyp[i] == LEXREPEAT) {
            if (n > 0) {
                n--;
                continue; /* seek proper level */
            }
            else if (token == LEXBREAK)
                outgo(labval[i]+1);
            else
                outgo(labval[i]);
/* original value
            xfer = true;
*/
            xfer = false;
            return;
        }
    if (token == LEXBREAK)
        synerr("illegal break.");
    else
        synerr("illegal next.");
    return;
}

/*
 * docode - generate code for beginning of do
 *
 */
void
docode(int *lab)
{
    xfer = false;
    outtab();
    outstr(sdo);
    *lab = labgen(2);
    outnum(*lab);
    eatup();
    outdon();
}

/*
 * dostat - generate code for end of do statement
 *
 */
void
dostat(int lab)
{
    outcon(lab);
    outcon(lab+1);
}

/*
 * elseifc - generate code for end of if before else
 *
 */
void
elseifc(int lab)
{

#ifdef F77
    outtab();
    outstr(selse);
    outdon();
#else
    outgo(lab+1);
    outcon(lab);
#endif /* F77 */
}

/*
 * forcode - beginning of for statement
 *
 */
void
forcode(int *lab)
{
    char t, token[MAXTOK];
    int i, j, nlpar,tlab;

    tlab = *lab;
    tlab = labgen(3);
    outcon(0);
    if (gnbtok(token, MAXTOK) != LPAREN) {
        synerr("missing left paren.");
        return;
    }
    if (gnbtok(token, MAXTOK) != SEMICOL) { /* real init clause */
        pbstr(token);
        outtab();
        eatup();
        outdon();
    }
    if (gnbtok(token, MAXTOK) == SEMICOL) /* empty condition */
        outcon(tlab);
    else { /* non-empty condition */
        pbstr(token);
        outnum(tlab);
        outtab();
        outstr(sifnot);
        outch(LPAREN);
        nlpar = 0;
        while (nlpar >= 0) {
            t = gettok(token, MAXTOK);
            if (t == SEMICOL)
                break;
            if (t == LPAREN)
                nlpar++;
            else if (t == RPAREN)
                nlpar--;
            if (t == EOF) {
                pbstr(token);
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
    j = 0;
    for (i = 1; i < fordep; i++) /* find end *** should i = 1 ??? *** */
        j += strlen(&forstk[j]) + 1;
    forstk[j] = EOS; /* null, in case no reinit */
    nlpar = 0;
    t = gnbtok(token, MAXTOK);
    pbstr(token);
    while (nlpar >= 0) {
        t = gettok(token, MAXTOK);
        if (t == LPAREN)
            nlpar++;
        else if (t == RPAREN)
            nlpar--;
        if (t == EOF) {
            pbstr(token);
            break;
        }
        if (nlpar >= 0 && !is_newline(t) && t != UNDERLINE) { /* XXX: underline? */
            if ((j + strlen(token)) >= MAXFORSTK)
                synerr_fatal("for clause too long.");
            scopy(token, 0, forstk, j);
            j += strlen(token);
        }
    }
    tlab++; /* label for next's */
    *lab = tlab;
}

/*
 * fors - process end of for statement
 *
 */
void
fors(int lab)
{
    int i, j;

    xfer = false;
    outnum(lab);
    j = 0;
    for (i = 1; i < fordep; i++)
        j = j + strlen(&forstk[j]) + 1;
    if (strlen(&forstk[j]) > 0) {
        outtab();
        outstr(&forstk[j]);
        outdon();
    }
    outgo(lab-1);
    outcon(lab+1);
    fordep--;
}

/*
 * ifgo - generate "if(.not.(...))goto lab"
 *
 */
static void
ifgo(int lab)
{

    outtab();       /* get to column 7 */
    outstr(sifnot); /* " if(.not. " */
    balpar();       /* collect and output condition */
    outch(RPAREN);  /* " ) " */
    outgo(lab);     /* " goto lab " */
}

/*
 * ifcode - generate initial code for if
 *
 */
void
ifcode(int *lab)
{

    xfer = false;
    *lab = labgen(2);
#ifdef F77
    ifthenc();
#else
    ifgo(*lab);
#endif /* F77 */
}

#ifdef F77
/*
 * ifend - generate code for end of if
 *
 */
void
ifend(void)
{
    outtab();
    outstr(sendif);
    outdon();
}

/*
 * ifthenc - generate "if((...))then"
 *
 */
void
ifthenc(void)
{
    outtab();
    outstr(sif);
    balpar();
    outstr(sthen);
    outdon();
}
#endif /* F77 */

/*
 * labelc - output statement number
 *
 */
void
labelc(char lexstr[])
{

    xfer = false;  /* can't suppress goto's now */
    if (can_label_conflict(atoi(lexstr)))
        synerr("possible label conflict.");
    outstr(lexstr);
    outtab();
}

/*
 * otherstmt - output ordinary Fortran statement
 *
 */
void
otherstmt(char lexstr[])
{
    xfer = false;
    outtab();
    outstr(lexstr);
    eatup();
    outdon();
}

/*
 * outcon - output "lab   continue"
 *
 */
void
outcon(int lab)
{
    xfer = false;
#if 0
    if (n <= 0 && outp == 0)
        return; /* don't need unlabeled continues */
#endif
    if (lab > 0)
        outnum(lab);
    outtab();
    outstr(scontinue);
    outdon();
}

/*
 * repcod - generate code for beginning of repeat
 *
 */
void repcode(int *lab)
{

    int tlab;

    tlab = *lab;
    outcon(0); /* in case there was a label */
    tlab = labgen(3);
    outcon(tlab);
    *lab = ++tlab; /* label to go on next's */
}

/*
 * retcode - generate code for return
 *
 */
void
retcode(void)
{
    char token[MAXTOK], t;

    t = gnbtok(token, MAXTOK);
    if (!is_stmt_ending(t) && t != RBRACE) {
        pbstr(token);
        outtab();
        outstr(current_function_name);
        outch(EQUALS);
        eatup();
        outdon();
    }
    else if (t == RBRACE)
        pbstr(token);
    outtab();
    outstr(sreturn);
    outdon();
    xfer = true;
}


/*
 * untils - generate code for until or end of repeat
 *
 */
void
untils(int lab, int token)
{
    char ptoken[MAXTOK];

    xfer = false;
    outnum(lab);
    if (token == LEXUNTIL) {
        while (is_newline(gnbtok(ptoken, MAXTOK)))
            /* skip empty lines, get next token */;
        ifgo(lab-1);
    }
    else
        outgo(lab-1);
    outcon(lab+1);
}

/*
 * whilecode - generate code for beginning of while
 *
 */
void
whilecode(int *lab)
{
    int tlab;

    tlab = *lab;
    outcon(0); /* unlabeled continue, in case there was a label */
    tlab = labgen(2);
    outnum(tlab);
#ifdef F77
    ifthenc();
#else
    ifgo(tlab+1);
#endif /* F77 */
    *lab = tlab;
}

/*
 * whiles - generate code for end of while
 *
 */
void
whiles(int lab)
{

    outgo(lab);
#ifdef F77
    ifend();
#endif /* F77 */
    outcon(lab+1);
}

/*
 * caslab - get one case label
 *
 */
static int
caslab (int *n, int *t)
{
    char tok[MAXTOK];
    int i, s;

    *t = gnbtok (tok, MAXTOK);
    while (is_newline(*t))
        *t = gnbtok (tok, MAXTOK);
    if (*t == EOF)
        return(*t);
    if (*t == MINUS)
        s = -1;
    else
        s = 1;
    if (*t == MINUS || *t == PLUS)
        *t = gnbtok (tok, MAXTOK);
    if (*t != DIGIT) {
        synerr ("invalid case label.");
        *n = 0;
    }
    else {
        i = 0;
        *n = s * ctoi (tok, &i);
    }
    do { /* ignore blank lines */; 
        *t = gnbtok (tok, MAXTOK);
    } while (is_newline(*t));
    return(0); /* expected to be ignored */
}

/*
 * cascode - generate code for case or default label
 *
 */
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
        t = gnbtok (scrtok, MAXTOK);
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

/*
 * swvar  - output switch variable Innn, where nnn = lab
 */
static void
swvar(int lab)
{
    outch('I');
    outnum(lab);
}

/*
 * swcode - generate code for switch stmt.
 *
 */
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
    while (is_newline(gnbtok(scrtok, MAXTOK)))
        /* skip empty lines, get next token */;
    if (scrtok[0] != LBRACE) {
        synerr ("missing left brace in switch statement.");
        pbstr (scrtok);
    }
}

/*
 * swend  - finish off switch statement; generate dispatch code
 *
 */
void
swend(int lab)
{
    int lb, ub, n, i;

    static const char eq[]  = ".eq.";
    static const char ge[]  = ".ge.";
    static const char le[]  = ".le.";
    static const char and[] = ".and.";

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
                outstr (eq); /* #   .eq....*/
                outnum (swstak[i]);
            }
            else {
                outstr (ge); /* #   .ge.lb.and.Innn.le.ub */
                outnum (swstak[i]);
                outstr (and);
                swvar  (lab);
                outstr (le);
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

/* vim: set ft=c ts=4 sw=4 et : */
