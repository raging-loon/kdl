#ifndef PARSER_TOKENS_H_
#define PARSER_TOKENS_H_

#include <unordered_map>
#include <string>

namespace kdl
{


enum class token_t : int
{
	// operators
	OPEN_PARENTHESIS,
	CLOSE_PARENTHESIS,
	OPEN_BRACE,
	CLOSE_BRACE,
	COLON,
	ASSIGNMENT,
	STAR,
	DOT,


	// section indicators
	SI_META,
	SI_CONTENT,
	SI_RULES,
	SI_CONDITIONS,

	// Type indicators
	
	
	// e.g. calling another rule.
	// This is used to identify a rule
	// SI_RULES is used to classify a block
	// as a rule
	TI_RULE,	 

	TI_POLICY,
	TI_VARIABLE,
	PROCESS_TYPE_NAME,

	// Literals
	STRING,
	INTEGER,
	REGEXP,
	BYTE_SEQ,
	BYTE_SEQ_COND,
	SIZE,
	IDENTIFIER,

	// Modifiers
	
	MOD_WIDE,
	MOD_ASCII,
	MOD_NOCASE,

	// Conditions

	CND_THEM,
	CND_ALL,
	CND_ANY,
	CND_AT, 

	// logical operators
	KW_OR,		// 'or'
	KW_AND,		// 'and'
	KW_OR_OP,	// '&&'
	KW_AND_OP,	// '||'

	CND_OF,
	KW_GT,
	KW_GEQ,
	KW_LT,
	KW_LEQ,
	KW_EQUALITY,
	TEOF
};

struct Token
{
	token_t t;
	/// Value. TODO: figure out if this can be replaced
	std::string val;

	/// Position in the source of the start of 
	/// the associated character/string 
	int startPos;

	/// the position of the line this token is 
	int lineStart;

	/// what line is this token on?
	int lineNumber;
};


#ifdef _DEBUG

const char* getTokenName(token_t t);

#endif // _DEBUG

} // kdl

#endif // PARSER_TOKENS_H_