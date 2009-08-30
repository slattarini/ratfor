/* $Id$ */
/* Big scary code of ratfor preprocessor */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "ratdef.h"
#include "ratcom.h"
#include "utils.h"
#include "lookup.h"
#include "error.h"
#include "io.h"
#include "rat4.h"


/* KEYWORDS */

static const char sdo[] = "do";
static const char vdo[] = { LEXDO, EOS };

static const char sif[] = "if";
static const char vif[] = { LEXIF, EOS };

static const char selse[] = "else";
static const char velse[] = { LEXELSE, EOS };

#ifdef F77
static const char sthen[] = "then";
static const char sendif[] = "endif";
#endif /* F77 */

static const char swhile[] = "while";
static const char vwhile[] = { LEXWHILE, EOS };

static const char ssbreak[] = "break";
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


/* CONSTANT STRINGS */

static const char ifnot[]  = "if(.not.";
static const char contin[] = "continue";
static const char rgoto[]  = "goto ";
static const char dat[]    = "data ";
static const char eoss[]   = "EOS/";


/*
 * initialisation
 */
void
init(int u_startlab, int u_leaveC, char *u_filename)
{
    int i;
    FILE *in;
    
    startlab = u_startlab;
    leaveC = u_leaveC;
    
    /* XXX wrap this in a function */
    if (STREQ(u_filename, "-"))
        in = stdin;
    else if ((in = fopen(u_filename, "r")) == NULL)
        error("%s: cannot open for reading\n", u_filename); /*XXX: perror?*/

    level = 0;                  /* file control */
    linect[0] = 1;              /* line count of first file */
    filename[0] = u_filename;   /* filename of first file */
    infile[0] = in;             /* file handle of first file */
    fordep = 0;                 /* for stack */
    swtop = 0;                  /* switch stack index */
    swlast = 1;                 /* switch stack index */
    
    for (i = 0; i <= 126; i++)
        tabptr[i] = 0;
    
    /* default definitions */
    static char deftyp[] = { DEFTYPE, EOS };
    install("define", deftyp); /*XXX*/
    install("DEFINE", deftyp); /*XXX*/
    
    fcname[0] = EOS;  /* current function name */
    label = startlab; /* next generated label */
}


/*
 * P A R S E R
 */

