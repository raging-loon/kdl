#ifndef KDLC_PARSER_TOKEN_H_
#define KDLC_PARSER_TOKEN_H_

#include <string_view>

namespace kdl
{

enum token_t
{
#define KDL_TOKEN(name) KDL_T_##name,
#include "Token.def.h"
   
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

using TokenPtr = Token*;

///
/// PURPOSE
///     Get the name of a token
/// 
std::string_view GetTokenName(token_t t);

///
/// PURPOSE
///     Given a keyword, return the token
///     Returns -1 on fail
/// 
token_t GetKeyWord(const std::string_view& str);

} // kdl


#endif // KDLC_PARSER_TOKEN_H_