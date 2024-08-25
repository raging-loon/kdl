#ifndef CONDITIONAL_CONDITIONAL_PARSER_H_
#define CONDITIONAL_CONDITIONAL_PARSER_H_

#include "parser/parser.h"
#include "ConditionalTree.h"
namespace kdl
{

class ConditionalParser : private Parser
{
public:

    ConditionalParser(ConditionalTree& ctree, CTokenMapView& ctmv);

    /// 
    /// PURPOSE
    ///		A recursive function that addes junctions/subconditions to 
    ///		the Conditional tree until it runs out of tokens
    /// 
    bool parse() override;

private:

    ConditionalTree& m_ctree;
    CTokenMapView& m_tokens;

    int m_numTokens;
    int pLevel = 0;
    // Inherited via Parser

    inline bool isAtEnd() override;

    CTokenPtr advance() override;

    CTokenPtr peek() override;

    CTokenPtr previous() override;

    bool check(token_t t) override;

    ///
    /// PURPOSE
    ///		Parse a subcondition/junction and add it 
    ///		to the ConditionalTree
    /// 
    ///		This function will call the various handlers
    ///		below
    /// 
    bool parseCondition();

    ///
    /// PURPOSE
    ///		Find the parenthesis that matches our plevel
    /// 
    CTokenPtr findMissingParenthesis();


    constexpr bool isMultiVariable();
    constexpr bool isSingleVariable();

    ///
    /// PURPOSE
    ///		Add a junction to the tree
    ///		Will throw an error if the previous token
    ///		was also a junction
    /// 
    bool handleJunction(CTokenPtr op);

    bool handleVariable(CTokenPtr start);

    ///
    /// PURPOSE
    ///		This will do one of three things:
    ///			1. Increase the parenthesis level
    ///			2. Add a single variable to the tree
    ///				e.g. '(' then '$name)'
    /// 		3. Add a multivarible to the tree
    ///				e.g. '(' then '$nam*)'
    ///		
    bool handleOpenParenthesis();

    ///
    /// PURPOSE
    ///		Merge sub-trees and decrease parenthesis level
    bool handleCloseParenthesis();

    bool handleSubcondition(CTokenPtr start);

};

} // kdl


#endif // CONDITIONAL_CONDITIONAL_PARSER_H_