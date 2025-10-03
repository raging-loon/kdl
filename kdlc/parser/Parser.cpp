#include "Parser.h"

#include "interface/Reporting.h"

#include <cassert>

#define EXPECT_OR_RETURN(token, message, retval)    \
{                                                   \
    consume(token, message);                        \
    if (m_error)                                    \
        return retval;                              \
}

#define EXPECT_TOKEN(token, message) EXPECT_OR_RETURN(token, message, nullptr)
namespace kdl
{

Parser::Parser(
    const std::vector<Token>& tokens,
    FileID fid

) : m_tokens{ tokens },
    m_cursor{ 0 },
    m_fileID{ fid },
    m_programRoot{ nullptr },
    m_error{ false }
{

    m_programRoot = std::make_unique<ASTProgram>(nullptr);
}

bool Parser::parse()
{
    while (!isAtEnd() && !m_error)
    {
        if (match(KDL_T_RULE))
        {
            m_programRoot->topDecls.push_back(parseRuleDecl());
        }
    }
    return m_error;
}

bool Parser::matchManyInternal(const std::initializer_list<token_t>& list)
{
    if (isAtEnd())
        return false;
    for (auto tok : list)
    {
        if (peek()->token == tok)
        {
            advance();
            return true;
        }
    }
    return false;
}

const TokenPtr Parser::peek()
{
    if (isAtEnd())
        return nullptr;

    return (TokenPtr)(&m_tokens[m_cursor]);
}

const TokenPtr Parser::previous()
{
    if (m_cursor == 0)
        return nullptr;

    return (TokenPtr)(&m_tokens[m_cursor - 1]);
}

void Parser::consume(
    token_t t,
    const std::string_view& errorMsg
)
{
    if (!match(t))
        errorAtCurrent(errorMsg);
    else
        advance();

}

const TokenPtr Parser::consumeValue(token_t tok, const std::string_view& error)
{
    if (peek()->token != tok)
    {
        errorAtCurrent(error);
        return nullptr;
    }
    const TokenPtr token = peek();
    advance();
    return token;
}

bool Parser::match(token_t t)
{
    if (isAtEnd())
        return false;

    return peek()->token == t;
}

NodePtr<ASTRule> Parser::parseRuleDecl()
{
    EXPECT_TOKEN(KDL_T_RULE, "Expected 'rule' declaration");

    auto* name = consumeValue(KDL_T_IDENTIFIER, "Expected an identifier");

    if (!name)
        return nullptr;

    EXPECT_TOKEN(KDL_T_OPEN_BRACE, "Expected '{{'");

    auto rule = std::make_unique<ASTRule>(name);

    rule->name = name->value;

    while (!match(KDL_T_CLOSE_BRACE) && !isAtEnd())
    {
        auto* cur = peek();

        switch (cur->token)
        {
            case KDL_T_EVT_SOURCE:
                assert(rule->evtSource == nullptr);
                rule->evtSource = parseEvtSource();
                break;

            case KDL_T_PREDICATE:
                assert(rule->predicate == nullptr);
                rule->predicate = parsePredicate();
                break;
        }

        advance();

    }


    return rule;
}

NodePtr<ASTIdentifier> Parser::parseEvtSource()
{
    EXPECT_TOKEN(KDL_T_EVT_SOURCE, "Expected 'evt_source'");
    EXPECT_TOKEN(KDL_T_COLON, "Expected ':'");
    const auto src = consumeValue(KDL_T_IDENTIFIER, "Expected identifier");

    auto evtSrcNode = std::make_unique<ASTIdentifier>(src);

    evtSrcNode->name = src->value;

    return nullptr;
}

NodePtr<ASTBlock> Parser::parsePredicate()
{
    EXPECT_TOKEN(KDL_T_PREDICATE, "Expected 'predicate'");
    EXPECT_TOKEN(KDL_T_COLON, "Expected ':'");

    auto block = MakeNode<ASTBlock>(nullptr);
    while (match(KDL_T_VARIABLE) && !isAtEnd())
    {

        block->statements.push_back(parseDecl());

        if (m_error)
            break;
    }


    return block;
}

NodePtr<ASTDecl> Parser::parseDecl()
{

    EXPECT_TOKEN(KDL_T_VARIABLE, "Expected '$' to precede variable name");

    const auto name = consumeValue(KDL_T_IDENTIFIER, "Expected Identifier");
    EXPECT_TOKEN(KDL_T_ASSIGNMENT, "Expected \"=\"");

    auto decl = MakeNode<ASTDecl>(name);

    decl->name = name->value;

    decl->value = parseUnary();

    EXPECT_TOKEN(KDL_T_SEMI_COLON, "Expected semicolon");
    return decl;

}

NodePtr<ASTNode> Parser::parsePrimary()
{
    if (matchMany(KDL_T_INTEGER, KDL_T_STRING))
    {
        auto token = previous();
        auto literal = MakeNode<ASTLiteral>(token);

        literal->type = (ASTLiteral::Type)token->token;

        literal->value = std::string(token->value);
        return literal;
    }
    errorAtCurrent("Expected a literal");
    return nullptr;
}

NodePtr<ASTNode> Parser::parseUnary()
{
    if (matchMany(KDL_T_NOT, KDL_T_LOGICAL_NOT))
    {
        token_t op = peek()->token;
        auto rhs = parseUnary();
        
        auto unaryExpr = MakeNode<ASTUnaryOperation>(peek());
        unaryExpr->operation = op;
        unaryExpr->operand = std::move(rhs);

        return unaryExpr;
    }

    return parsePrimary();
}

void Parser::errorAtCurrent(
    const std::string_view& errorMsg
)
{
    const Token& t = m_tokens.at(m_cursor);

    ReportInfo ri{};

    ri.fileID = m_fileID;

    ri.location.lineNumber = t.lineNumber;
    ri.location.lineOffset = t.lineStart;
    ri.location.locationStart = t.startPosition;

    if (!t.value.empty())
        ri.location.locationEnd = (t.startPosition + t.value.size());
    else
        ri.location.locationEnd = 0;

    Report::Error(ri, errorMsg);

    m_error = true;
}

} // kdl