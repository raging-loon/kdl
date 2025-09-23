#ifndef KDLC_PARSER_TOKEN_H_
#define KDLC_PARSER_TOKEN_H_

#include <string_view>

namespace kdl
{

enum token_t
{
    /// OPERATORS
    KDL_T_OPEN_PARENTHESIS,
    KDL_T_CLOSE_PARENTHESIS,
    KDL_T_OPEN_BRACE,
    KDL_T_CLOSE_BRACE,
    KDL_T_OPEN_BRACKET,
    KDL_T_CLOSE_BRACKET,

    KDL_T_COMMA,
    KDL_T_SEMI_COLON,
    KDL_T_COLON,
    KDL_T_DOT,
    KDL_T_ASSIGNMENT,

    /// COMPARISON OPERATORS
    KDL_T_EQUALS,
    

    /// KEYWORDS
    KDL_T_RULE,
    KDL_T_EVT_SOURCE,
    KDL_T_CONDITION,
    KDL_T_ACTION,
    KDL_T_META,

    KDL_T_STRING,
    KDL_T_REGEX,
    KDL_T_INTEGER,

    KDL_NUM_TOKENS
};

struct Token
{
    token_t token;

    std::string_view value;
    /// Position of start of this token in source
    int startPosition;
    /// Position of start of line containing this token
    int lineStart;
    /// Used for error messages
    int lineNumber;
};

std::string_view getTokenName(token_t t);

} // kdl


#endif // KDLC_PARSER_TOKEN_H_