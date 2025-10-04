#ifndef KDLC_PARSER_AST_AST_DUMPER_H_
#define KDLC_PARSER_AST_AST_DUMPER_H_

#include "ASTVisitor.h"
#include "ASTNode.h"
namespace kdl
{

class ASTDumper : ASTVisitor
{
public:

    void dump(const NodePtr<ASTProgram>& root);



    void visit(const ASTNode& node)                 override;
    void visit(const ASTProgram& node)              override;
    void visit(const ASTIdentifier& node)           override;
    void visit(const ASTLiteral& node)              override;
    void visit(const ASTBinaryOperation& node)      override;
    void visit(const ASTUnaryOperation& node)       override;
    void visit(const ASTStmt& node)                 override;
    void visit(const ASTDecl& node)                 override;
    void visit(const ASTBlock& node)                override;
    void visit(const ASTRule& node)                 override;


private:

    std::string indentation();

    size_t m_indentLevel{ 0 };
    
};



} // kdl


#endif // KDLC_PARSER_AST_AST_DUMPER_H_