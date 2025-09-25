#ifndef KDLC_PARSER_LEXER_LEXER_H_
#define KDLC_PARSER_LEXER_LEXER_H_

#include <string_view>
#include <string>
#include <vector>
#include <cassert>

#include "parser/Token.h"
#include "context/SourceManager.h"
namespace kdl
{

class Lexer
{
public:

    Lexer(FileID id);

    ///
    /// @brief
    ///     Scan tokens until there are none left, 
    ///     or there was an error
    /// 
    /// @return 
    ///     Number of tokens or -1 if failed
    /// 
    int scan();

    /// @brief Dump all scanned tokens
    void dumpTokens();

    const auto& getTokens() const { return m_tokens; }

private:

    void scanToken();
    /// @brief: go to the next char
    inline char advance()
    {
        assert(!atEnd());
        return m_source->contents[m_current++];
    }

    inline bool atEnd() const 
    {
        return m_current >= m_source->contents.length();
    }

    ///
    /// @brief
    ///     Add a token without an associated value
    /// 
    void addToken(token_t tok);

    ///
    /// @brief
    ///    Add a token whose value is stored in 'str'
    /// 
    void addTokenString(token_t tok, std::string_view str);

    ///
    /// @brief
    ///     Add a token whose value is a substring
    ///     of m_source from 'start' to 'end'
    /// 
    /// @detail
    ///     This internally calls @ref addTokenString
    /// 
    void addTokenSubstring(token_t, int start, int end);

    ///
    /// @brief
    ///     Return m_current + 1
    ///     Returns '\0' if end is reached
    /// 
    char peek();

    ///
    /// @brief
    ///     Test if the next character is n
    ///     and advance the cursor if so
    /// 
    /// @detail
    ///     This is used to match tokens such as '<=' given
    ///     the character '<'
    /// 
    bool match(char n);
    
    ///
    /// @brief
    ///     Get the previous character
    ///     Returns '\0' if the current character
    ///     is the first
    /// 
    char previous();

    ///
    /// @brief
    ///     Scan from start of comment ('#')
    ///     to the end of the current line
    /// 
    void scanComment();

    ///
    /// @brief
    ///     Reset the line variables 
    ///     
    void nextLine();

    ///
    /// @brief
    ///     Section a section from 'target' to the location of the next next target,
    ///     unless it is escaped (e.g. if the target is '"', '\"' 
    ///     would be included in the section)
    /// 
    void scanSection(char target, bool errorOnNewline = false);
private:
    /// @brief Source being lexed
    const FileInfo* m_source;

    /// @brief Tokens found
    std::vector<Token> m_tokens;
    
    /// @brief current position in m_source
    int m_current;
    
    /// @brief start position of current token
    int m_start;

    int m_currentLine;
    /// 
    /// @brief
    ///     Position of the start of the current line
    ///     Used for printing errors
    /// 
    int m_lineStartPos;

    /// 
    /// @brief
    ///     Set if there was an error during lexing
    ///     Used to return early when scanning
    /// 
    bool m_error;


};

} // kdl


#endif // KDLC_PARSER_LEXER_LEXER_H_