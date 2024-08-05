#ifndef PARSER_COMBINATOR_H_
#define PARSER_COMBINATOR_H_

#include "lex.h"
#include "parser.h"
#include <cstdarg>
#include <span>
namespace kdl
{

///
/// PURPOSE
///		This parser simply calls other parsers (Rule/PolicyParser)
///		It will scan over the tokens to find blocks
///		These blocks are identified by a header:
///			e.g. "rule RuleName"
///		The block is obtained by searching for open and closing 
///		braces
/// 
///		An std::span is then constructed from the block and given to 
///		the other parsers
/// 
class Combinator : private Parser 
{
public:
	explicit Combinator(const TokenList& t);

	bool parse() override;
	// TODO: replace me with DI
	RuleBlockList& getBlockList()
	{
		return m_ruleBlocks;
	}

	void dumpBlocks();
private:
	/// List of tokens generated by Lexer
	const TokenList& m_tokenList;

	int m_numTokens;
	// TODO: see if DI would work better here
	RuleBlockList m_ruleBlocks;
	// CTokenMapView m_policyBlocks;


	/// Parser-overriden functions
	inline bool isAtEnd() override;

	inline const Token* advance() override;
	
	const Token* peek() override;

	const Token* previous() override;

	bool check(token_t t) override;


	/// 
	/// PURPOSE
	///		Return the index of the next
	///		CLOSE_BRACE token
	///		Returns -1 if none are found
	///		Or if OPEN_BRACE is found at all
	///
	int getBlockEndIndex();

	///
	///	PURPOSE
	///		Find the next rule/policy to parse
	///		Will run the appropriate parser 
	///		and set the cursor the preceding token
	///
	///	RETURNS
	///		False if the rule/policy parser failed
	///
	bool nextBlock();

};


} // kdl


#endif // PARSER_COMBINATOR_H_