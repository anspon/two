#include "stdafx.h"
#include "Node.h"
#include "CodeStreamer.h"


namespace Ast
{

CNode::~CNode()
{
    int i=0; i++;
}
//--------------------------------------------------------------------------------------------------
void CNode::MakeCpp(
    CCodeStreamer& streamer
    )const
{
}
//--------------------------------------------------------------------------------------------------
CInteger32::CInteger32( 
    const std::string* value
    )
{
    m_value = std::atoi((*value).c_str()); 
    delete value;
}
//--------------------------------------------------------------------------------------------------
CDouble::CDouble(const std::string * value)
{
    m_value = atof((*value).c_str());
    m_originalText = *value;
    delete value;
}
//--------------------------------------------------------------------------------------------------
CInteger64::CInteger64(
    const std::string* value
    )
{
    m_value = std::atoll((*value).c_str());
    m_originalText = *value;
    delete value;
}
//--------------------------------------------------------------------------------------------------
void CInteger64::MakeCpp( 
    CCodeStreamer& streamer 
    )const
{
    streamer.CoreStream() << m_value;
}
//--------------------------------------------------------------------------------------------------
void CInteger32::MakeCpp(
    CCodeStreamer& streamer
    ) const
{
    streamer.CoreStream() << m_value;
}
//--------------------------------------------------------------------------------------------------
void CDouble::MakeCpp(
    CCodeStreamer & streamer
    ) const
{
    streamer.CoreStream() << m_value;
}
//--------------------------------------------------------------------------------------------------
CFunctionParameter::CFunctionParameter(
    const CIdentifier* type, 
    const CIdentifier* id, 
    const CExpression* assignmentExpr
    )
{
    m_type = attach_sp(type);
    m_id = attach_sp(id);
    m_assignmentExpr = attach_sp(assignmentExpr);
}
//--------------------------------------------------------------------------------------------------
void CFunctionParameter::MakeCpp(
    CCodeStreamer& streamer
    ) const
{
    streamer << m_type << " " << m_id;
}
//--------------------------------------------------------------------------------------------------
CVariableDeclaration::CVariableDeclaration(
    const CIdentifier* type, 
    const CIdentifier* id, 
    const CExpression* assignmentExpr
    )
{
    m_type = attach_sp(type);
    m_id = attach_sp(id);
    m_assignmentExpr = attach_sp(assignmentExpr);
}
//--------------------------------------------------------------------------------------------------
CStructVariableDeclaration::CStructVariableDeclaration(
    const CIdentifier* type, 
    const CIdentifier* id, 
    const CExpression* assignmentExpr
    )
{
    m_type = attach_sp(type);
    m_id = attach_sp(id);
    m_assignmentExpr = attach_sp(assignmentExpr);
}
//--------------------------------------------------------------------------------------------------
void CStructVariableDeclaration::MakeCpp(
    CCodeStreamer& streamer
        ) const
{
    streamer << m_type << CStream::endl;
    {
        CIndenter indent(streamer.Stream(), 1);
        streamer << m_id;
        
        if( m_assignmentExpr )
        {
            streamer << " = " << m_assignmentExpr;
        }
        streamer << ";";
    }
    streamer.Stream() << CStream::endl;
}
//--------------------------------------------------------------------------------------------------
void CIdentifier::MakeCpp(
    CCodeStreamer& streamer
    )const
{
    streamer << m_name;
}
//--------------------------------------------------------------------------------------------------
void CVariableDeclaration::MakeCpp(
    CCodeStreamer& streamer
    ) const
{
    streamer << m_type << CStream::endl;
    {
        CIndenter indent(streamer.Stream(), 1);
        streamer << m_id;

        if( m_assignmentExpr )
        {
            streamer << " = " << m_assignmentExpr;
        }
        streamer << ";";
    }
    streamer << CStream::endl;
}
//--------------------------------------------------------------------------------------------------
CExpressionStatement::CExpressionStatement(
    const CExpression* expression
    )
{
    m_expression = attach_sp(expression);
}
//--------------------------------------------------------------------------------------------------
CStructDeclaration::CStructDeclaration(
    const CIdentifier* name,
    const StructPartList* list 
    )
{
    m_name = attach_sp(name);
    if( list )
    {
        m_parts = *list;
        delete list;
    }

}
//--------------------------------------------------------------------------------------------------
CStructPart::CStructPart()
{
}
//--------------------------------------------------------------------------------------------------
void CStructDeclaration::MakeCpp(
    CCodeStreamer& streamer
    )const
{
    streamer.BeginStruct(m_name->m_name);
    {
        CIndenter indent(streamer.Stream(), 1);

        for( sp<const CStructPart> structPart : m_parts )
        {
            streamer << structPart;
        }

    }

    streamer.EndStruct();
}
//--------------------------------------------------------------------------------------------------
CAssignment::CAssignment(
    const CIdentifier* lhs, 
    const CExpression* rhs
    )
{
    m_lhs = attach_sp(lhs);
    m_rhs = attach_sp(rhs);
}
//--------------------------------------------------------------------------------------------------
CBinaryOperator::CBinaryOperator(
    const CExpression* lhs, 
    int op, 
    const CExpression* rhs
    )
{
    m_lhs = attach_sp(lhs);
    m_op = op;
    m_rhs = attach_sp(rhs);
}
//--------------------------------------------------------------------------------------------------
void CBlock::MakeCpp(
    CCodeStreamer& streamer
    )const
{
    for( sp<const CBlockPart> block : m_blockParts )
    {
        streamer << block;
    }
}
//--------------------------------------------------------------------------------------------------
void CExpressionStatement::MakeCpp(
    CCodeStreamer& streamer
    ) const
{
    m_expression->MakeCpp(streamer);
}
//--------------------------------------------------------------------------------------------------
void CBlock::AddBlockPart(
    const CBlockPart* part
    )
{
    m_blockParts.push_back( sp<const CBlockPart>(part) );
}
//--------------------------------------------------------------------------------------------------
CMethodCall::CMethodCall(
    const CIdentifier* id, 
    const ExpressionList* arguments
    )
{
    m_id = attach_sp(id);
    if( arguments )
    {
        m_arguments = *arguments;
        delete arguments;
    }
}
//--------------------------------------------------------------------------------------------------
CFunctionDeclaration::CFunctionDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        FunctionParameterList* arguments, 
        const CBlockPart* block
        )
{
    m_type = attach_sp(type);
    m_id = attach_sp(id);
    if( arguments )
    {
        m_arguments = *arguments;
        delete arguments;
    }
    m_block = attach_sp(block);
}
//--------------------------------------------------------------------------------------------------
void CFunctionDeclaration::MakeCpp(
    CCodeStreamer& streamer
    )const
{
    streamer << m_type << " " << m_id << "(" << StreamCollection(streamer, m_arguments, "," )  << ")" << CStream::endl;
    streamer << "{" << CStream::endl;


    streamer << "}" << CStream::endl;
}
//--------------------------------------------------------------------------------------------------
CCodeStreamer& operator<<(CCodeStreamer& streamer, sp<const CNode> node )
{
    node->MakeCpp(streamer);
    return streamer;
}

}

