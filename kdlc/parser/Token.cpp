#include "Token.h"

#include <array>
#include <cassert>

namespace kdl
{

constexpr std::array<std::string_view, KDL_NUM_TOKENS> TOKEN_STRS = 
{
    "OPEN_PARENTHESIS",
    "CLOSE_PARENTHESIS",
    "OPEN_BRACE",
    "CLOSE_BRACE",
    "OPEN_BRACKET",
    "CLOSE_BRACKET",
    "COMMA",
    "SEMI_COLON",
    "COLON",
    "DOT",
    "ASSIGNMENT",
    "EQUALS",
    "RULE",
    "EVT_SOURCE",
    "CONDITION",
    "ACTION",
    "META",
    "STRING",
    "REGEX",
    "INTEGER",
};

std::string_view getTokenName(token_t t)
{
    assert((int)t < KDL_NUM_TOKENS);

    return TOKEN_STRS[t];
}

} // kdl