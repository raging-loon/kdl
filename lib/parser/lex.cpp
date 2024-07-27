#include "parser/lex.h"
#include "utils.h"

#include "parser/ErrorPrinter.h"

using namespace kdl;

KeywordMap Lexer::m_keywords =
{
	{"rule", token_t::SI_RULES},
	{"meta", token_t::SI_META},
	{"condition", token_t::SI_CONDITIONS},
	{"content", token_t::SI_CONTENT},
	{"and", token_t::KW_AND},
	{"or", token_t::KW_OR},
	{"nocase", token_t::MOD_NOCASE},
	{"ascii", token_t::MOD_ASCII},
	{"wide", token_t::MOD_WIDE},
	{"of", token_t::CND_OF},
	{"them", token_t::CND_THEM},
	{"any", token_t::CND_ANY},
	{"all", token_t::CND_ALL},
	{"at", token_t::CND_AT},
};


Lexer::Lexer(const char* filename, const char* source, int len)
	: m_filename(filename), m_source(source), m_srcLen(len), m_tokens({})
{
}

int Lexer::scan()
{
	while (!atEnd())
	{
		if (m_wasError)
			return -1;

		m_start = m_current;
		scanToken();
	}

	return m_tokens.size();
}


void Lexer::scanToken()
{
	char c = advance();

	switch (c)
	{
		case '(': addToken(token_t::OPEN_PARENTHESIS); break;
		case ')': addToken(token_t::CLOSE_PARENTHESIS); break;
		case '{': addToken(token_t::OPEN_BRACE); break;
		case '}': addToken(token_t::CLOSE_BRACE); break;
		case ':': addToken(token_t::COLON); break;
		case '$': addToken(token_t::TI_VARIABLE); break;
		case '=':
		{
			// test for ==
			match('=') ?
				addToken(token_t::KW_EQUALITY) : 
				addToken(token_t::ASSIGNMENT);
			break;
		}
		case '<':
		{
			// test for <=
			match('=') ?
				addToken(token_t::KW_LEQ) :
				addToken(token_t::KW_LT);
			break;
		}
		case '>':
		{
			// test for >=
			match('=') ?
				addToken(token_t::KW_GEQ) :
				addToken(token_t::KW_GT);
			break;
		}
		case '*': addToken(token_t::STAR); break;
		
		case ' ':
		case '\t':
		case '\0':
			break;
		case '\n': 
			m_curLine++; 
			m_lineStart = m_current + 1;
			break;
		
		case '"': scanString();		break;
		case '|': scanByteSearch(); break;
		case '/': scanRegex();		break;
		default:

			if (utils::isNumeric(c))
				scanInteger();
			else if (utils::isAlpha(c))
				scanIdentifierOrKeyword();
			else
				showError("Unknown token");
	}
}
char Lexer::peek()
{

	if (atEnd()) return '\0';
	
	return m_source[m_current];
}

bool Lexer::match(char n)
{
	if (atEnd()) return false;

	if (m_source[m_current] != n) return false;

	m_current++;
	return true;
}

void Lexer::addToken(token_t tok)
{
	m_tokens.push_back({
		.t = tok, 
		.startPos = m_start,
		.lineStart = m_lineStart,
		.lineNumber = m_curLine

	});
}

void Lexer::addToken(token_t tok, std::string& val)
{
	m_tokens.push_back({ 
		.t = tok, 
		.val = val,
		.startPos = m_start,
		.lineStart = m_lineStart,
		.lineNumber = m_curLine
	});
}

void Lexer::addToken(token_t tok, int startPos, int endPos)
{
	m_tokens.push_back({
		.t = tok,
		.val = std::string(m_source + startPos, m_source + endPos),
		.startPos = m_start,
		.lineStart = m_lineStart,
		.lineNumber = m_curLine


	});
}

