#ifndef Imdr_Ast_H
#define Imdr_Ast_H

#include <memory>
#include <list>

#include <iostream>
#include <vector>
#include <string>



class CCodeStreamer;

template<typename T>
using sp = std::shared_ptr<T>;

template<class T>
sp<T> attach_sp( T* naked )
{
    return sp<T>(naked);
}
class CCodeStreamer;

namespace Ast
{
class CBlockPart;
class CExpression;
class CVariableDeclaration;
class CStructPart;
class CFunctionParameter;

typedef std::vector< sp<const CStructPart> > StructPartList;
typedef std::vector< sp<const CBlockPart> > BlockPartList;
typedef std::vector< sp<const CExpression> > ExpressionList;
typedef std::vector< sp<const CVariableDeclaration> > VariableList;
typedef std::vector< sp<const CFunctionParameter> > FunctionParameterList;

class CNode 
{
public:
    virtual ~CNode();

    virtual void 
        MakeCpp(
            CCodeStreamer& streamer
            )const;
private:
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

class CStructPart : public CNode
{
public:
    CStructPart();
};

class CLiteral : public CExpression
{
};
class CInteger32 : public CLiteral 
{
public:
    int32_t m_value;
    CInteger32(
        const std::string* value
        );

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const;

};
class CInteger64 : public CLiteral 
{
public:
    CInteger64(
        const std::string* value
        );

    int64_t 
        m_value;

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;
    
    std::string
        m_originalText;
};

class CDouble : public CLiteral 
{
public:
    CDouble(
        const std::string* value
        );
    
    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;
       
private:    
    double 
        m_value;

    std::string 
        m_originalText;    
};

class CIdentifier : public CExpression 
{
public:
    CIdentifier(const std::string* name) : m_name(*name) {delete name; }
    
    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;

    std::string m_name;
};

class CStructDeclaration : public CStatement
{
public:
    CStructDeclaration(
        const CIdentifier* name,
        const StructPartList* list 
        );

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const;

private:
    sp<const CIdentifier>
        m_name;

    StructPartList
        m_parts;
};

class CMethodCall : public CExpression 
{
public:
    CMethodCall(
        const CIdentifier* id, 
        const ExpressionList* arguments
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

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override ;

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

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;

};


class CStructVariableDeclaration : public CStructPart
{
public:

    CStructVariableDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        const CExpression* assignmentExpr
        );
    
    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;
private:
    sp<const CIdentifier> 
        m_type;
    
    sp<const CIdentifier> 
        m_id;
    
    sp<const CExpression> 
        m_assignmentExpr;
};

class CFunctionParameter : public CStatement 
{
public:

    CFunctionParameter(
        const CIdentifier* type, 
        const CIdentifier* id, 
        const CExpression* assignmentExpr
        );

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;

private:
    sp<const CIdentifier> 
        m_type;
    
    sp<const CIdentifier> 
        m_id;
    
    sp<const CExpression> 
        m_assignmentExpr;
};

class CVariableDeclaration : public CStatement 
{
public:

    CVariableDeclaration(
        const CIdentifier* type, 
        const CIdentifier* id, 
        const CExpression* assignmentExpr
        );

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;

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
        FunctionParameterList* arguments, 
        const CBlockPart* block
        );

    void 
        MakeCpp(
            CCodeStreamer& streamer
            )const override;

private:
    sp<const CIdentifier> 
        m_type;

    sp<const CIdentifier> 
        m_id;
    
    FunctionParameterList 
        m_arguments;
    
    sp<const CBlockPart> 
        m_block;

};

CCodeStreamer& operator<<(CCodeStreamer& streamer, sp<const CNode> node );


}
#endif // __EXPRESSION_H__
