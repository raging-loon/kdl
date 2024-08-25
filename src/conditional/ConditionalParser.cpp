#include "conditional/ConditionalParser.h"
#include "parser/CompilerMessage.h"
using kdl::ConditionalParser;
using kdl::ConditionalTree;
using kdl::Token;
using kdl::token_t;
using kdl::CTokenPtr;


static constexpr bool isComparisonOperator(token_t t)
{
    return t >= token_t::CND_OF && t <= token_t::KW_EQUALITY;
}

static constexpr bool isJunctionOperator(token_t t)
{
    return t >= token_t::KW_OR && t <= token_t::KW_AND_OP;
}


constexpr bool ConditionalParser::isSingleVariable()
{
    return matchInOrder({
        token_t::TI_VARIABLE,
        token_t::SINGLE_VAR_IDENTIFIER,
        token_t::CLOSE_PARENTHESIS

        });
}


constexpr bool ConditionalParser::isMultiVariable()
{
    return matchInOrder({
        token_t::TI_VARIABLE,
        token_t::MULTI_VAR_IDENTIFIER,
        token_t::STAR,
        token_t::CLOSE_PARENTHESIS
        });

}

ConditionalParser::ConditionalParser(ConditionalTree& ctree, CTokenMapView& ctmv)
    : m_ctree(ctree), m_tokens(ctmv), m_numTokens(ctmv.size())
{
    m_cursor = 0;
}

bool kdl::ConditionalParser::parse()
{

    if (isJunctionOperator(m_tokens[0].t))
    {
        CompilerMessage::error(
            message_class_t::INVALID_SYNTAX,
            "Condition cannot start with 'and' or 'or'",
            &m_tokens[0]
        );
        return false;
    }

    if (isJunctionOperator(m_tokens.back().t))
    {
        CompilerMessage::error(
            message_class_t::INVALID_SYNTAX,
            "Condition cannot end with 'and' or 'or'",
            &m_tokens.back()
        );
        return false;
    }


    while (!isAtEnd())
    {
        if (!parseCondition())
        {
            return false;
        }
    }

    if (pLevel != 0)
    {
        auto* unclosedParenth = findMissingParenthesis();
        if (unclosedParenth)
        {
            CompilerMessage::error(
                message_class_t::INVALID_SYNTAX,
                "No closing parenthesis found",
                unclosedParenth
            );
            return false;
        }
    }

    return true;
}

bool kdl::ConditionalParser::isAtEnd()
{
    return m_cursor >= m_numTokens;
}

CTokenPtr kdl::ConditionalParser::advance()
{
    return &m_tokens[m_cursor++];
}

CTokenPtr kdl::ConditionalParser::peek()
{
    if (isAtEnd()) return nullptr;
    return &m_tokens[m_cursor];
}

CTokenPtr kdl::ConditionalParser::previous()
{
    if (m_cursor == 0) return nullptr;
    return &m_tokens[m_cursor - 1];
}

bool kdl::ConditionalParser::check(token_t t)
{
    if (isAtEnd()) return false;
    return peek()->t == t;
}

bool ConditionalParser::parseCondition()
{
    CTokenPtr start = advance();

    if (isAtEnd())
        return true;

    CTokenPtr next = peek();

    // test for a subcondition
    if (isComparisonOperator(next->t))
    {

        auto res = handleSubcondition(start);
        return res;

    }

    else if (isJunctionOperator(next->t))
        return handleJunction(start);

    else if (next->t == token_t::CLOSE_PARENTHESIS)
        return handleCloseParenthesis();

    else if (start->t == token_t::OPEN_PARENTHESIS)
        return handleOpenParenthesis();
    //else if (next->t == token_t::IDENTIFIER)
    //	return m_ctree.addVariableReference(next, pLevel, false);
    //else if (next->t == token_t::MULTI_VAR_IDENTIFIER)
    //	return m_ctree.addVariableReference(next, pLevel, true);


    else if (start->t == token_t::TI_VARIABLE)
        return handleVariable(start);


    else
        return false;
}

CTokenPtr ConditionalParser::findMissingParenthesis()
{
    int curPlevel = 0;

    for (int i = 0; i < m_numTokens; i++)
    {
        if (m_tokens[i].t == token_t::OPEN_PARENTHESIS)
            ++curPlevel;

        if (curPlevel == pLevel)
            return &m_tokens[i];
    }

    return nullptr;

}

bool ConditionalParser::handleJunction(CTokenPtr op)
{

    if (isJunctionOperator(op->t))
    {
        CompilerMessage::error(
            message_class_t::INVALID_SYNTAX,
            "Expected subcondition, got junction",
            op
        );
        return false;
    }

    //printf("Junction Found: %s\n", kdl::getTokenName(peek()->t));

    bool result = m_ctree.addJunction(peek(), pLevel);

    advance();

    return result;
}

bool ConditionalParser::handleVariable(CTokenPtr start)
{
    if (peek()->t == token_t::SINGLE_VAR_IDENTIFIER)
        return m_ctree.addVariableReference(peek(), pLevel, false);

    if (peek()->t == token_t::MULTI_VAR_IDENTIFIER)
        return m_ctree.addVariableReference(peek(), pLevel, true);

    return false;

}

bool ConditionalParser::handleOpenParenthesis()
{

    int variableNamePos = m_cursor + 1;
    if (isMultiVariable())
    {
        //	printf("we've gut a multivariable\n");
        return m_ctree.addVariableReference(&m_tokens[variableNamePos], pLevel, true);
    }

    if (isSingleVariable())
    {
        //	printf("we've gut a single variable\n");
        return m_ctree.addVariableReference(&m_tokens[variableNamePos], pLevel, false);
    }

    pLevel++;

    return true;
}

bool ConditionalParser::handleCloseParenthesis()
{
    if ((pLevel - 1) < 0)
        return false;

    int copy = pLevel;

    m_ctree.merge(copy);

    pLevel--;

    return true;

}

bool ConditionalParser::handleSubcondition(CTokenPtr start)
{

    advance();

    /*printf("Sub Condition Found: %s %s %s\n",
        kdl::getTokenName(start->t),
        kdl::getTokenName(previous()->t),
        kdl::getTokenName(peek()->t)
    );*/
    CTokenPtr op = previous();

    CTokenPtr rval = peek();
    if (peek()->t == token_t::TI_VARIABLE)
    {
        advance();
        if (peek()->t != token_t::IDENTIFIER &&
            peek()->t != token_t::MULTI_VAR_IDENTIFIER &&
            peek()->t != token_t::SINGLE_VAR_IDENTIFIER)
        {
            printf("ERrorroror\n");
            return false;
        }
        rval = peek();
    }
    return m_ctree.addSubCondition(op, start, rval, pLevel);

}