void Lexer::scanIdentifierOrKeyword()
{
	while (utils::isAlphaNumeric(peek())) advance();
	
	std::string test((m_source + m_start), (m_source + m_current));
	
	//printf("%s s:%d e:%d\n", test.c_str(), m_start, m_current);
	auto posKw = m_keywords.find(test);
	if (posKw != m_keywords.end())
	{
		addToken(
			posKw->second
		);
	}
	else
	{
		addToken(
			token_t::IDENTIFIER,
			test
		);
	}
}

void Lexer::scanInteger()
{
	while (utils::isNumeric(peek())) advance();

	if (utils::isAlpha(peek()))
		scanSize();
	else
		addToken(token_t::INTEGER, m_start, m_current);
}

void Lexer::scanSize()
{
	char n = peek();

	if (n == 'B' || n == 'b')
	{
		advance();
		addToken(token_t::SIZE, m_start, m_current);
		return; 
	}

	if (n == 'M' || n == 'm' || n == 'K' ||
		n == 'k' || n == 'g' || n == 'G')
	{
		advance(); 
		char sb = peek();
		if (sb == 'B' || sb == 'b')
		{
			advance();
			addToken(token_t::SIZE, m_start, m_current);
		}
		else
		{
			showError("Invalid Size indicator. Must end with 'B' or 'b'");
		}
	}
	else
	{
		showError("Invalid Size indicator. Must start with one of these characters: 'KkMmGg'");
	}
	
}


void Lexer::scanString()
{
	scanSection('"');
	addToken(token_t::STRING, m_start + 1, m_current - 1);
}

void kdl::Lexer::scanRegex()
{
	scanSection('/');
	// we want to keep the '/''s. 
	// They are necessary for processing
	addToken(token_t::REGEXP, m_start, m_current );
}

void kdl::Lexer::scanByteSearch()
{
	scanSection('|');

	int substringStart = m_start + 1;
	int substringEnd = m_current ;

	// Strip leading and ending white-spaces
	for (int i = substringStart; i < substringEnd; i++)
	{
		if (m_source[i] != ' ') break;
		substringStart++;
	}
	for (int i = substringEnd-1; i > substringStart; --i)
	{
		if (m_source[i] != ' ' && m_source[i] != '|') break;
		substringEnd--;
	}
	
	addToken(token_t::BYTE_SEQ, substringStart, substringEnd);
	
	int possibleErrorOffset = formatByteSequence(m_tokens.back().val);

	if ( possibleErrorOffset != -1)
	{
		m_start += possibleErrorOffset;
		showError("Invalid hexadecimal");

	}
}

void Lexer::scanSection(char target)
{
	while (peek() != target) advance();

	while ((peek() != target) && !atEnd())
		advance();
	
	if (atEnd())
		assert(false);

	advance();

}

void Lexer::showError(const char* message)
{
	m_wasError = true;

	ErrorPrinter::print(
		ErrorPrinter::UNKNOWN_TOKEN,
		"Unknown token/operator/identifier",
		m_curLine, m_lineStart, m_start
	);
	
}

bool Lexer::validateRegex()
{
	return false;
}

int Lexer::formatByteSequence(std::string& input)
{
	int charsParsed = 0;
	int position = 0;

	char* formatted = new char[input.size()];
	memset(formatted, 0, input.size());
	for (int i = 0; i < input.size(); i++)
	{
		if (input[i] != ' ')
		{
			int num = utils::hexToInt(input[i]);

			if (num == -1)
				return i;
			
			if (charsParsed % 2 == 0)
			{
				// set the upper nibble
				formatted[position] = (num << 4);
			}
			else
			{
				// set the lower nibble
				formatted[position] |= num;
				++position;
			}

			++charsParsed;
		}
	}

	input.clear();
	input.assign(formatted, charsParsed);
	delete[] formatted;

	return -1;
}




#ifdef _DEBUG

void Lexer::dumpTokens()
{

	for(auto& tok : m_tokens)
	{

		printf("[%s]", getTokenName(tok.t));
		if (!tok.val.empty())
			printf(" %s",tok.val.c_str());
		putchar('\n');
		
	}
}

#endif // _DEBUG