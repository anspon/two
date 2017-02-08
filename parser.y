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
%parse-param { CNode **expression }
%parse-param { yyscan_t scanner }

/* Represents the many different ways we can access our data */
%union {
    CNode* m_node;
    CBlock* block;
    CBlock* blockParts;
    CExpression *expr;
    CBlockPart* blockPart;
    CIdentifier *ident;
    CVariableDeclaration *var_decl;
    CVariableDeclaration *func_param;
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
%token <token> TPLUS TMINUS TMUL TDIV TSEMICOLON

/* Define the type of node our nonterminal symbols represent.
   The types refer to the %union declaration above. Ex: when
   we call an ident (defined by union type ident) we are really
   calling an (NIdentifier*). It makes the compiler happy.
 */
%type <ident> ident
%type <expr> numeric expr 
%type <varvec> func_decl_args
%type <exprvec> call_args
%type <blockParts> blockParts 
%type <blockPart> var_decl func_decl struct_decl blockPart fileScope block 
%type <func_param> func_param
%type <token> comparison

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start fileScope

%%

fileScope : 
    blockParts { *expression = $1; }
    ;
      
blockParts : 
    blockPart { $$ = new CBlock(); $$->blockParts.push_back($<blockPart>1); } | 
    blockParts blockPart { $1->blockParts.push_back($<blockPart>2); }
    ;

blockPart : 
    var_decl | 
    func_decl | 
    struct_decl | 
    expr { $$ = new CExpressionStatement(*$1); } |
    block
    ;

block : 
    TLBRACE blockParts TRBRACE { $$ = $2; } | 
    TLBRACE TRBRACE { $$ = new CBlock(); }
    ;

lparen:
    TLPAREN
    ;

func_decl : 
    ident ident lparen func_decl_args TRPAREN block { $$ = new CFunctionDeclaration(*$1, *$2, *$4, *$6); delete $4; }
    ;
    
func_decl_args : 
    /*blank*/  { $$ = new VariableList(); } | 
    func_param { $$ = new VariableList(); $$->push_back($<func_param>1); } | 
    func_decl_args TCOMMA func_param { $1->push_back($<func_param>3); }
    ;

func_param:
    ident ident { $$ = new CVariableDeclaration(*$1, *$2); } 
    ;
    

var_decl : 
    ident ident TSEMICOLON { $$ = new CVariableDeclaration(*$1, *$2); } | 
    ident ident TEQUAL expr TSEMICOLON { $$ = new CVariableDeclaration(*$1, *$2, $4); }
    ;

struct_decl :  
    TSTRUCT ident TLBRACE TRBRACE TSEMICOLON { $$ = new CStructDeclaration(*$2); }
    ;
        

ident : 
    TIDENTIFIER { $$ = new CIdentifier(*$1); delete $1; }
    ;

numeric : 
    TINTEGER { $$ = new CInteger(*$1); delete $1; } | 
    TDOUBLE { $$ = new CDouble(*$1); delete $1; }
    ;
    
expr : 
    ident TEQUAL expr { $$ = new CAssignment(*$<ident>1, *$3); } | 
    ident TLPAREN call_args TRPAREN { $$ = new CMethodCall(*$1, *$3); delete $3; } | 
    ident { $<ident>$ = $1; } | 
    numeric | 
    expr comparison expr{ $$ = new CBinaryOperator(*$1, $2, *$3); } | 
    TLPAREN expr TRPAREN { $$ = $2; }
    ;
    
call_args : 
    /*blank*/  { $$ = new ExpressionList(); } | 
    expr { $$ = new ExpressionList(); $$->push_back($1); } | 
    call_args TCOMMA expr  { $1->push_back($3); }
    ;

comparison : 
    TCEQ | 
    TCNE | 
    TCLT | 
    TCLE | 
    TCGT | 
    TCGE | 
    TPLUS | 
    TMINUS | 
    TMUL | 
    TDIV
    ;

%%