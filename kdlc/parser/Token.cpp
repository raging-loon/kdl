#include "Token.h"

#include <array>
#include <cassert>
#include <unordered_map>
namespace kdl
{

constexpr auto TOKEN_STRS = std::to_array<std::string_view>(
{
#define KDL_TOKEN(name) #name,
#include "Token.def.h"
});

static const std::unordered_map<
    std::string_view,
    token_t
> s_KeywordMap = {
#define KDL_TOKEN(name)
#define KDL_SYMBOL_TOKEN(name, sym)
#define KDL_KEYWORD_TOKEN(name, kw) { kw, KDL_T_##name }

};
std::string_view GetTokenName(token_t t)
{
    assert((int)t < TOKEN_STRS.size());

    return TOKEN_STRS[t];
}

token_t GetKeyWord(const std::string_view& str)
{
    auto iter = s_KeywordMap.find(str);

    if (iter != s_KeywordMap.cend())
        return iter->second;

    return KDL_T_ERROR;
}

} // kdl