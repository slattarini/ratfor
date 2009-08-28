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

/* keywords: */

char sdo[] = "do";
char vdo[] = { LEXDO, EOS };

char sif[] = "if";
char vif[] = { LEXIF, EOS };

char selse[] = "else";
char velse[] = { LEXELSE, EOS };

#ifdef F77
char sthen[] = "then";
char sendif[] = "endif";
#endif /* F77 */

char swhile[] = "while";
char vwhile[] = { LEXWHILE, EOS };

char ssbreak[] = "break";
char vbreak[] = { LEXBREAK, EOS };

char snext[] = "next";
char vnext[] = { LEXNEXT, EOS };

char sfor[] = "for"; 
char vfor[] = { LEXFOR, EOS };

char srept[] = "repeat";
char vrept[] = { LEXREPEAT, EOS };

char suntil[] = "until";
char vuntil[] = { LEXUNTIL, EOS };

char sswitch[] = "switch";
char vswitch[] = { LEXSWITCH, EOS };

char scase[] = "case";
char vcase[] = { LEXCASE, EOS };

char sdefault[] = "default";
char vdefault[] = { LEXDEFAULT, EOS };

char sret[] = "return";
char vret[] = { LEXRETURN, EOS };

char sstr[] = "string";
char vstr[] = { LEXSTRING, EOS };

/* constant strings */

char *ifnot  = "if(.not.";
char *incl   = "include";
char *fncn   = "function";
char *contin = "continue";
char *rgoto  = "goto ";
char *dat    = "data ";
char *eoss   = "EOS/";

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
    if (equal(u_filename, "-"))
        in = stdin;
    else if ((in = fopen(u_filename, "r")) == NULL)
        error("%s: cannot open for reading\n", u_filename); /*XXX: perror?*/

    outp = 0;                   /* output character pointer */
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
    install("define", deftyp);
    install("DEFINE", deftyp);
    
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
 * L E X I C A L  A N A L Y S E R
 */

/*
 *  alldig - return YES if str is all digits
 *
 */
int
alldig(str)
char str[];
{
    int i,j;

    j = NO;
    if (str[0] == EOS)
        return(j);
    for (i = 0; str[i] != EOS; i++)
        if (type(str[i]) != DIGIT)
            return(j);
    j = YES;
    return(j);
}


/*
 * balpar - copy balanced paren string
 *
 */
