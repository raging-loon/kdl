#include "ASTBuilder.h"

#include <any>

using namespace kdl_gen;

namespace kdl
{

template <class T = ASTNode>
constexpr auto NodeCast(const std::any& operand)
{
    return std::any_cast<NodePtr<T>>(operand);
}


ASTBuilder::ASTBuilder()
{
    m_root = MakeNode<ASTProgram>();
}

std::any ASTBuilder::visitStartRule(KDLGrammarParser::StartRuleContext* ctx)
{

    return visitChildren(ctx);
}

std::any ASTBuilder::visitTop_decls(KDLGrammarParser::Top_declsContext* ctx)
{
    // parse rules 

    for (const auto& i : ctx->rule_decl())
    {
       NodePtr<ASTRule> rule = std::any_cast<NodePtr<ASTRule>>(visitRule_decl(i));
       m_root->topDecls.push_back(rule);
    }
    return std::any{};
}

std::any ASTBuilder::visitRule_decl(kdl_gen::KDLGrammarParser::Rule_declContext* ctx)
{
    auto newRule = MakeNode<ASTRule>();

    newRule->name = ctx->IDENTIFIER()->toString();
    // get event source 
    if (ctx->event_source())
    {
        newRule->evtSource = MakeNode<ASTIdentifier>();
        newRule->evtSource->name = ctx->event_source()->IDENTIFIER()->toString();
    }
    // get predicate
    if (ctx->predicate_section())
    {
        newRule->predicate = MakeNode<ASTBlock>();
        auto statements = ctx->predicate_section()->statement();
        for (const auto& stmt : statements)
        {
            auto* assignment = stmt->assignment();

            NodePtr<ASTDecl> decl = MakeNode<ASTDecl>();
            decl->name = assignment->IDENTIFIER()->toString();
            auto expr = assignment->expr();
            auto node = visitExpr(expr);
            decl->value = std::any_cast<NodePtr<ASTNode>>(node);

            newRule->predicate->statements.push_back(decl);
        }
    }

    if (ctx->condition_section())
    {
        auto node = visitCondition_section(ctx->condition_section());
        newRule->condition = NodeCast<>(node);
    }

    if (ctx->action_section())
    {
        auto node = visitAction_section(ctx->action_section());
        newRule->action = NodeCast<ASTArrayLiteral>(node);
    }
    return newRule;

}

std::any ASTBuilder::visitExpr(kdl_gen::KDLGrammarParser::ExprContext* ctx)
{
    if (ctx->binaryOp())
    {
        Operation op = getOperation(ctx->binaryOp());

        auto binOp = MakeNode<ASTBinaryOperation>();

        binOp->operation = op;

        binOp->lhs = std::any_cast<NodePtr<ASTNode>>(visit(ctx->expr(0)));
        binOp->rhs = std::any_cast<NodePtr<ASTNode>>(visit(ctx->expr(1)));

         return NodePtr<ASTNode>(binOp);
    }

    return visitPrimary(ctx->primary());

}

std::any ASTBuilder::visitField_access(kdl_gen::KDLGrammarParser::Field_accessContext* ctx)
{

    auto base = MakeNode<ASTFieldAccess>();
    auto* head = &base;

    size_t numIdentifiers = ctx->IDENTIFIER().size();

    for (size_t i = 0; i < numIdentifiers - 1; i++)
    {
        (*head)->fieldName = ctx->IDENTIFIER(i)->toString();
        auto newField = MakeNode<ASTFieldAccess>();
        (*head)->target = newField;
        head = &((*head)->target);
    }
    (*head)->fieldName = ctx->IDENTIFIER(numIdentifiers - 1)->toString();

    return NodePtr<ASTNode>(base);

}

std::any ASTBuilder::visitPrimary(kdl_gen::KDLGrammarParser::PrimaryContext* ctx)
{

    if (ctx->IDENTIFIER())
    {
        auto id = MakeNode<ASTIdentifier>();
        id->name = ctx->IDENTIFIER()->toString();
        return NodePtr<ASTNode>(id);
    }
    return visitChildren(ctx);
}

std::any ASTBuilder::visitArray_literal(kdl_gen::KDLGrammarParser::Array_literalContext* ctx)
{
    auto values = MakeNode<ASTArrayLiteral>();

    for (const auto& literal : ctx->literal())
    {
        values->values.push_back(NodeCast<>(visitLiteral(literal)));
    }
    
    return NodePtr<ASTNode>(values);
}

std::any ASTBuilder::visitLiteral(kdl_gen::KDLGrammarParser::LiteralContext* ctx)
{
    auto literal = MakeNode<ASTLiteral>();
    
    if (ctx->STRING())
    {
        auto value = ctx->STRING()->toString();
        value = value.substr(1, value.size() - 2);
        literal->value = value;
    }
    else 
        literal->value = ctx->toString();

    literal->type = getLiteralType(ctx);


    return NodePtr<ASTNode>(literal);
}

std::any ASTBuilder::visitCondition_section(kdl_gen::KDLGrammarParser::Condition_sectionContext* ctx)
{
    auto expr = visitExpr(ctx->expr());

    return NodeCast<>(expr);
}

std::any ASTBuilder::visitAction_section(kdl_gen::KDLGrammarParser::Action_sectionContext* ctx)
{
    auto array = MakeNode<ASTArrayLiteral>();
    for (const auto& id : ctx->IDENTIFIER())
    {
        auto identifier = MakeNode<ASTIdentifier>();
        identifier->name = id->toString();
        array->values.push_back(identifier);
    }

    return array;
}

Operation ASTBuilder::getOperation(kdl_gen::KDLGrammarParser::BinaryOpContext* ctx)
{
    if (ctx->LE())
        return Operation::LEQ;

    if (ctx->NE())
        return Operation::NE;

    if (ctx->LT())
        return Operation::LT;

    if (ctx->AND())
        return Operation::AND;

    if (ctx->OR())
        return Operation::OR;

    if (ctx->GE())
        return Operation::GEQ;

    if (ctx->GT())
        return Operation::GT;

    if (ctx->EQUAL())
        return Operation::EQUALS;

    if (ctx->IN())
    {
        if (ctx->NOT())
            return Operation::NOT_IN;
        return Operation::IN;
    }

    return Operation::INVALID;
}

ASTLiteral::Type ASTBuilder::getLiteralType(kdl_gen::KDLGrammarParser::LiteralContext* ctx)
{
    if (ctx->STRING())
        return ASTLiteral::Type::STRING;
    if (ctx->INTEGER())
        return ASTLiteral::Type::INTEGER;

    return ASTLiteral::Type::INVALID;
}

} // kdl