%{
#define YY_NO_UNISTD_H 1
#define YYERROR_VERBOSE 1

#include "Node.h"
#include "Parser.hpp"
#include "Tokens.h"
//NBlock *programBlock; /* the top level root node of our final AST */

extern void ReportError(const char* msg );

int yyerror(Ast::CNode **expression, yyscan_t scanner, const char *msg) 
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
 
%define api.pure
%lex-param   { yyscan_t scanner }
%parse-param { Ast::CNode **expression }
%parse-param { yyscan_t scanner }

/* Represents the many different ways we can access our data */
%union {
    Ast::CNode* m_node;
    Ast::CBlock* block;
    Ast::CBlock* blockParts;
    Ast::CExpression* expr;
    Ast::CBlockPart* blockPart;
    Ast::CIdentifier* ident;
    Ast::CVariableDeclaration* var_decl;
    Ast::CFunctionParameter* func_param;
    Ast::VariableList* varvec;
    Ast::FunctionParameterList* funcParList;
    Ast::ExpressionList* exprvec;
    std::string *string;
    int token;

    Ast::CStructPart*     structPart;
    Ast::StructPartList* structParts;
}

/* Define our terminal symbols (tokens). This should
   match our tokens.l lex file. We also define the node type
   they represent.
 */
%token <string> TIDENTIFIER T_I32 T_I64 TDOUBLE TSTRUCT
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
%type <funcParList> func_decl_args
%type <exprvec> call_args
%type <blockParts> blockParts 
%type <blockPart> var_decl func_decl struct blockPart fileScope block  
%type <func_param> func_param
%type <token> comparison
%type <structPart> structPart
%type <structParts> structParts

/* Operator precedence for mathematical operators */
%left TPLUS TMINUS
%left TMUL TDIV

%start fileScope

%%

fileScope : 
    blockParts { *expression = $1; }
    ;
      
blockParts : 
    blockPart { $$ = new Ast::CBlock(); $$->AddBlockPart($<blockPart>1); } | 
    blockParts blockPart { $1->AddBlockPart($<blockPart>2); }
    ;

blockPart : 
    var_decl | 
    func_decl | 
    struct | 
    expr { $$ = new Ast::CExpressionStatement($1); } |
    block
    ;

block : 
    TLBRACE blockParts TRBRACE { $$ = $2; } | 
    TLBRACE TRBRACE { $$ = new Ast::CBlock(); }
    ;

lparen:
    TLPAREN
    ;

struct :  
    TSTRUCT ident TLBRACE structParts TRBRACE TSEMICOLON { $$ = new Ast::CStructDeclaration($2, $4); } |
    TSTRUCT ident TLBRACE TRBRACE TSEMICOLON { $$ = new Ast::CStructDeclaration($2, nullptr); }
    ;

structPart : 
    ident ident TSEMICOLON { $$ = new Ast::CStructVariableDeclaration($1, $2, nullptr); } |
    ident ident TEQUAL expr TSEMICOLON { $$ = new Ast::CStructVariableDeclaration($1, $2, $4); }
    ;

structParts: 
    structPart { $$ = new Ast::StructPartList; $$->push_back(attach_sp($<structPart>1)); } | 
    structParts structPart { $1->push_back(attach_sp($<structPart>2)); }
    ;

func_decl : 
    ident ident lparen func_decl_args TRPAREN block { $$ = new Ast::CFunctionDeclaration($1, $2, $4, $6); }
    ;
    
func_decl_args : 
    /*blank*/  { $$ = new Ast::FunctionParameterList; } | 
    func_param { $$ = new Ast::FunctionParameterList; $$->push_back(attach_sp($<func_param>1)); } | 
    func_decl_args TCOMMA func_param { $1->push_back(attach_sp($<func_param>3)); }
    ;

func_param:
    ident ident { $$ = new Ast::CFunctionParameter($1, $2, nullptr); } 
    ;
    

var_decl : 
    ident ident TSEMICOLON { $$ = new Ast::CVariableDeclaration($1, $2, nullptr); } | 
    ident ident TEQUAL expr TSEMICOLON { $$ = new Ast::CVariableDeclaration($1, $2, $4); }
    ;

        

ident : 
    TIDENTIFIER { $$ = new Ast::CIdentifier($1); }
    ;

numeric : 
    T_I32 { $$ = new Ast::CInteger32($1);} | 
    TDOUBLE { $$ = new Ast::CDouble($1);}
    ;
    
expr : 
    ident TEQUAL expr { $$ = new Ast::CAssignment($<ident>1, $3); } | 
    ident TLPAREN call_args TRPAREN { $$ = new Ast::CMethodCall($1, $3); } | 
    ident { $<ident>$ = $1; } | 
    numeric | 
    expr comparison expr{ $$ = new Ast::CBinaryOperator($1, $2, $3); } | 
    TLPAREN expr TRPAREN { $$ = $2; }
    ;
    
call_args : 
    /*blank*/  { $$ = new Ast::ExpressionList; } | 
    expr { $$ = new Ast::ExpressionList; $$->push_back(attach_sp($1)); } | 
    call_args TCOMMA expr  { $1->push_back(attach_sp($3) ); }
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