void
parse(void)
{
    char lexstr[MAXTOK];
    int lab, labval[MAXSTACK], lextyp[MAXSTACK], sp, i, token;

    sp = 0;
    lextyp[0] = EOF;
    for (token = lex(lexstr); token != EOF; token = lex(lexstr)) {
        if (token == LEXIF)
            ifcode(&lab);
        else if (token == LEXDO)
            docode(&lab);
        else if (token == LEXWHILE)
            whilec(&lab);
        else if (token == LEXFOR)
            forcod(&lab);
        else if (token == LEXREPEAT)
            repcod(&lab);
        else if (token == LEXSWITCH)
            swcode(&lab);
        else if (token == LEXCASE || token == LEXDEFAULT) {
            for (i = sp; i >= 0; i--)
                if (lextyp[i] == LEXSWITCH)
                    break;
            if (i < 0)
                synerr("illegal case of default.");
            else
                cascod(labval[i], token);
        }
        else if (token == LEXDIGITS)
            labelc(lexstr);
        else if (token == LEXELSE) {
            if (lextyp[sp] == LEXIF)
                elseif(labval[sp]);
            else
                synerr("illegal else.");
        }
        if (token == LEXIF || token == LEXELSE || token == LEXWHILE
            || token == LEXFOR || token == LEXREPEAT
            || token == LEXDO || token == LEXDIGITS
            || token == LEXSWITCH || token == LBRACE) {
            sp++;         /* beginning of statement */
            if (sp > MAXSTACK)
                baderr("stack overflow in parser.");
            lextyp[sp] = token;     /* stack type and value */
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
                    swend(labval[sp]);
                    sp--;
                }
                else
                    synerr("illegal right brace.");
            }
            else if (token == LEXOTHER)
                otherc(lexstr);
            else if (token == LEXBREAK || token == LEXNEXT)
                brknxt(sp, lextyp, labval, token);
            else if (token == LEXRETURN)
                retcod();
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


/*
 *  L E X I C A L  A N A L Y S E R
 */


/*
 *  alldig - return YES if str is all digits
 *
 */
int
alldig(const char str[])
{
    int i;

    if (str[0] == EOS)
        return(NO);
    for (i = 0; str[i] != EOS; i++)
        if (!isdigit(str[i]))
            return(NO);
    return(YES);
}

/*
 * lex - return lexical type of token
 *
 */
int
lex(lexstr)
char lexstr[];
{

    int tok;

    for (tok = gnbtok(lexstr, MAXTOK);
         tok == NEWLINE; tok = gnbtok(lexstr, MAXTOK))
            /* empty body */;
    if (tok == EOF || tok == SEMICOL || tok == LBRACE || tok == RBRACE)
        return(tok);
    if (tok == DIGIT)
        tok = LEXDIGITS;
    else if (STREQ(lexstr, sif) == YES)
        tok = vif[0];
    else if (STREQ(lexstr, selse) == YES)
        tok = velse[0];
    else if (STREQ(lexstr, swhile) == YES)
        tok = vwhile[0];
    else if (STREQ(lexstr, sdo) == YES)
        tok = vdo[0];
    else if (STREQ(lexstr, ssbreak) == YES)
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

/*
 * balpar - copy balanced paren string
 *
 */
balpar()
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
        if (t == NEWLINE) /* delete newlines */
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
void
eatup()
{

    char ptoken[MAXTOK], token[MAXTOK];
    int nlpar, t;

    nlpar = 0;
    do {
        t = gettok(token, MAXTOK);
        if (t == SEMICOL || t == NEWLINE)
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
        if (t == COMMA || t == PLUS || t == MINUS || t == STAR
            || t == LPAREN || t == AND || t == BAR || t == BANG
            || t == EQUALS || t == UNDERLINE ) {
            while (gettok(ptoken, MAXTOK) == NEWLINE)
                /* empty body */;
            pbstr(ptoken);
            if (t == UNDERLINE)
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
 *  C O D E  G E N E R A T I O N
 */

/*
 * brknxt - generate code for break n and next n; n = 1 is default
 */
brknxt(sp, lextyp, labval, token)
int sp;
int lextyp[];
int labval[];
int token;
{
    int i, n;
    char t, ptoken[MAXTOK];

    n = 0;
    t = gnbtok(ptoken, MAXTOK);
    if (alldig(ptoken) == YES) {     /* have break n or next n */
        i = 0;
        n = ctoi(ptoken, &i) - 1;
    }
    else if (t != SEMICOL)      /* default case */
        pbstr(ptoken);
    for (i = sp; i >= 0; i--)
        if (lextyp[i] == LEXWHILE || lextyp[i] == LEXDO
            || lextyp[i] == LEXFOR || lextyp[i] == LEXREPEAT) {
            if (n > 0) {
                n--;
                continue;             /* seek proper level */
            }
            else if (token == LEXBREAK)
                outgo(labval[i]+1);
            else
                outgo(labval[i]);
/* original value
            xfer = YES;
*/
            xfer = NO;
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
docode(lab)
int *lab;
{
    xfer = NO;
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
dostat(lab)
int lab;
{
    outcon(lab);
    outcon(lab+1);
}

/*
 * elseif - generate code for end of if before else
 *
 */
elseif(lab)
int lab;
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
 * forcod - beginning of for statement
 *
 */
forcod(lab)
int *lab;
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
    if (gnbtok(token, MAXTOK) != SEMICOL) {   /* real init clause */
        pbstr(token);
        outtab();
        eatup();
        outdon();
    }
    if (gnbtok(token, MAXTOK) == SEMICOL)   /* empty condition */
        outcon(tlab);
    else {   /* non-empty condition */
        pbstr(token);
        outnum(tlab);
        outtab();
        outstr(ifnot);
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
            if (t != NEWLINE && t != UNDERLINE)
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
    for (i = 1; i < fordep; i++)   /* find end *** should i = 1 ??? *** */
        j = j + strlen(&forstk[j]) + 1;
    forstk[j] = EOS;   /* null, in case no reinit */
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
        if (nlpar >= 0 && t != NEWLINE && t != UNDERLINE) {
            if ((j + strlen(token)) >= MAXFORSTK)
                baderr("for clause too long.");
            scopy(token, 0, forstk, j);
            j = j + strlen(token);
        }
    }
    tlab++;   /* label for next's */
    *lab = tlab;
}

/*
 * fors - process end of for statement
 *
 */
fors(lab)
int lab;
{
    int i, j;

    xfer = NO;
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
 * ifcode - generate initial code for if
 *
 */
ifcode(lab)
int *lab;
{

    xfer = NO;
    *lab = labgen(2);
#ifdef F77
    ifthen();
#else
    ifgo(*lab);
#endif /* F77 */
}

#ifdef F77
/*
 * ifend - generate code for end of if
 *
 */
ifend()
{
    outtab();
    outstr(sendif);
    outdon();
}
#endif /* F77 */

/*
 * ifgo - generate "if(.not.(...))goto lab"
 *
 */
ifgo(lab)
int lab;
{

    outtab();      /* get to column 7 */
    outstr(ifnot); /* " if(.not. " */
    balpar();      /* collect and output condition */
    outch(RPAREN); /* " ) " */
    outgo(lab);    /* " goto lab " */
}

#ifdef F77
/*
 * ifthen - generate "if((...))then"
 *
 */
ifthen()
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
labelc(lexstr)
char lexstr[];
{

    xfer = NO;   /* can't suppress goto's now */
    if (strlen(lexstr) == 5)   /* warn about 23xxx labels */
        if (atoi(lexstr) >= startlab)
            synerr("warning: possible label conflict.");
    outstr(lexstr);
    outtab();
}

/*
 * labgen - generate  n  consecutive labels, return first one
 *
 */
int
labgen(n)
int n;
{
    int i;

    i = label;
    label = label + n;
    return(i);
}

/*
 * otherc - output ordinary Fortran statement
 *
 */
otherc(lexstr)
char lexstr[];
{
    xfer = NO;
    outtab();
    outstr(lexstr);
    eatup();
    outdon();
}

/*
 * outcon - output "n   continue"
 *
 */
outcon(n)
int n;
{
    xfer = NO;
#if 0
    if (n <= 0 && outp == 0)
        return; /* don't need unlabeled continues */
#endif
    if (n > 0)
        outnum(n);
    outtab();
    outstr(contin);
    outdon();
}

/*
 * outgo - output "goto  n"
 *
 */
outgo(n)
int n;
{
    if (xfer == YES)
        return;
    outtab();
    outstr(rgoto);
    outnum(n);
    outdon();
}

/*
 * repcod - generate code for beginning of repeat
 *
 */
repcod(lab)
int *lab;
{

    int tlab;

    tlab = *lab;
    outcon(0); /* in case there was a label */
    tlab = labgen(3);
    outcon(tlab);
    *lab = ++tlab; /* label to go on next's */
}

/*
 * retcod - generate code for return
 *
 */
retcod()
{
    char token[MAXTOK], t;

    t = gnbtok(token, MAXTOK);
    if (t != NEWLINE && t != SEMICOL && t != RBRACE) {
        pbstr(token);
        outtab();
        outstr(fcname);
        outch(EQUALS);
        eatup();
        outdon();
    }
    else if (t == RBRACE)
        pbstr(token);
    outtab();
    outstr(sret);
    outdon();
    xfer = YES;
}


/* strdcl - generate code for string declaration */
strdcl()
{
    char t, name[MAXNAME], init[MAXTOK];
    int i, len;

    t = gnbtok(name, MAXNAME);
    if (t != ALPHA)
        synerr("missing string name.");
    if (gnbtok(init, MAXTOK) != LPAREN) {
        /* make size same as initial value */
        len = strlen(init) + 1;
        if (init[1] == SQUOTE || init[1] == DQUOTE)
            len = len - 2;
    }
    else { /* form is string name(size) init */
        t = gnbtok(init, MAXTOK);
        i = 0;
        len = ctoi(init, &i);
        if (init[i] != EOS)
            synerr("invalid string size.");
        if (gnbtok(init, MAXTOK) != RPAREN)
            synerr("missing right paren.");
        else
            t = gnbtok(init, MAXTOK);
    }
    outtab();
    /*
     *   outstr(int);
     */
    outstr(name);
    outch(LPAREN);
    outnum(len);
    outch(RPAREN);
    outdon();
    outtab();
    outstr(dat);
    len = strlen(init) + 1;
    if (init[0] == SQUOTE || init[0] == DQUOTE) {
        init[len-1] = EOS;
        scopy(init, 1, init, 0);
        len = len - 2;
    }
    for (i = 1; i <= len; i++) {
        /* put out variable names */
        outstr(name);
        outch(LPAREN);
        outnum(i);
        outch(RPAREN);
        if (i < len)
            outch(COMMA);
        else
            outch(SLASH);
        ;
    }
    for (i = 0; init[i] != EOS; i++) {
        /* put out init */
        outnum(init[i]);
        outch(COMMA);
    }
    pbstr(eoss); /* push back EOS for subsequent substitution */
}


/*
 * unstak - unstack at end of statement
 *
 */
unstak(sp, lextyp, labval, token)
int *sp;
int lextyp[];
int labval[];
char token;
{
    int tp;

    tp = *sp;
    for ( ; tp > 0; tp--) {
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

/*
 * untils - generate code for until or end of repeat
 *
 */
untils(lab, token)
int lab;
int token;
{
    char ptoken[MAXTOK];

    xfer = NO;
    outnum(lab);
    if (token == LEXUNTIL) {
        lex(ptoken);
        ifgo(lab-1);
    }
    else
        outgo(lab-1);
    outcon(lab+1);
}

/*
 * whilec - generate code for beginning of while
 *
 */
whilec(lab)
int *lab;
{
    int tlab;

    tlab = *lab;
    outcon(0); /* unlabeled continue, in case there was a label */
    tlab = labgen(2);
    outnum(tlab);
#ifdef F77
    ifthen();
#else
    ifgo(tlab+1);
#endif /* F77 */
    *lab = tlab;
}

/*
 * whiles - generate code for end of while
 *
 */
whiles(lab)
int lab;
{

    outgo(lab);
#ifdef F77
    ifend();
#endif /* F77 */
    outcon(lab+1);
}

/*
 * cascod - generate code for case or default label
 *
 */
cascod (lab, token)
int lab;
int token;
{
    int t, l, lb, ub, i, j, junk;
    char scrtok[MAXTOK];

    if (swtop <= 0) {
        synerr ("illegal case or default.");
        return;
    }
    outgo(lab + 1); /* # terminate previous case */
    xfer = YES;
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
                baderr ("switch table overflow.");
            for (i = swtop + 3; i < swlast; i = i + 3)
                if (lb <= swstak[i])
                    break;
                else if (lb <= swstak[i+1])
                    synerr ("duplicate case label.");
            if (i < swlast && ub >= swstak[i])
                synerr ("duplicate case label.");
            for (j = swlast; j > i; j--) /* # insert new entry */
                swstak[j+2] = swstak[j-1];
            swstak[i] = lb;
            swstak[i + 1] = ub;
            swstak[i + 2] = l;
            swstak[swtop + 1] = swstak[swtop + 1]  +  1;
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
            baderr ("multiple defaults in switch statement.");
        else
            swstak[swtop + 2] = l;
    }

    if (t == EOF)
        synerr_eof();
    else if (t != COLON)
        baderr ("missing colon in case or default label.");

    xfer = NO;
    outcon (l);
}

/*
 * caslab - get one case label
 *
 */
int
caslab (n, t)
int *n;
int *t;
{
    char tok[MAXTOK];
    int i, s;

    *t = gnbtok (tok, MAXTOK);
    while (*t == NEWLINE)
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
    *t = gnbtok (tok, MAXTOK);
    while (*t == NEWLINE)
        *t = gnbtok (tok, MAXTOK);
}

/*
 * swcode - generate code for switch stmt.
 *
 */
swcode (lab)
int *lab;
{
    char scrtok[MAXTOK];

    *lab = labgen (2);
    if (swlast + 3 > MAXSWITCH)
        baderr ("switch table overflow.");
    swstak[swlast] = swtop;
    swstak[swlast + 1] = 0;
    swstak[swlast + 2] = 0;
    swtop = swlast;
    swlast = swlast + 3;
    xfer = NO;
    outtab(); /* # Innn=(e) */
    swvar(*lab);
    outch(EQUALS);
    balpar();
    outdon();
    outgo(*lab); /* # goto L */
    xfer = YES;
    while (gnbtok(scrtok, MAXTOK) == NEWLINE)
        /* empty body */;
    if (scrtok[0] != LBRACE) {
        synerr ("missing left brace in switch statement.");
        pbstr (scrtok);
    }
}

/*
 * swend  - finish off switch statement; generate dispatch code
 *
 */
swend(lab)
int lab;
{
    int lb, ub, n, i, j;

    static char *sif    = "if (";
    static char *slt    = ".lt.1.or.";
    static char *sgt    = ".gt.";
    static char *sgoto  = "goto (";
    static char *seq    = ".eq.";
    static char *sge    = ".ge.";
    static char *sle    = ".le.";
    static char *sand   = ".and.";

    lb = swstak[swtop + 3];
    ub = swstak[swlast - 2];
    n = swstak[swtop + 1];
    outgo(lab + 1); /* # terminate last case */
    if (swstak[swtop + 2] == 0)
        swstak[swtop + 2] = lab + 1; /* # default default label */
    xfer = NO;
    outcon (lab); /*  L continue */
    /* output branch table */
#if 0
    if (n >= CUTOFF && ub - lb < DENSITY * n) {
        if (lb != 0) {  /* L  Innn=Innn-lb */
            outtab();
            swvar  (lab);
            outch (EQUALS);
            swvar  (lab);
            if (lb < 0)
                outch (PLUS);
            outnum (-lb + 1);
            outdon();
        }
        outtab(); /*  if (Innn.lt.1.or.Innn.gt.ub-lb+1)goto default */
        outstr (sif);
        swvar  (lab);
        outstr (slt);
        swvar  (lab);
        outstr (sgt);
        outnum (ub - lb + 1);
        outch (RPAREN);
        outgo (swstak[swtop + 2]);
        outtab();
        outstr (sgoto); /* goto ... */
        j = lb;
        for (i = swtop + 3; i < swlast; i = i + 3) {
            /* # fill in vacancies */
            for ( ; j < swstak[i]; j++) {
                outnum(swstak[swtop + 2]);
                outch(COMMA);
            }
            for (j = swstak[i + 1] - swstak[i]; j >= 0; j--)
                outnum(swstak[i + 2]); /* # fill in range */
            j = swstak[i + 1] + 1;
            if (i < swlast - 3)
                outch(COMMA);
        }
        outch(RPAREN);
        outch(COMMA);
        swvar(lab);
        outdon();
    }
    else if (n > 0) { /* # output linear search form */
#else
    if (n > 0) { /* # output linear search form */
#endif
        for (i = swtop + 3; i < swlast; i = i + 3) {
            outtab(); /* # if (Innn */
            outstr (sif);
            swvar  (lab);
            if (swstak[i] == swstak[i+1]) {
                outstr (seq); /* #   .eq....*/
                outnum (swstak[i]);
            }
            else {
                outstr (sge); /* #   .ge.lb.and.Innn.le.ub */
                outnum (swstak[i]);
                outstr (sand);
                swvar  (lab);
                outstr (sle);
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

/*
 * swvar  - output switch variable Innn, where nnn = lab
 */
swvar  (lab)
int lab;
{
    outch ('I');
    outnum (lab);
}

/* vim: set ft=c ts=4 sw=4 et : */
