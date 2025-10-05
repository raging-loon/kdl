#ifndef KDLC_PARSER_PARSER_H_
#define KDLC_PARSER_PARSER_H_

#include "Token.h"
#include "ast/ASTNode.h"
#include "context/SourceManager.h"
namespace kdl
{

class Parser
{
public:

    Parser(
        const std::vector<Token>& tokens,
        FileID fid
    );

    bool parse();

private:

    inline bool isAtEnd() 
    {
        return m_cursor >= m_tokens.size();
    }

    inline const TokenPtr advance()
    {
        if (isAtEnd())
            return nullptr;
        return (TokenPtr)&m_tokens[m_cursor++];
    }


    ///
    /// @brief
    ///     Parse a rule block
    /// 
    /// @details
    ///     This function expects the current tokens to be 
    ///     <rule> <identifier> <open bracket>
    ///         <sections>
    ///     <close bracket>
    /// 
    NodePtr<ASTRule> parseRuleDecl();

    ///
    /// @brief
    ///     Parse an event source
    ///     This should be:
    ///     <evt_source> <colon> <identifier>
    ///     An Identifier node will be created with this value
    /// 
    NodePtr<ASTIdentifier> parseEvtSource();

    ///
    /// @brief
    ///     A predicate is a series of variable declarations to be used
    ///     both further in the predicate as well as in the condition section
    /// 
    NodePtr<ASTBlock> parsePredicate();

    ///
    /// @brief
    ///     Parse a variable declaration
    /// 
    ///     This looks like:
    ///     <VARIABLE> <IDENTIFIER> <EQUALS> [expression] <SEMICOLON>
    ///     Where in text this would be:
    ///     $var_name = <expression>;
    /// 
    NodePtr<ASTDecl> parseDecl();

    ///
    /// @brief
    ///     Parse a primary sub expression
    ///     This includes
    ///         - Literals
    ///         - Identifiers
    ///         -
    NodePtr<ASTNode> parsePrimary();
    
    ///
    /// @brief
    ///     Parse unary sub expressions
    ///     This includes mainly negation
    /// 
    NodePtr<ASTNode> parseUnary();

    ///
    /// @brief 
    ///     Parse multiplication and division operations 
    /// 
    NodePtr<ASTNode> parseFactor();
    
    ///
    /// @brief
    ///     Parse addition and subtraction operations
    /// 
    NodePtr<ASTNode> parseTerm();

    NodePtr<ASTNode> parseComparison();

    NodePtr<ASTNode> parseEquality();

    NodePtr<ASTLiteral> parseArray();
    
    NodePtr<ASTIdentifier> parseIdentifier();

    NodePtr<ASTFieldAccess> parseFieldAccess();
    template <std::same_as<token_t>... Tokens>
    inline bool matchMany(Tokens&&... tokens)
    {
        auto list = std::initializer_list<token_t>{ tokens... };

        return matchManyInternal(list);
    }

    bool matchManyInternal(const std::initializer_list<token_t>&);


    ///
    /// @brief
    ///     Peek at the current token
    /// 
    /// @returns null if at end
    ///
    const TokenPtr peek();

    ///
    /// @brief
    ///     Get the previous token
    /// 
    const TokenPtr previous();

    ///
    /// @brief
    ///     Test if the current token is t
    ///     advance if so
    /// 
    bool match(token_t t);

    ///
    /// @brief
    ///     Consume a token t. If it is not found,
    ///     print an error
    /// 
    void consume(token_t t, const std::string_view& errorMsg);

    ///
    /// @brief
    ///     Consume a token t and return its value
    ///     Print an error if not found
    /// 
    const TokenPtr consumeValue(token_t tok, const std::string_view& error);




    ///
    /// @brief
    ///     Print an error about the current token
    /// 
    void errorAtCurrent(const std::string_view& errorMsg);



private:

    const std::vector<Token>& m_tokens;
    FileID m_fileID;
    int m_cursor;
    bool m_error;
    NodePtr<ASTProgram> m_programRoot;
};


} // kdl

#endif //KDLC_PARSER_PARSER_H_