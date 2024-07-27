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

};

} // kdl


#endif // CONDITIONAL_CONDITIONAL_PARSER_H_