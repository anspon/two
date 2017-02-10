#include "Node.h"

namespace Ast
{

CNode::~CNode()
{
    int i=0; i++;
}
// --------------------------------------------------------------------------------------------------
CInteger32::CInteger32( 
    const std::string* value
    )
{
    m_value = std::atoi((*value).c_str()); 
    delete value;
}
// --------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------
CExpressionStatement::CExpressionStatement(
    const CExpression* expression
    )
{
    m_expression = attach_sp(expression);
}
// --------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------
CAssignment::CAssignment(
    const CIdentifier* lhs, 
    const CExpression* rhs
    )
{
    m_lhs = attach_sp(lhs);
    m_rhs = attach_sp(rhs);
}
// --------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------
void CBlock::AddBlockPart(
    const CBlockPart* part
    )
{
    m_blockParts.push_back( sp<const CBlockPart>(part) );
}
// --------------------------------------------------------------------------------------------------
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
// --------------------------------------------------------------------------------------------------
CFunctionDeclaration::CFunctionDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        VariableList* arguments, 
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

}

