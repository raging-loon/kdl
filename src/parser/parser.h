#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <span>

#include "lex.h"

namespace kdl
{

using CTokenPtr = const Token*;

using CTokenMapIter = TokenList::const_iterator;

/// This is used to view a subsection of a TokenMap
/// without creating copies.
using CTokenMapView = std::span<const Token>;

using RuleBlockList = std::vector< CTokenMapView >;
///
/// PURPOSE
///		General Parser Class
///		To reduce code duplication, functions shared by
///		different parser are found here
///		
///		All pure virtual function will rely on the specific
///		parser to provide an implementation based on the 
///		data structure it is using to manage tokens.
///
class Parser
{
public:

    virtual bool parse() = 0;


protected:

    /// Points to somewhere in a subclass's data structure
    int m_cursor;
    /// Used by set/returnToMarker
    int m_returnMarker;

    virtual inline bool isAtEnd() = 0;

    /// 
    /// PURPOSE 
    ///		Increment m_cursor
    ///		returns where m_cursor was
    /// 
    virtual inline CTokenPtr advance() = 0;

    ///
    /// PURPOSE
    ///		Get m_cursor + 1
    ///		Returns nullptr if its reached the end
    /// 
    virtual CTokenPtr peek() = 0;

    ///
    /// PURPOSE
    ///		Get m_cursor - 1
    ///		Returns nullptr if we are at the beginning
    ///
    virtual CTokenPtr previous() = 0;

    /// 
    /// PURPOSE
    ///		Match a single token type
    /// 
    virtual bool match(token_t t);

    /// 
    /// PURPOSE
    ///		Match multiple token types
    ///		Will return true if the next token is any of t
    /// 
    virtual bool match(std::initializer_list<token_t> tokenList);

    /// 
    /// PURPOSE
    ///		Match multiple token types in order
    ///		Will only return true if the next n tokens
    ///		match the corresponding n tokens in t
    ///
    virtual bool matchInOrder(std::initializer_list<token_t> tokenList);

    /// 
    /// PURPOSE
    ///		Check if the token at m_cursor
    ///		is 't'
    /// 
    virtual bool check(token_t t) = 0;

    ///
    /// PURPOSE
    ///		Save the cursors current location
    ///		Return to it via `returnToMarker`
    ///	
    virtual void setReturnMarker()
    {
        m_returnMarker = m_cursor;
    }

    ///
    /// PURPOSE
    ///		Set cursor to the return marker
    ///
    virtual void returnToMarker()
    {
        m_cursor = m_returnMarker;
    }

};

}

#endif // PARSER_PARSER_H_