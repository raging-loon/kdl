#include "Parser.h"

#include "interface/Reporting.h"
#include "ast/ASTDumper.h"
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
    while (!match(KDL_T_EOS) && !isAtEnd() && !m_error)
    {
        if (match(KDL_T_RULE))
        {
            m_programRoot->topDecls.push_back(parseRuleDecl());
        }
    }

    if (!m_error)
    {
        ASTDumper d{};
        d.dump(m_programRoot);
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
const TokenPtr Parser::next()
{
    if (m_cursor + 1 >= m_tokens.size())
        return nullptr;

    return (TokenPtr)&m_tokens[m_cursor + 1];
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

    while (!match(KDL_T_CLOSE_BRACE))
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

            case KDL_T_CONDITION:
                assert(rule->condition == nullptr);
                advance();
                rule->condition = parseCondition();
                break;
        }
    }
    
    consume(KDL_T_CLOSE_BRACE, "Expected '}}'");

    return rule;
}

NodePtr<ASTIdentifier> Parser::parseEvtSource()
{
    EXPECT_TOKEN(KDL_T_EVT_SOURCE, "Expected 'evt_source'");
    EXPECT_TOKEN(KDL_T_COLON, "Expected ':'");
    const auto src = consumeValue(KDL_T_IDENTIFIER, "Expected identifier");

    auto evtSrcNode = std::make_unique<ASTIdentifier>(src);

    evtSrcNode->name = src->value;
    advance();
    return evtSrcNode;
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

NodePtr<ASTBinaryOperation> Parser::parseCondition()
{
    EXPECT_OR_RETURN(KDL_T_COLON, "Expected ':'", nullptr);

    //auto cTreeRoot = MakeNode<ASTBinaryOperation>()

    return nullptr;
}

NodePtr<ASTDecl> Parser::parseDecl()
{

    EXPECT_TOKEN(KDL_T_VARIABLE, "Expected '$' to precede variable name");

    const auto name = consumeValue(KDL_T_IDENTIFIER, "Expected Identifier");
    EXPECT_TOKEN(KDL_T_ASSIGNMENT, "Expected \"=\"");

    auto decl = MakeNode<ASTDecl>(name);

    decl->name = name->value;

    decl->value = parseEquality();

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
    if (match(KDL_T_IDENTIFIER))
    {
        advance();
        if (match(KDL_T_DOT))
            return parseFieldAccess();
        return parseIdentifier();
    }
    if (match(KDL_T_VARIABLE))
    {
        advance();
        const auto variable = consumeValue(KDL_T_IDENTIFIER, "Expected an identifier");
        auto identifier = MakeNode<ASTIdentifier>(variable);
        identifier->name = variable->value;
        return identifier;

    }
    if (match(KDL_T_OPEN_PARENTHESIS))
    {
        advance();
        auto expr = parseEquality();
        consume(KDL_T_CLOSE_PARENTHESIS, "Expected closing ')' after expression");
        return expr;
    }
    if (match(KDL_T_OPEN_BRACKET))
    {
        advance();
        auto expr = parseArray();

        return expr;
    }

    return nullptr;
}
NodePtr<ASTFieldAccess> Parser::parseFieldAccess()
{
    auto token = previous();
    auto baseAccess = MakeNode<ASTFieldAccess>(token);
    baseAccess->fieldName = token->value;

    auto* header = &baseAccess;

    while (match(KDL_T_DOT))
    {
        advance();
        auto id = consumeValue(KDL_T_IDENTIFIER, "Expected Identifier");
        if (!id)
            return nullptr;
        
        auto fieldAccess = MakeNode<ASTFieldAccess>(id);
        fieldAccess->fieldName = id->value;

        (*header)->target = std::move(fieldAccess);

        header = &(*header)->target;


    }

    return baseAccess;
}

NodePtr<ASTLiteral> Parser::parseArray()
{
    auto array = MakeNode<ASTLiteral>(previous());
    array->type = ASTLiteral::ARRAY;
    array->value = std::vector<NodePtr<ASTNode>>{};
    auto& values = std::get<1>(array->value);

    while (!match(KDL_T_CLOSE_BRACKET))
    {
        auto value = parseEquality();
        values.push_back(std::move(value));
        if (!match(KDL_T_CLOSE_BRACKET))
        {
            EXPECT_OR_RETURN(KDL_T_COMMA, "Expected comma", nullptr);
        }
    }
 
    EXPECT_OR_RETURN(KDL_T_CLOSE_BRACKET, "Expected ']'", nullptr);

    return array;
}

NodePtr<ASTIdentifier> Parser::parseIdentifier()
{
    auto id = previous();
    auto identifier = MakeNode<ASTIdentifier>(id);
    identifier->name = id->value;
    return identifier;
}

NodePtr<ASTNode> Parser::parseUnary()
{
    if (matchMany(KDL_T_LOGICAL_NOT, KDL_T_NOT))
    {
        TokenPtr op = previous();
        auto rhs = parseUnary();
        
        auto unaryExpr = MakeNode<ASTUnaryOperation>(op);
        unaryExpr->operation = op->token;
        unaryExpr->operand = std::move(rhs);

        return unaryExpr;
    }

    return parsePrimary();
}

NodePtr<ASTNode> Parser::parseFactor()
{
    auto lhs = parseUnary();

    while (matchMany(KDL_T_ASTERISK, KDL_T_DIV))
    {
        TokenPtr op = previous();
        
        auto rhs = parseUnary();

        auto expr = MakeNode<ASTBinaryOperation>(op);

        expr->operation = op->token;
        expr->lhs = std::move(lhs);
        expr->rhs = std::move(rhs);

        lhs = std::move(expr);
    }

    return lhs;
}

NodePtr<ASTNode> Parser::parseTerm()
{
    auto lhs = parseFactor();

    while (matchMany(KDL_T_PLUS, KDL_T_MINUS, KDL_T_NOT))
    {
        TokenPtr op = previous();
        token_t operation = op->token;
        if (op->token == KDL_T_NOT && peek()->token == KDL_T_IN)
        {
            advance();
            operation = KDL_T_NOT_IN;
        }
        auto rhs = parseFactor();

        auto expr = MakeNode<ASTBinaryOperation>(op);

        expr->operation = operation;
        expr->lhs = std::move(lhs);
        expr->rhs = std::move(rhs);

        lhs = std::move(expr);
    }

    return lhs;
}

NodePtr<ASTNode> Parser::parseComparison()
{
    auto lhs = parseTerm();

    while (matchMany(KDL_T_GT, KDL_T_GEQ, KDL_T_LT, KDL_T_LEQ))
    {
        TokenPtr op = previous();
        auto rhs = parseTerm();

        auto expr = MakeNode<ASTBinaryOperation>(op);

        expr->operation = op->token;
        expr->lhs = std::move(lhs);
        expr->rhs = std::move(rhs);

        lhs = std::move(expr);
    }

    return lhs;
}

NodePtr<ASTNode> Parser::parseEquality()
{
    auto lhs = parseComparison();
    while (matchMany(KDL_T_EQUALS, KDL_T_NE, KDL_T_IN))
    {
        TokenPtr op = previous();
        auto rhs = parseComparison();

        auto expr = MakeNode<ASTBinaryOperation>(op);

        expr->operation = op->token;
        expr->lhs = std::move(lhs);
        expr->rhs = std::move(rhs);

        lhs = std::move(expr);
    }

    return lhs;
}


void Parser::errorAtCurrent(
    const std::string_view& errorMsg
)
{
    int tokLoc = m_cursor;
    if (isAtEnd())
        tokLoc = m_tokens.size() - 1;
    const Token& t = m_tokens.at(tokLoc);

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