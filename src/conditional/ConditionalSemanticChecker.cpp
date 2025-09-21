#include "conditional/ConditionalSemanticChecker.h"
#include "parser/CompilerMessage.h"
#include "parser/tokens.h"
using kdl::ConditionalSemanticChecker;
using kdl::CTokenPtr;
using kdl::Token;
using kdl::token_t;


bool ConditionalSemanticChecker::isValidSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right)
{
    switch (op->t)
    {
        case token_t::CND_OF:
            return isValid_CND_OF_Operation(left, right);
        case token_t::KW_GT:
        case token_t::KW_GEQ:
        case token_t::KW_LT:
        case token_t::KW_LEQ:
        case token_t::KW_EQUALITY:
            return isValidRelationalOperation(left, right);


        default:
            return false;

    }
}

bool ConditionalSemanticChecker::isValid_CND_OF_Operation(CTokenPtr left, CTokenPtr right)
{

    if (right->t != token_t::MULTI_VAR_IDENTIFIER && right->t != token_t::CND_THEM)
    {
        return error(right, "Expected multivariable or 'them'");
    }

    if (left->t != token_t::CND_ALL &&
        left->t != token_t::CND_ANY &&
        left->t != token_t::INTEGER)
    {
        return error(left, "Expected 'all', 'any' or integer.");
    }

    if (left->t == token_t::INTEGER && std::stoi(left->val) == 0)
        CompilerMessage::warning(
            "Left side of conditional is zero.\nThis could result in unnecessary branching or code generation.",
            left
        );


    return true;
}

bool ConditionalSemanticChecker::isValidRelationalOperation(CTokenPtr left, CTokenPtr right)
{
    if (left->t != token_t::SINGLE_VAR_IDENTIFIER && left->t != token_t::IDENTIFIER)
        return error(left, "Expected identifier");

    if (right->t != token_t::INTEGER)
        return error(right, "Expected an integer");



    return true;
}

bool kdl::ConditionalSemanticChecker::error(CTokenPtr bad, const char* message)
{
    CompilerMessage::error(
        message_class_t::INVALID_SEMANTICS,
        message,
        bad
    );

    return false;
}
