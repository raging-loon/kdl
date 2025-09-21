#include "parser/parser.h"

using kdl::Parser;
using kdl::Token;
using kdl::token_t;

bool Parser::match(token_t t)
{
    if (check(t))
    {
        advance();
        return true;
    }

    return false;
}

bool Parser::match(std::initializer_list<token_t> t)
{
    for (auto i : t)
    {
        if (check(i))
        {
            advance();
            return true;
        }
    }
    return false;
}

bool Parser::matchInOrder(std::initializer_list<token_t> t)
{
    setReturnMarker();
    for (auto i : t)
    {
        if (peek()->t != i)
        {
            returnToMarker();
            return false;
        }

        advance();
    }

    return true;
}