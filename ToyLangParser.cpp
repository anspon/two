#include "stdafx.h"

#define YY_NO_UNISTD_H 1

#include "Node.h"
#include "Parser.hpp"
#include "Tokens.h"
#include "CodeStreamer.h"


#define filesystem std::experimental::filesystem::v1

namespace Tk
{
class CException
{
public:
    CException(const std::string& text ) : m_text(text)
    {
    }

    const std::string&
        Message(
            )const
    {
        return m_text;
    }
        

private:
    std::string 
        m_text;
};
}

void ThrowStream(const std::stringstream& stream )
{
    throw Tk::CException(stream.str());
}

#define A_ASSERT( condition, text )\
    if(!(condition) ){ std::stringstream stream; stream << text; ThrowStream(stream) ;}


void  SaveToken(YYSTYPE* yylval, const char* text, int len)
{
    yylval->string = new std::string(text, len);
}

void ReportError(const char* error)
{
    std::cout << error << std::endl;
}
sp<const Ast::CNode> getAST(const char* expr)
{
    Ast::CNode* rootNode = 0;
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

    return sp<const Ast::CNode>(rootNode);
}
// --------------------------------------------------------------------------------------------------
int main(int argv, char** argc)
{
    try
    {
        A_ASSERT( 3 == argv, "invalid number of arguments provided" );

        filesystem::directory_iterator sourceIterator(argc[1]);
        A_ASSERT( filesystem::directory_iterator() != sourceIterator, "Invalid source directory '" << argc[1] << "' ");

        filesystem::path buildPath(argc[2]);

        filesystem::path outputPath = buildPath /= "HelloWorld";

        if(!filesystem::exists(outputPath ) )
        {
            bool created = filesystem::create_directory(outputPath);
            A_ASSERT( created, "Failed to create output directory '" << outputPath << "'" );
        }

        for( const filesystem::directory_entry& entry : sourceIterator )
        {
            const filesystem::path& path = entry.path();
        
            if( std::string(".aspp") == path.extension() )
            {
                filesystem::path filePath = outputPath /= path.stem();
                filePath.replace_extension("cpp");
                CStreamT<std::ofstream> targetFile( filePath );

                int i=0;i++;
                std::string completeText;
                {            
                    std::ifstream inFile( path );
                    A_ASSERT( !inFile.bad(), "Unable to open file '" << path << "'" );
                    char line[1000] = {0};
                    while( inFile.getline(line, 999) )
                    {
                        completeText += line + std::string("\n");
                    }
                }
                if( sp<const Ast::CNode> node = getAST(completeText.c_str() ) )
                {
                    
                    CCodeStreamer streamer(targetFile);
                    node->MakeCpp(streamer);
                    int i=0; i++;
                }
            }        
        }

    }
    catch( Tk::CException& e )
    {
        std::cout << e.Message() << std::endl;
        std::cin.get();
    }
    return 0;
}
