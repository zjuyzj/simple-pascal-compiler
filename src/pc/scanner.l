%option noyywrap
%option yylineno

%{
#include "include/node_const.hpp"
#include "include/node_type.hpp"
#include "include/node_var.hpp"
#include "include/node_func.hpp"
#include "include/node_stmt.hpp"
#include "include/node_expr.hpp"
#include "include/node_block.hpp"
#include "parser.tab.hpp"
#include <cstdio>

extern int yylex(void);

void to_lowercase(char *str){
    for (char *p = str; *p; p++)
        if (*p >= 'A' && *p <= 'Z') *p |= 32;
}

%}

Comment "{"[^"}"]*"}"
StringElement [^"'"]|('')
Digit [0-9]
Letter [a-zA-Z]
DigitSequence {Digit}+
Character "'"{StringElement}"'"
CharacterString "'"{StringElement}+"'"
Sign "+"|"-"
ScaleFactor {Sign}?{DigitSequence}
UnsignedReal {DigitSequence}"."{DigitSequence}("e"{ScaleFactor})?|{DigitSequence}"e"{ScaleFactor}
UnsignedInteger {DigitSequence}
Identifier {Letter}({Letter}|{Digit})*

%%

(?i:true) { return SID_TRUE; }
(?i:false) { return SID_FALSE; }
(?i:maxint) { return SID_MAXINT; }

(?i:boolean) { return SID_BOOLEAN; }
(?i:char) { return SID_CHAR; }
(?i:integer) { return SID_INTEGER; }
(?i:real) { return SID_REAL; }
(?i:string) { return SID_STRING; }

(?i:read) { return SID_READ; }
(?i:write) { return SID_WRITE; }
(?i:writeln) { return SID_WRITELN; }

"+" { return SYM_ADD; }
"-" { return SYM_SUB; }
"*" { return SYM_MUL; }
"/" { return SYM_DIV; }

"="  { return SYM_EQ; }
"<>" { return SYM_NE; }
"<"  { return SYM_LT; }
">"  { return SYM_GT; }
"<=" { return SYM_LE; }
">=" { return SYM_GE; }

"." { return SYM_DOT; }
"," { return SYM_COMMA; }
":" { return SYM_COL; }
";" { return SYM_SEMI; }
":=" { return SYM_ASSIGN; }
".." { return SYM_DDOT; }

"(" { return SYM_LPAR; }
")" { return SYM_RPAR; }
"[" { return SYM_LSBKT; }
"]" { return SYM_RSBKT; }
"^" { return SYM_HAT; }

(?i:and) { return WSYM_AND; }
(?i:array) { return WSYM_ARRAY; }
(?i:begin) { return WSYM_BEGIN; }
(?i:case) { return WSYM_CASE; }
(?i:const) { return WSYM_CONST; }
(?i:div) { return WSYM_DIV; }
(?i:do) { return WSYM_DO; }
(?i:downto) { return WSYM_DOWNTO; }
(?i:else) { return WSYM_ELSE; }
(?i:end) { return WSYM_END; }
(?i:for) { return WSYM_FOR; }
(?i:function) { return WSYM_FUNCTION; }
(?i:goto) { /* return WSYM_GOTO; */ }
(?i:if) { return WSYM_IF; }
(?i:in) { return WSYM_IN; }
(?i:label) { /* return WSYM_LABEL; */ }
(?i:mod) { return WSYM_MOD; }
(?i:nil) { return WSYM_NIL; }
(?i:not) { return WSYM_NOT; }
(?i:of) { return WSYM_OF; }
(?i:or) { return WSYM_OR; }
(?i:procedure) { return WSYM_PROCEDURE; }
(?i:program) { return WSYM_PROGRAM; }
(?i:record) { return WSYM_RECORD; }
(?i:repeat) { return WSYM_REPEAT; }
(?i:shl) { return WSYM_SHL; }
(?i:shr) { return WSYM_SHR; }
(?i:set) { return WSYM_SET; }
(?i:then) { return WSYM_THEN; }
(?i:to) { return WSYM_TO; }
(?i:type) { return WSYM_TYPE; }
(?i:until) { return WSYM_UNTIL; }
(?i:var) { return WSYM_VAR; }
(?i:while) { return WSYM_WHILE; }
(?i:with) { return WSYM_WITH; }
(?i:xor) { return WSYM_XOR; }

(?i:{Identifier}) {
	yylval.sval = (char*)malloc(strlen(yytext) + 1);
	strcpy(yylval.sval, yytext);
    to_lowercase(yylval.sval);
    return VAL_ID;
}

{UnsignedInteger} {
    yylval.ival = atoi(yytext);
    return VAL_INT;
}

(?i:{UnsignedReal}) {
    yylval.dval = atof(yytext);
    return VAL_REAL;
}

{Character} {
    yylval.cval = yytext[1];
    return VAL_CHAR;
}

{CharacterString} {
    yylval.sval = yytext + 1;
    yylval.sval[strlen(yylval.sval) - 1] = '\0';
    return VAL_STRING;
}

[\t\r\n\f]|" "|{Comment} {}

. {
    fprintf(stderr, "Illegal charcter '%c' (0x%x) at line %d\n",
                    yytext[0], yytext[0], yylineno);
}

%%

