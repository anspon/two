/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 20 "parser.y" /* yacc.c:1909  */


#ifndef YY_TYPEDEF_YY_SCANNER_T
#define YY_TYPEDEF_YY_SCANNER_T
typedef void* yyscan_t;
#endif


#line 53 "parser.hpp" /* yacc.c:1909  */

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    TIDENTIFIER = 258,
    T_I32 = 259,
    T_I64 = 260,
    TDOUBLE = 261,
    TSTRUCT = 262,
    TCEQ = 263,
    TCNE = 264,
    TCLT = 265,
    TCLE = 266,
    TCGT = 267,
    TCGE = 268,
    TEQUAL = 269,
    TLPAREN = 270,
    TRPAREN = 271,
    TLBRACE = 272,
    TRBRACE = 273,
    TCOMMA = 274,
    TDOT = 275,
    TPLUS = 276,
    TMINUS = 277,
    TMUL = 278,
    TDIV = 279,
    TSEMICOLON = 280
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 40 "parser.y" /* yacc.c:1909  */

    Ast::CNode* m_node;
    Ast::CBlock* block;
    Ast::CBlock* blockParts;
    Ast::CExpression* expr;
    Ast::CBlockPart* blockPart;
    Ast::CIdentifier* ident;
    Ast::CVariableDeclaration* var_decl;
    Ast::CVariableDeclaration* func_param;
    Ast::VariableListNaked* varvec;
    Ast::ExpressionListNaked* exprvec;
    std::string *string;
    int token;

#line 106 "parser.hpp" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif



int yyparse (Ast::CNode **expression, yyscan_t scanner);

#endif /* !YY_YY_PARSER_HPP_INCLUDED  */
