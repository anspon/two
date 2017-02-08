#define YY_NO_UNISTD_H 1

#include "Node.h"
#include "Parser.hpp"
#include "Tokens.h"

#include <string>
#include <iostream>
#include <fstream>
#include <strstream>

#define A_ASSERT( condition, text )\
    if(!(condition) ){ std::strstream stream; stream << text; throw std::exception(stream.str()) ;}


void  SaveToken(YYSTYPE* yylval, const char* text, int len)
{
    yylval->string = new std::string(text, len);
}

void ReportError(const char* error)
{
    std::cout << error << std::endl;
}
CNode* getAST(const char* expr)
{
    CNode* rootNode = 0;
    yyscan_t scanner = 0;
    YY_BUFFER_STATE state = 0;

    if (yylex_init(&scanner))
    {
        // couldn't initialize
        return NULL;
    }

    state = yy_scan_string(expr, scanner);

    if( yyparse(&rootNode, scanner) )
    {
        // error parsing
        return NULL;
    }

    yy_delete_buffer(state, scanner);

    yylex_destroy(scanner);

    return rootNode;
}
// --------------------------------------------------------------------------------------------------
int main(int argv, char** argc)
{
    try
    {
        A_ASSERT( 2 == argv, "invalid number of arguments provided" );
        std::string completeText;
        {            
            std::ifstream inFile( argc[1] );
            A_ASSERT( !inFile.bad(), "Unable to open file '" << argc[1] << "'" );
            char line[1000] = {0};
            while( inFile.getline(line, 999) )
            {
                completeText += line + std::string("\n");
            }
        }
        if( const CNode* node = getAST(completeText.c_str() ) )
        {
            delete node;
        }

    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }
    return 0;
}
