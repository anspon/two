#ifndef Imdr_Ast_H
#define Imdr_Ast_H

#include <memory>
#include <list>

#include <iostream>
#include <vector>

template<typename T>
using sp = std::shared_ptr<T>;

template<class T>
sp<T> attach_sp( T* naked )
{
    return sp<T>(naked);
}

namespace Ast
{
class CBlockPart;
class CExpression;
class CVariableDeclaration;

typedef std::vector< const CVariableDeclaration* > VariableListNaked;
typedef std::vector< const CExpression* > ExpressionListNaked;

typedef std::vector< sp<const CBlockPart> > BlockPartList;
typedef std::vector< sp<const CExpression> > ExpressionList;
typedef std::vector< sp<const CVariableDeclaration> > VariableList;

class CNode 
{
public:
    virtual ~CNode();
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

class CInteger32 : public CExpression 
{
public:
    int32_t m_value;
    CInteger32(const std::string* value) : 
        m_value(std::atoi((*value).c_str())) 
        {
            delete value;
        }
};
class CInteger64 : public CExpression 
{
public:
    int64_t m_value;
    CInteger64(const std::string* value) : 
        m_value(std::atoll((*value).c_str())) 
        {
            delete value;
        }
};

class CDouble : public CExpression 
{
public:
    double m_value;
    CDouble(const std::string* value) : 
        m_value(atof((*value).c_str()) ) 
        { 
            delete value;
        }
};

class CIdentifier : public CExpression 
{
public:
    std::string m_name;
    CIdentifier(const std::string* name) : m_name(*name) {delete name; }
};

class CStructDeclaration : public CStatement
{
public:
    sp<const CIdentifier>
        m_name;
    
    CStructDeclaration(const CIdentifier* name);
};

class CMethodCall : public CExpression 
{
public:
    CMethodCall(
        const CIdentifier* id, 
        const ExpressionListNaked* arguments
        ) ;
    
   
private:
    sp<const CIdentifier>
        m_id;

    ExpressionList 
        m_arguments;

};

class CBinaryOperator : public CExpression 
{
public:
    CBinaryOperator(
        const CExpression* lhs, 
        int op, 
        const CExpression* rhs
        );

private:
    sp<const CExpression> 
        m_lhs;
    
    int 
        m_op;

    sp<const CExpression> 
        m_rhs;

};

class CAssignment : public CExpression 
{
public:
    CAssignment(
        const CIdentifier* lhs, 
        const CExpression* rhs
        );


private:
    sp<const CIdentifier> 
        m_lhs;

    sp<const CExpression> 
        m_rhs;

};

class CBlock : public CExpression 
{
public:
    void
        AddBlockPart(
            const CBlockPart* part
            );

private:        
    BlockPartList 
        m_blockParts;
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
    sp<const CExpression> 
        m_expression;

    CExpressionStatement(
        const CExpression* expression
        );
};

class CVariableDeclaration : public CStatement 
{
public:

    CVariableDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        const CExpression* assignmentExpr
        );

private:
    sp<const CIdentifier> 
        m_type;
    
    sp<const CIdentifier> 
        m_id;
    
    sp<const CExpression> 
        m_assignmentExpr;
};

class CFunctionDeclaration : public CStatement 
{
public:
    CFunctionDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        VariableListNaked* arguments, 
        const CBlockPart* block
        );


private:
    sp<const CIdentifier> 
        m_type;
    sp<const CIdentifier> 
        m_id;
    
    VariableList 
        m_arguments;
    
    sp<const CBlockPart> 
        m_block;

};

}
#endif // __EXPRESSION_H__
