#ifndef PARSER_RULE_PARSER_H_
#define PARSER_RULE_PARSER_H_

#include "parser/Combinator.h"
#include "parser/parser.h"
#include "channels/rule.h"

namespace kdl
{


class RuleParser : private Parser
{
public:
	
	RuleParser(CTokenMapView& tokenMap, Rule& newRule);
	bool parse() override;

private:
	/// Parser-overriden functions
	inline CTokenPtr advance() override;
	
	inline bool isAtEnd() override;

	CTokenPtr peek() override;

	CTokenPtr previous() override;
	
	bool check(token_t t) override;

private:
	/// 
	/// PURPOSE
	///		Retrieves header information
	///		Including the rule name and if found,
	///		a parameter like "File" or "Net"
	///		
	bool parseHeader();
	
	///
	/// PURPOSE
	///		Initiate parsing of other sections e.g. "conditions:", "meta:"
	///		
	bool parseSection();

	///
	/// PURPOSE
	///		Pick out and format variables in the "content:" section
	/// 
	bool parseVariables();

	///
	/// PURPOSE
	///		Scan for modifications after the TI_* token has been found
	///		Will OR them together based on flags present in 'channels/type.h'
	///
	uint8_t scanVariableMods();


	///
	/// PURPOSE
	///		Parse the conditionals section
	///		Will produce a conditional tree
	///		
	///		Note: parseConditional expcets the current token to be 
	///			  The first token in the conditional sequence
	///
	bool parseConditionals();

private:

	/// PURPOSE: The rule we are operating upon
	Rule& m_rule;

	/// Span of tokens
	CTokenMapView& m_tokenMapView;

	int m_numTokens;
	
	enum State
	{
		NO_SECTION,
		SECTION_META,
		SECTION_CONDITION,
		SECTION_CONTENT
	};

	State m_currentState;


};

} // kdl

#endif // PARSER_RULE_PARSER_H_