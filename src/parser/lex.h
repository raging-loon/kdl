#ifndef PARSER_LEX_H_
#define PARSER_LEX_H_

#include <vector>
#include <unordered_map>
#include <cassert>
#include <string>
#include <regex>
#include "tokens.h"

namespace kdl
{

using KeywordMap = std::unordered_map<std::string, token_t>;
using TokenList  = std::vector<Token>;

///
/// PURPOSE
///		Given a string, pick out tokens and store them in a vector.
/// 
///		Data Transformations:
///			1. Any byte sequence (.e.g | de ad be ef |) will be transformed
///			   into an actual byte sequence (e.g. '\xde\xad\xbe\xef')
///			2. Multivariables (e.g. $pdb*) will ignore the star at then end.
///				$pdb* will not be read as VARIABLE IDENTIFIER STAR
///				It will be read as VARIABLE MULTI_VAR_IDENTIFIER
///			3. Any variables with parenthesis will have the parenthesis removed
///				($malfn) => $malfn, ($str*) => $str*
/// 
class Lexer
{
public:
	Lexer(const char* src, int len);

	///
	/// PURPOSE
	///		Scanning loop
	///		Will continutally call scanToken
	///		until there are none left
	/// 
	int scan();

	const TokenList& getTokens() const { return m_tokens; }
	
	void dumpTokens();

private:
	const char* m_source{ nullptr };
	int			m_srcLen{ -1 };

	TokenList m_tokens;
	
	// TODO: figure out if this can be moved elsewhere
	static KeywordMap m_keywords;
	
	/// Hold the current position in m_source
	int m_current{ 0 };
	/// Is reset every token. Used for substrings
	int m_start{ 0 };

	/// Keep track of the current line. Used for errors
	int m_curLine{ 0 };
	/// Used for printing out the line that caused an error
	int m_lineStart{ 0 };


	/// Used by `scan` to break and return earlys
	bool m_wasError{ false };
private:

	///
	/// PURPOSE
	///		Scan possible tokens into the token list
	/// 
	void scanToken();

	inline char advance()
	{
		assert(m_source);
		return m_source[m_current++];
	}

	inline bool atEnd() const
	{
		return m_current >= m_srcLen;
	}

	///
	/// PURPOSE 
	///		Return m_current + 1
	///		Returns '\0' if it reaches the end
	/// 
	char peek();
	
	///
	/// PURPOSE
	///		Is the next character n?
	///		If so advance the cursor
	/// 
	bool match(char n);

	///
	char previous();

	/// 
	/// PURPOSE
	///		Add a token that does not reference text
	///		e.g. 'or', '{', '}', 'rule'
	/// 
	void addToken(token_t tok);

	/// 
	/// PURPOSE
	///		Add a token that does reference text
	///		e.g. '$identifier', '"this is a string"', '/this is a regex/'
	/// 
	void addToken(token_t tok, std::string& val);

	///
	/// PURPOSE
	///		Add a token and associated with a substring.
	///		startPos and endPos should reference `m_source`
	/// 
	void addToken(token_t tok, int startPos, int endPos);


	/// 
	/// PURPOSE
	///		Scan in an identifier
	///		If it is a valid keyword, add that keyword to the token list
	///		Otherwise add it as an identifier
	/// 
	void scanIdentifierOrKeyword();

	///
	/// PURPOSE
	///		Scan an integer like 123
	/// 
	void scanInteger();

	///
	/// PURPOSE
	///		Scan a size like '10MB' or '300KB'
	/// 
	void scanSize();
	
	/// 
	/// PURPOSE 
	///		Scan anything that starts and ends with '"'
	///
	void scanString();

	/// 
	/// PURPOSE 
	///		Scan anything that starts and ends with '/'
	/// 
	void scanRegex();
	
	/// 
	/// PURPOSE 
	///		Scan anything that starts and ends with '|'
	/// 
	void scanByteSearch();

	/// 
	/// PURPOSE
	///		Scan a section of text from target, to target
	///		e.g. if target is '/' and there is a string '/test/'
	///		This will adjust m_current to point to the end of that 
	///		string
	/// 
	///		The calling code is responsible for using m_start and m_current
	/// 
	///		Will ignore any 'target' preceded by '\'
	/// 
	void scanSection(char target);


	void showError(const char* message);


	bool validateRegex();

	/// 
	/// PURPOSE
	///		Format a byte sequence.
	///		e.g. | 00 01 ff 4f ee | => "\x00\x01\xff\x4f\xee"
	/// 
	/// RETURNS
	///		-1 on success or position of failure
	/// 
	int formatByteSequence(std::string& input);
};


} // kdl

#endif // PARSER_LEX_H_