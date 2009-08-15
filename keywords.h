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
