
%{
#include "stdafx.h"
#include <string>
#include "Node.h"
#include "parser.hpp"
extern void SaveToken( YYSTYPE* yylval, const char* text , int len );
#define SAVE_TOKEN SaveToken( yylval, yytext, yyleng)
#define TOKEN(t) (yylval->token = t)
//extern "C" int yywrap() { }
%}


/*%option outfile="Lexer.c" header-file="Lexer.h"
*/
%option warn nodefault
 
%option reentrant noyywrap never-interactive nounistd
%option bison-bridge
%%

[ \t\n]                 ;
"struct"                return TOKEN(TSTRUCT);
[a-zA-Z_][a-zA-Z0-9\_]*  SAVE_TOKEN; return TIDENTIFIER;
[0-9][0-9\_]*(\.)?[0-9\_]*d      SAVE_TOKEN; return TDOUBLE;
[0-9][0-9\_]*i32             SAVE_TOKEN; return T_I32;
[0-9][0-9\_]*i64               SAVE_TOKEN; return T_I64;
[0-9][0-9\_]*               SAVE_TOKEN; return T_I32;
";"                     return TOKEN(TSEMICOLON);
"="                     return TOKEN(TEQUAL);
"=="                    return TOKEN(TCEQ);
"!="                    return TOKEN(TCNE);
"<"                     return TOKEN(TCLT);
"<="                    return TOKEN(TCLE);
">"                     return TOKEN(TCGT);
">="                    return TOKEN(TCGE);
"("                     return TOKEN(TLPAREN);
")"                     return TOKEN(TRPAREN);
"{"                     return TOKEN(TLBRACE);
"}"                     return TOKEN(TRBRACE);
"."                     return TOKEN(TDOT);
","                     return TOKEN(TCOMMA);
"+"                     return TOKEN(TPLUS);
"-"                     return TOKEN(TMINUS);
"*"                     return TOKEN(TMUL);
"/"                     return TOKEN(TDIV);
.                       printf("Unknown token!n"); yyterminate();

%%