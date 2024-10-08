#include <cstdarg>
#include <span>
#include <iostream>
#include <iomanip>

#include "CompilerMessage.h"
#include "parser.h"
#include "tokens.h"
#include "RuleParser.h"
#include "Combinator.h"
using kdl::Combinator;
using kdl::Token;
using kdl::Lexer;
using kdl::TokenList;

Combinator::Combinator(const TokenList& t)
    : m_tokenList(t), m_numTokens(t.size())
{
    m_cursor = 0;
}

bool Combinator::parse()
{
    while (!isAtEnd())
    {
        // if there was an error
        if (!nextBlock())
            return false;
    }
    return true;
}

void Combinator::dumpBlocks()
{
    printf("\n+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
        "|         name        | start position | end position |\n"
        "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");

    std::string toPrint;
    for (auto& i : m_ruleBlocks)
    {
        toPrint = i[1].val;
        if (toPrint.length() > 21)
            toPrint = toPrint.substr(0, 18) + "...";

        std::cout << '|' << std::left << std::setw(21) << toPrint
            << '|' << std::left << std::setw(16) << i.front().startPos
            << '|' << std::left << std::setw(14) << i.back().startPos
            << "|\n";
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}

const Token* Combinator::advance()
{
    return &m_tokenList[m_cursor++];
}

bool Combinator::isAtEnd()
{
    return m_cursor >= m_numTokens;
}

const Token* Combinator::peek()
{
    if (isAtEnd())
        return nullptr;
    return &m_tokenList[m_cursor];
}

const Token* Combinator::previous()
{
    if (m_cursor == 0)
        return nullptr;

    return &m_tokenList[m_cursor - 1];
}


bool Combinator::check(token_t t)
{
    if (isAtEnd())
        return false;
    return peek()->t == t;
}

int Combinator::getBlockEndIndex()
{
    const Token* tok = nullptr;
    while ((tok = peek()) != nullptr)
    {

        // open brace found. 
        // This one never terminated, throw an error
        if (tok->t == token_t::OPEN_BRACE)
            return -1;
        if (tok->t == token_t::CLOSE_BRACE)
        {
            int index = m_cursor;


            return index + 1;
        }
        advance();

    }

    return -1;
}

bool Combinator::nextBlock()
{

    // Todo: split this into a couple of functions for
    //		 rules and policies
    int ruleStart = m_cursor;
    //							rule TestRule{
    if (matchInOrder({ token_t::SI_RULES, token_t::IDENTIFIER, token_t::OPEN_BRACE }) ||
        //							rule TestRule : Net {
        matchInOrder({ token_t::SI_RULES, token_t::IDENTIFIER, token_t::COLON, token_t::IDENTIFIER, token_t::OPEN_BRACE }))
    {

        // Used for error handling
        CTokenPtr braceToken = previous();

        int sectionEnd = getBlockEndIndex();
        if (sectionEnd == -1)
        {
            CompilerMessage::error(message_class_t::INVALID_SYNTAX, "No closing brace", braceToken);
            return false;

        }
        m_ruleBlocks.push_back(CTokenMapView(m_tokenList.data() + ruleStart, (sectionEnd - ruleStart)));

        /*Rule r;
        RuleParser p(m_ruleBlocks.back(), r);

        p.parse();
        */
        m_cursor = sectionEnd;
        return true;
    }
    return false;

}
