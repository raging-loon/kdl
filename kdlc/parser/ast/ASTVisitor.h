#ifndef KDLC_PARSER_AST_AST_VISITOR_H_
#define KDLC_PARSER_AST_AST_VISITOR_H_


namespace kdl
{
struct ASTNode;
struct ASTProgram;
struct ASTIdentifier;
struct ASTLiteral;
struct ASTBinaryOperation;
struct ASTUnaryOperation;
struct ASTStmt;
struct ASTDecl;
struct ASTBlock;
struct ASTRule;

///
/// @brief
///     A class that visits nodes in the AST Tree
class ASTVisitor
{
public:

    virtual ~ASTVisitor() = default;

    virtual void visit(const ASTNode& node) = 0;
    virtual void visit(const ASTProgram& node) = 0;
    virtual void visit(const ASTIdentifier& node) = 0;
    virtual void visit(const ASTLiteral& node) = 0;
    virtual void visit(const ASTBinaryOperation& node) = 0;
    virtual void visit(const ASTUnaryOperation& node) = 0;
    virtual void visit(const ASTStmt& node) = 0;
    virtual void visit(const ASTDecl& node) = 0;
    virtual void visit(const ASTBlock& node) = 0;
    virtual void visit(const ASTRule& node) = 0;

};



} // kdl

#endif // KDLC_PARSER_AST_AST_VISITOR_H_