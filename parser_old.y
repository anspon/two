%{
#define YY_NO_UNISTD_H 1
#define YYERROR_VERBOSE 1

#include "Node.h"
#include "Parser.hpp"
#include "Tokens.h"
//NBlock *programBlock; /* the top level root node of our final AST */

extern void ReportError(const char* msg );

int yyerror(CNode **expression, yyscan_t scanner, const char *msg) 
{
    ReportError( msg );
    return 0;
}

%}

%code requires {

#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif

}

/*
%output  "Parser.c"
%defines "Parser.h"
 */

%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { CFileScopeNode **expression }
%parse-param { yyscan_t scanner }

/* Represents the many different ways we can access our data */
%union {
    CNode* m_node;
    CBlock *block;
    CExpression *expr;
    CStatement *stmt;
    CIdentifier *ident;
    CVariableDeclaration *var_decl;
    std::vector<CVariableDeclaration*> *varvec;
    std::vector<CExpression*> *exprvec;
    std::string *string;
    int token;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER TINTEGER TDOUBLE TSTRUCT
%token <token> TCEQ TCNE TCLT TCLE TCGT TCGE TEQUAL
%token <token> TLPAREN TRPAREN TLBRACE TRBRACE TCOMMA TDOT 
%token <token> TPLUS TMINUS TMUL TDIV

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <block> program stmts block
%type <stmt> stmt var_decl func_decl struct_decl
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start program

%%

program : stmts { *expression = $1; }
        ;
        
stmts : stmt { $$ = new CFileScopeNode(); $$->statements.push_back($<stmt>1); }
      | stmts stmt { $1->statements.push_back($<stmt>2); }
      ;

stmt : var_decl | func_decl | struct_decl
     | expr { $$ = new CExpressionStatement(*$1); }
     ;

block : TLBRACE stmts TRBRACE { $$ = $2; }
      | TLBRACE TRBRACE { $$ = new CBlock(); }
      ;

var_decl : ident ident { $$ = new CVariableDeclaration(*$1, *$2); }
         | ident ident TEQUAL expr { $$ = new CVariableDeclaration(*$1, *$2, $4); }
         ;

struct_decl :  TSTRUCT ident TLBRACE TRBRACE
        { $$ = new CStructDeclaration(*$2); }
        ;
        
func_decl : ident ident TLPAREN func_decl_args TRPAREN block 
            { $$ = new CFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
          ;
    
func_decl_args : /*blank*/  { $$ = new VariableList(); }
          | var_decl { $$ = new VariableList(); $$->push_back($<var_decl>1); }
          | func_decl_args TCOMMA var_decl { $1->push_back($<var_decl>3); }
          ;

ident : TIDENTIFIER { $$ = new CIdentifier(*$1); delete $1; }
      ;

numeric : TINTEGER { $$ = new CInteger(atol($1->c_str())); delete $1; }
        | TDOUBLE { $$ = new CDouble(atof($1->c_str())); delete $1; }
        ;
    
expr : ident TEQUAL expr { $$ = new CAssignment(*$<ident>1, *$3); }
     | ident TLPAREN call_args TRPAREN { $$ = new CMethodCall(*$1, *$3); delete $3; }
     | ident { $<ident>$ = $1; }
     | numeric
     | expr comparison expr { $$ = new CBinaryOperator(*$1, $2, *$3); }
     | TLPAREN expr TRPAREN { $$ = $2; }
     ;
    
call_args : /*blank*/  { $$ = new ExpressionList(); }
          | expr { $$ = new ExpressionList(); $$->push_back($1); }
          | call_args TCOMMA expr  { $1->push_back($3); }
          ;

comparison : TCEQ | TCNE | TCLT | TCLE | TCGT | TCGE 
           | TPLUS | TMINUS | TMUL | TDIV
           ;

%%