#include "Node.h"

namespace Ast
{

CNode::~CNode()
{
    int i=0; i++;
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
CExpressionStatement::CExpressionStatement(
    const CExpression* expression
    )
{
    m_expression = attach_sp(expression);
}
// --------------------------------------------------------------------------------------------------
CStructDeclaration::CStructDeclaration(
    const CIdentifier* name
    )
{
    m_name = attach_sp(name);
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
    const ExpressionListNaked* arguments
    )
{
    m_id = attach_sp(id);
    if( arguments )
    {
        for( const CExpression* expr : *arguments )
        {
            m_arguments.push_back( attach_sp(expr) );
        }
        delete arguments;
    }
}
// --------------------------------------------------------------------------------------------------
CFunctionDeclaration::CFunctionDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        VariableListNaked* arguments, 
        const CBlockPart* block
        )
{
    m_type = attach_sp(type);
    m_id = attach_sp(id);
    if( arguments )
    {
        for( const CVariableDeclaration* var : *arguments )
        {
            m_arguments.push_back( attach_sp(var) );
        }
        arguments->clear();
        delete arguments;
    }
    m_block = attach_sp(block);
}

}