balpar()
{
    char token[MAXTOK];
    int t,nlpar;

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
 * deftok - get token; process macro calls and invocations
 *
 */
int
deftok(token, toksiz, fd)
char token[];
int toksiz;
FILE *fd;
{
    char defn[MAXDEF];
    int t;

    for (t=gtok(token, toksiz, fd); t!=EOF; t=gtok(token, toksiz, fd)) {
        if (t != ALPHA) /* non-alpha */
            break;
        if (look(token, defn) == NO) /* undefined */
            break;
        if (defn[0] == DEFTYPE) { /* get definition */
            getdef(token, toksiz, defn, MAXDEF, fd);
            install(token, defn);
        }
        else
            pbstr(defn); /* push replacement onto input */
    }
    if (t == ALPHA) /* convert to single case */
        fold(token);
    return(t);
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
        if (t == COMMA || t == PLUS
                   || t == MINUS || t == STAR || t == LPAREN
                       || t == AND || t == BAR || t == BANG
                   || t == EQUALS || t == UNDERLINE ) {
            while (gettok(ptoken, MAXTOK) == NEWLINE)
                ;
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
 * getdef (for no arguments) - get name and definition
 *
 */
getdef(token, toksiz, defn, defsiz, fd)
char token[];
int toksiz;
char defn[];
int defsiz;
FILE *fd;
{
    int i, nlpar, t;
    char c, ptoken[MAXTOK];

    skpblk(fd);
    /*
     * define(name,defn) or
     * define name defn
     *
     */
    if ((t = gtok(ptoken, MAXTOK, fd)) != LPAREN) {;
        t = BLANK; /* define name defn */
        pbstr(ptoken);
    }
    skpblk(fd);
    if (gtok(token, toksiz, fd) != ALPHA)
        baderr("non-alphanumeric name.");
    skpblk(fd);
    c = gtok(ptoken, MAXTOK, fd);
    if (t == BLANK) { /* define name defn */
        pbstr(ptoken);
        i = 0;
        do {
            c = ngetch(&c, fd);
            if (i > defsiz)
                baderr("definition too long.");
            defn[i++] = c;
        } while (c != SHARP && c != NEWLINE && c != EOF && c != PERCENT);
        if (c == SHARP || c == PERCENT)
            putbak(c);
    }
    else if (t == LPAREN) { /* define (name, defn) */
        if (c != COMMA)
            baderr("missing comma in define.");
        /* else got (name, */
        nlpar = 0;
        for (i = 0; nlpar >= 0; i++)
            if (i > defsiz)
                baderr("definition too long.");
            else if (ngetch(&defn[i], fd) == EOF)
                baderr("missing right paren.");
            else if (defn[i] == LPAREN)
                nlpar++;
            else if (defn[i] == RPAREN)
                nlpar--;
        /* else normal character in defn[i] */
    }
    else
        baderr("getdef is confused.");
    defn[i-1] = EOS;
}

/*
 * gettok - get token. handles file inclusion and line numbers
 *
 */
int
gettok(token, toksiz)
char token[];
int toksiz;
{
    int t, i, j;
    int tok;
    char name[MAXNAME];

    for ( ; level >= 0; level--) {
        for (tok = deftok(token, toksiz, infile[level]); tok != EOF;
             tok = deftok(token, toksiz, infile[level]))
        {
            if (equal(token, fncn) == YES) {
                skpblk(infile[level]);
                t = deftok(fcname, MAXNAME, infile[level]);
                pbstr(fcname);
                if (t != ALPHA)
                    synerr("missing function name.");
                putbak(BLANK);
                return(tok);
            }
            else if (equal(token, incl) == NO) {
                return(tok);
            }
            /* deal with file inclusion */
            for (i = 0; ; i = strlen(name)) {
                t = deftok(&name[i], MAXNAME, infile[level]);
                if (t == NEWLINE || t == SEMICOL) {
                    pbstr(&name[i]);
                    break;
                }
            }
            name[i] = EOS;
/*WSB 6-25-91
            if (name[1] == SQUOTE) {
                outtab();
                outstr(token);
                outstr(name);
                outdon();
                eatup();
                return(tok);
            }
*/
            if (level >= NFILES)
                synerr("includes nested too deeply.");
            else {
/*XXX re-add support for quoted filenames, sooner or later
                name[i-1]=EOS;
                infile[level+1] = fopen(&name[2], "r");
*/
                /* skip leading white space in name */
                for (j = 0; name[j] == BLANK || name[j] == TAB; j++)
                    /* empty body */;
                filename[level+1] = strsave(&name[j]);
                if (filename[level+1] == NULL) {
                    synerr("cannot open include: memory error."); /*XXX improve errmsg */
                    goto include_done;
                }
                infile[level+1] = fopen(filename[level+1], "r");
                if (infile[level+1] == NULL) {
                    synerr("cannot open include: I/O error."); /*XXX improve errmsg */
                    goto include_done;
                }
                linect[level+1] = 1;
                ++level;
include_done:
                /* nothing else to do */;
            }
        }
        if (level > 0) {  /* close include and pop file name stack */
            fclose(infile[level]);
            infile[level] = NULL; /* just to be sure */
            free(filename[level]);
            filename[level] = NULL; /* just to be sure */
        }
    }
    token[0] = EOF;   /* in case called more than once */
    token[1] = EOS;
    tok = EOF;
    return(tok);
}

/*
 * gnbtok - get nonblank token
 *
 */
int
gnbtok(token, toksiz)
char token[];
int toksiz;
{
    int tok;

    skpblk(infile[level]);
    tok = gettok(token, toksiz);
    return(tok);
}

/*
 * gtok - get token for Ratfor
 *
 */
int
gtok(lexstr, toksiz, fd)
char lexstr[];
int toksiz;
FILE *fd;
{ 
    int i, b, n, tok;
    char c;
    c = ngetch(&lexstr[0], fd);
    if (c == BLANK || c == TAB) {
        lexstr[0] = BLANK;
        while (c == BLANK || c == TAB) /* compress many blanks to one */
            c = ngetch(&c, fd);
        if (c == PERCENT) {
            outasis(fd); /* copy direct to output if % */
            c = NEWLINE;
        }
        if (c == SHARP) {
            if(leaveC == YES) {
              outcmnt(fd); /* copy comments to output */
              c = NEWLINE;
            }
            else
                while (ngetch(&c, fd) != NEWLINE) /* strip comments */
                    /* empty body */;
        }
/*
        if (c == UNDERLINE)
            if(ngetch(&c, fd) == NEWLINE)
                while(ngetch(&c, fd) == NEWLINE)
                    ;
            else
            {
                putbak(c);
                c = UNDERLINE;
            }
*/
        if (c != NEWLINE)
            putbak(c);
        else
            lexstr[0] = NEWLINE;
        lexstr[1] = EOS;
        return((int)lexstr[0]);
    }
    i = 0;
    tok = type(c);
    if (tok == LETTER) { /* alpha */
        for (i = 0; i < toksiz - 3; i++) {
            tok = type(ngetch(&lexstr[i+1], fd));
            /* Test for DOLLAR added by BM, 7-15-80 */
            if (tok != LETTER && tok != DIGIT
                && tok != UNDERLINE && tok!=DOLLAR
                && tok != PERIOD)
                break;
        }
        putbak(lexstr[i+1]);
        tok = ALPHA;
    }
    else if (tok == DIGIT) { /* digits */
        b = c - DIG0; /* in case alternate base number */
        for (i = 0; i < toksiz - 3; i++) {
            if (type(ngetch(&lexstr[i+1], fd)) != DIGIT)
                break;
            b = 10*b + lexstr[i+1] - DIG0;
        }
        if (lexstr[i+1] == RADIX && b >= 2 && b <= 36) {
            /* n%ddd... */
            for (n = 0;; n = b*n + c) {
                c = ngetch(&lexstr[0], fd);
                if (c >= 'a' && c <= 'z')
                    c = c - 'a' + DIG9 + 1;
                else if (c >= 'A' && c <= 'Z')
                    c = c - 'A' + DIG9 + 1;
                if (c < DIG0 || c >= DIG0 + b)
                    break;
                c = c - DIG0;
            }
            putbak(lexstr[0]);
            i = itoc(n, lexstr, toksiz);
        }
        else
            putbak(lexstr[i+1]);
        tok = DIGIT;
    }
#ifdef SQUAREB
    else if (c == LBRACK) { /* allow [ for { */
        lexstr[0] = LBRACE;
        tok = LBRACE;
    }
    else if (c == RBRACK) { /* allow ] for } */
        lexstr[0] = RBRACE;
        tok = RBRACE;
    }
#endif
    else if (c == SQUOTE || c == DQUOTE) {
        for (i = 1; ngetch(&lexstr[i], fd) != lexstr[0]; i++) {
            if (lexstr[i] == UNDERLINE)
                if (ngetch(&c, fd) == NEWLINE) {
                    while (c == NEWLINE || c == BLANK || c == TAB)
                        c = ngetch(&c, fd);
                    lexstr[i] = c;
                }
                else
                    putbak(c);
            if (lexstr[i] == NEWLINE || i >= toksiz-1) {
                synerr("missing quote.");
                lexstr[i] = lexstr[0];
                putbak(NEWLINE);
                break;
            }
        }
    }
    else if (c == PERCENT) {
        outasis(fd);        /* direct copy of protected */
        tok = NEWLINE;
    }
    else if (c == SHARP) { 
        if(leaveC == YES)
          outcmnt(fd);      /* copy comments to output */
        else
          while (ngetch(&lexstr[0], fd) != NEWLINE) /* strip comments */
            /* empty body */;
          tok = NEWLINE;
    }
    else if (c == GREATER || c == LESS || c == NOT
             || c == BANG || c == CARET || c == EQUALS
             || c == AND || c == OR)
        i = relate(lexstr, fd);
    if (i >= toksiz-1)
        synerr("token too long.");
    lexstr[i+1] = EOS;
    if (lexstr[0] == NEWLINE)
        linect[level] = linect[level] + 1;

    /* cray cannot compare char and ints, since EOF is an int we check
       with feof */
    if (feof(fd))
        tok = EOF;

    return(tok);
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
    else if (equal(lexstr, sif) == YES)
        tok = vif[0];
    else if (equal(lexstr, selse) == YES)
        tok = velse[0];
    else if (equal(lexstr, swhile) == YES)
        tok = vwhile[0];
    else if (equal(lexstr, sdo) == YES)
        tok = vdo[0];
    else if (equal(lexstr, ssbreak) == YES)
        tok = vbreak[0];
    else if (equal(lexstr, snext) == YES)
        tok = vnext[0];
    else if (equal(lexstr, sfor) == YES)
        tok = vfor[0];
    else if (equal(lexstr, srept) == YES)
        tok = vrept[0];
    else if (equal(lexstr, suntil) == YES)
        tok = vuntil[0];
    else if (equal(lexstr, sswitch) == YES)
        tok = vswitch[0];
    else if (equal(lexstr, scase) == YES)
        tok = vcase[0];
    else if (equal(lexstr, sdefault) == YES)
        tok = vdefault[0];
    else if (equal(lexstr, sret) == YES)
        tok = vret[0];
    else if (equal(lexstr, sstr) == YES)
        tok = vstr[0];
    else
        tok = LEXOTHER;
    return(tok);
}

/*
 * relate - convert relational shorthands into long form
 *
 */
int
relate(token, fd)
char token[];
FILE *fd;
{

    if (ngetch(&token[1], fd) != EQUALS) {
        putbak(token[1]);
        token[2] = 't';
    } else {
        token[2] = 'e';
    }
    token[3] = PERIOD;
    token[4] = EOS;
    token[5] = EOS; /* for .not. and .and. */
    if (token[0] == GREATER)
        token[1] = 'g';
    else if (token[0] == LESS)
        token[1] = 'l';
    else if (token[0] == NOT || token[0] == BANG || token[0] == CARET) {
        if (token[1] != EQUALS) {
            token[2] = 'o';
            token[3] = 't';
            token[4] = PERIOD;
        }
        token[1] = 'n';
    }
    else if (token[0] == EQUALS) {
        if (token[1] != EQUALS) {
            token[2] = EOS;
            return(0);
        }
        token[1] = 'e';
        token[2] = 'q';
    }
    else if (token[0] == AND) { /* look for && or & */
        if (ngetch(&token[1], fd) != AND) 
            putbak(token[1]);
        token[1] = 'a';
        token[2] = 'n';
        token[3] = 'd';
        token[4] = PERIOD;
    }
    else if (token[0] == OR) {
        if (ngetch(&token[1], fd) != OR) /* look for || or | */ 
            putbak(token[1]);
        token[1] = 'o';
        token[2] = 'r';
    }
    else   /* can't happen */
        token[1] = EOS;
    token[0] = PERIOD;
    return(strlen(token)-1);
}

/*
 * skpblk - skip blanks and tabs in file  fd
 *
 */
skpblk(fd)
FILE *fd;
{
    char c;

    for (c = ngetch(&c, fd); c == BLANK || c == TAB; c = ngetch(&c, fd))
        /* empty body */;
    putbak(c);
}


/*
 * type - return LETTER, DIGIT or char; works with ascii alphabet
 *
 */
int
type(c)
char c;
{
    int t;

    if (c >= DIG0 && c <= DIG9)
        t = DIGIT;
    else if (isupper(c) || islower(c))
        t = LETTER;
    else
        t = c;
    return(t);
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
 * outch - put one char into output buffer
 *
 */
outch(c)
char c;
{
    int i;

    if (outp >= 72) {   /* continuation card */
        outdon();
        for (i = 0; i < 6; i++)
            outbuf[i] = BLANK;
        outbuf[5]='*';
        outp = 6;
    }
    outbuf[outp] = c;
    outp++;
}

/*
 * outcon - output "n   continue"
 *
 */
outcon(n)
int n;
{
    xfer = NO;
    if (n <= 0 && outp == 0)
        return;            /* don't need unlabeled continues */
    if (n > 0)
        outnum(n);
    outtab();
    outstr(contin);
    outdon();
}

/*
 * outdon - finish off an output line
 *
 */
outdon()
{

    outbuf[outp] = NEWLINE;
    outbuf[outp+1] = EOS;
    printf("%s", outbuf);
    outp = 0;
}

/*
 * outcmnt - copy comment to output
 *
 */
outcmnt(fd)
FILE * fd;
{
    char c;
    char comout[81];
    int i, comoutp=0;

    comoutp=1;
    comout[0]='C';
    while((c = ngetch(&c, fd)) != NEWLINE) {
        if (comoutp > 79) {
            comout[80]=NEWLINE;
            comout[81]=EOS;
            printf("%s",comout);
            comoutp=0;
            comout[comoutp]='C';
            comoutp++;
        }
        comout[comoutp]=c;
        comoutp++;
    }
    comout[comoutp]=NEWLINE;
    comout[comoutp+1]=EOS;
    printf("%s",comout);
}

/*
 * outasis - copy directly out
 *
 */
outasis(fd)
FILE * fd;
{
    char c;
    while((c = ngetch(&c, fd)) != NEWLINE)
        outch(c);
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
 * outnum - output decimal number
 *
 */
outnum(n)
int n;
{

    char chars[MAXCHARS];
    int i, m;

    m = abs(n);
    i = -1;
    do {
        i++;
        chars[i] = (m % 10) + DIG0;
        m = m / 10;
    }
    while (m > 0 && i < MAXCHARS);
    if (n < 0)
        outch(MINUS);
    for ( ; i >= 0; i--)
        outch(chars[i]);
}



/*
 * outstr - output string
 *
 */
outstr(str)
char str[];
{
    int i;

    for (i=0; str[i] != EOS; i++)
        outch(str[i]);
}

/*
 * outtab - get past column 6
 *
 */
outtab()
{
    while (outp < 6)
        outch(BLANK);
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
