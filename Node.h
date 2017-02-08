#ifndef Imdr_Ast_H
#define Imdr_Ast_H

#include <memory>
#include <list>

#include <iostream>
#include <vector>

class CBlockPart;
class CExpression;
class CVariableDeclaration;

typedef std::vector<CBlockPart*> BlockPartList;
typedef std::vector<CExpression*> ExpressionList;
typedef std::vector<CVariableDeclaration*> VariableList;

class CNode 
{
public:
    virtual ~CNode() {}
};


class CBlockPart : public CNode 
{
};

class CExpression : public CBlockPart
{
};

class CStatement : public CBlockPart
{
};

class CInteger : public CExpression 
{
public:
    long long m_value;
    CInteger(const std::string& value) : 
        m_value(atoi(value.c_str())) 
        {
        }
};

class CDouble : public CExpression 
{
public:
    double m_value;
    CDouble(const std::string& value) : 
        m_value(atof(value.c_str()) ) 
        { 
        }
};

class CIdentifier : public CExpression 
{
public:
    std::string m_name;
    CIdentifier(const std::string& name) : m_name(name) { }
};

class CStructDeclaration : public CStatement
{
public:
    const CIdentifier 
        m_name;
    
    CStructDeclaration(const CIdentifier& name) : m_name(name) { }
};

class CMethodCall : public CExpression 
{
public:
    const CIdentifier& id;
    ExpressionList arguments;
    CMethodCall(
        const CIdentifier& id, 
        ExpressionList& arguments) :
        id(id), arguments(arguments) { }
    
    CMethodCall(const CIdentifier& id) : id(id) { }
};

class CBinaryOperator : public CExpression 
{
public:
    int op;
    CExpression& lhs;
    CExpression& rhs;
    CBinaryOperator(CExpression& lhs, int op, CExpression& rhs) :
        lhs(lhs), rhs(rhs), op(op) { }
};

class CAssignment : public CExpression 
{
public:
    CIdentifier& lhs;
    CExpression& rhs;
    CAssignment(CIdentifier& lhs, CExpression& rhs) : 
        lhs(lhs), rhs(rhs) { }
};

class CBlock : public CExpression {
public:
    BlockPartList blockParts;
    CBlock() { }
};

class CScope : public CBlock
{
};


class CFileScope : public CScope
{

};


class CExpressionStatement : public CStatement 
{
public:
    CExpression& expression;
    CExpressionStatement(CExpression& expression) : 
        expression(expression) { }
};

class CVariableDeclaration : public CStatement 
{
public:
    const CIdentifier& m_type;
    CIdentifier& m_id;
    CExpression* m_assignmentExpr;
    
    CVariableDeclaration(const CIdentifier& type, CIdentifier& id) :
        m_type(type), m_id(id) 
    { 
    }
    
    CVariableDeclaration(
        const CIdentifier& type, 
        CIdentifier& id, 
        CExpression *assignmentExpr
        ) :
        m_type(type), m_id(id), m_assignmentExpr(assignmentExpr) { }
};

class CFunctionDeclaration : public CStatement {
public:
    const CIdentifier& m_type;
    const CIdentifier& m_id;
    VariableList m_arguments;
    CBlockPart& m_block;

    CFunctionDeclaration(
        const CIdentifier& type, 
        const CIdentifier& id, 
        const VariableList& arguments, 
        CBlockPart& block
        ) :
        m_type(type), m_id(id), m_arguments(arguments), m_block(block) { }
};

#endif // __EXPRESSION_H__
