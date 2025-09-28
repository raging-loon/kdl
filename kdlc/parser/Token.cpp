#include "Token.h"

#include <array>
#include <cassert>
#include <unordered_map>
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
    "ASTERISK",
    "PLUS",
    "MINUS",
    "EQUALS",
    "LEQ",
    "LT",
    "NE",
    "NOT",
    "GT",
    "GEQ",
    "RULE",
    "EVT_SOURCE",
    "CONDITION",
    "ACTION",
    "META",
    "KDL_T_PREDICATE",
    "STRING",
    "REGEX",
    "INTEGER",
    "IDENTIFIER"
};

static const std::unordered_map<
    std::string_view,
    token_t
> s_KeywordMap = {
    { "rule",           KDL_T_RULE },
    { "condition",      KDL_T_CONDITION},
    { "action",         KDL_T_ACTION },
};
std::string_view GetTokenName(token_t t)
{
    assert((int)t < KDL_NUM_TOKENS);

    return TOKEN_STRS[t];
}

token_t GetKeyWord(const std::string_view& str)
{
    auto iter = s_KeywordMap.find(str);

    if (iter != s_KeywordMap.cend())
        return iter->second;

    return static_cast<token_t>(-1);
}

} // kdl