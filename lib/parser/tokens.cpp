#include "parser/tokens.h"

using namespace kdl;

static std::unordered_map<token_t, const char*> tokenNames = {
	{token_t::OPEN_PARENTHESIS, "OPEN_PARENTHESIS"},
	{token_t::CLOSE_PARENTHESIS, "CLOSE_PARENTHESIS"},
	{token_t::OPEN_BRACE, "OPEN_BRACE"},
	{token_t::CLOSE_BRACE, "CLOSE_BRACE"},
	{token_t::COLON, "COLON"},
	{token_t::ASSIGNMENT, "ASSIGNMENT"},
	{token_t::STAR, "STAR"},
	{token_t::DOT, "DOT"},
	{token_t::KW_GT, "KW_GT"},
	{token_t::KW_GEQ, "KW_GEQ"},
	{token_t::KW_LT, "KW_LT"},
	{token_t::KW_LEQ, "KW_LEQ"},
	{token_t::KW_EQUALITY, "KW_EQUALITY"},
	{token_t::KW_OR, "KW_OR"},
	{token_t::KW_AND, "KW_AND"},
	{token_t::KW_OR_OP, "KW_OR_OP"},
	{token_t::KW_AND_OP, "KW_AND_OP"},
	{token_t::SI_META, "SI_META"},
	{token_t::SI_CONTENT, "SI_CONTENT"},
	{token_t::SI_RULES, "SI_RULES"},
	{token_t::SI_CONDITIONS, "SI_CONDITIONS"},
	{token_t::TI_RULE, "TI_RULE"},
	{token_t::TI_POLICY, "TI_POLICY"},
	{token_t::TI_VARIABLE, "TI_VARIABLE"},
	{token_t::PROCESS_TYPE_NAME, "PROCESS_TYPE_NAME"},
	{token_t::STRING, "STRING"},
	{token_t::INTEGER, "INTEGER"},
	{token_t::REGEXP, "REGEXP"},
	{token_t::BYTE_SEQ, "BYTE_SEQ"},
	{token_t::BYTE_SEQ_COND, "BYTE_SEQ_COND"},
	{token_t::SIZE, "SIZE"},
	{token_t::IDENTIFIER, "IDENTIFIER"},
	{token_t::MOD_WIDE, "MOD_WIDE"},
	{token_t::MOD_ASCII, "MOD_ASCII"},
	{token_t::MOD_NOCASE, "MOD_NOCASE"},
	{token_t::CND_OF, "CND_OF"},
	{token_t::CND_THEM, "CND_THEM"},
	{token_t::CND_ANY, "CND_ANY"},
	{token_t::CND_ALL, "CND_ALL"},
	{token_t::CND_AT, "CND_AT"},
	{token_t::TEOF, "TEOF"}
};

const char* kdl::getTokenName(token_t t)
{
	auto f = tokenNames.find(t);
	if (f != tokenNames.end())
		return f->second;
	return "UNKNOWN_TOKEN";
}
