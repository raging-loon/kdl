#ifndef KDLC_AST_AST_BUILDER_H_
#define KDLC_AST_AST_BUILDER_H_

#include <antlr4-runtime.h>
#include "generated/KDLGrammarBaseVisitor.h"

#include <any>

#include "Nodes.h"

namespace kdl
{

class ASTBuilder : public kdl_gen::KDLGrammarBaseVisitor
{
public:
    ASTBuilder();

    std::any visitStartRule(kdl_gen::KDLGrammarParser::StartRuleContext* ctx) override;
private:

    std::any visitTop_decls(kdl_gen::KDLGrammarParser::Top_declsContext* ctx) override;
    std::any visitRule_decl(kdl_gen::KDLGrammarParser::Rule_declContext* ctx) override;

    std::any visitExpr(kdl_gen::KDLGrammarParser::ExprContext* ctx) override;

    std::any visitField_access(kdl_gen::KDLGrammarParser::Field_accessContext* ctx) override;
    std::any visitPrimary(kdl_gen::KDLGrammarParser::PrimaryContext* ctx) override;

    std::any visitArray_literal(kdl_gen::KDLGrammarParser::Array_literalContext* ctx) override;
    
    std::any visitLiteral(kdl_gen::KDLGrammarParser::LiteralContext* ctx) override;


    Operation getOperation(kdl_gen::KDLGrammarParser::BinaryOpContext* ctx);
    ASTLiteral::Type getLiteralType(kdl_gen::KDLGrammarParser::LiteralContext* ctx);
    NodePtr<ASTProgram> m_root;
};

} // kdl

#endif // KDLC_AST_AST_BUILDER_H_