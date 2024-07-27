#include "parser/RuleParser.h"
#include "conditional/ConditionalParser.h"
#include "conditional/ConditionalTree.h"

using kdl::RuleParser;
using kdl::CTokenMapIter;
using kdl::token_t;
using kdl::Token;
using kdl::CTokenPtr;


constexpr static bool nextTokenIsSection(token_t t)
{
	return (t >= token_t::SI_META && t <= token_t::SI_CONDITIONS);
}

RuleParser::RuleParser(CTokenMapView& tokenView, Rule& newRule)
	: m_rule(newRule),
	  m_tokenMapView(tokenView), 
	  m_numTokens(tokenView.size()),
	  m_currentState(NO_SECTION)
{
	m_cursor = 0;
}

bool RuleParser::parse()
{
	if(!parseHeader())
		return false;

	while (!isAtEnd())
	{
		if (!parseSection()) return false;
	}

	return true;
}

CTokenPtr RuleParser::advance()
{
	return &m_tokenMapView[m_cursor++];
}

inline bool RuleParser::isAtEnd()
{
	return m_cursor >= m_numTokens;
}

CTokenPtr RuleParser::peek()
{
	if (isAtEnd()) return nullptr;
	return &m_tokenMapView[m_cursor];
}

CTokenPtr RuleParser::previous()
{
	if (m_cursor == 0) return nullptr;

	return &m_tokenMapView[m_cursor - 1];
}

bool RuleParser::check(token_t t)
{
	if (isAtEnd()) return false;
	return peek()->t == t;
}

bool RuleParser::parseHeader()
{
	if (matchInOrder({ token_t::SI_RULES, token_t::IDENTIFIER }))
	{
		printf("Found new rule: %s\n", m_tokenMapView[1].val.c_str());
		
		m_rule.setName(m_tokenMapView[1].val.c_str());

		if (peek()->t == token_t::COLON)
		{
			advance();
			if (peek()->t != token_t::IDENTIFIER)
			{
				printf("error\n");
				return false;
			}
			else
			{
				printf("Paramter found: %s\n", peek()->val.c_str());
				
				m_rule.setRuleParameters(peek()->val);
			}
			// get past the param id
			advance();
		}

		else if (peek()->t == token_t::OPEN_BRACE)
			printf("Rule has no paramters\n");
		
		// get past the open brace
		advance();

		return true;
	}

	return false;

}

bool RuleParser::parseSection()
{
	auto* next = peek();
	if (nextTokenIsSection(next->t))
	{
		advance();
		if (peek()->t != token_t::COLON)
		{
			printf("error\n");
			return false;
		}

		advance();
	
		switch (next->t)
		{
			case token_t::SI_CONTENT:
				return parseVariables();
			case token_t::SI_CONDITIONS:
				return parseConditionals();
			default:
				abort();
		}
	}

	return false;

}

bool RuleParser::parseVariables()
{
	printf("Parsing variables\n");

	while (!(nextTokenIsSection(peek()->t)))
	{
		if ( !( matchInOrder( { token_t::TI_VARIABLE, token_t::IDENTIFIER, token_t::ASSIGNMENT } ) ) )
		{
			printf("invalid\n");
			return false;
		}
		Variable::Type tType;


		switch (peek()->t)
		{
			case token_t::STRING:
				tType = Variable::STRING;
				break;
			case token_t::REGEXP:
				tType = Variable::REGEX;
				break;
			case token_t::BYTE_SEQ:
				tType = Variable::BYTE_SEQUENCE;
				break;
			default:
				printf("Invalid expression\n");
				return false;
		}


		const std::string& name = m_tokenMapView[m_cursor - 2].val;
		const std::string& value = m_tokenMapView[m_cursor].val;

			
		advance();

		auto flags = scanVariableMods();

		m_rule.addRule(name, value, tType, flags);
		
		
		
		printf("Found variable: %s with '%s' as value\n", 
			name.c_str(), 
			value.c_str());
	
	}

	return true;
}

uint8_t RuleParser::scanVariableMods()
{
	CTokenPtr next = nullptr;
	uint8_t flags = 0;
	while (next = peek())
	{
		if (next->t >= token_t::MOD_WIDE && next->t <= token_t::MOD_NOCASE)
		{
			// Match RMOD_*
			// e.g. if next-> == token_t::MOD_ASCII,
			//		then RMOD_ASCII == 1 << (token_t::MOD_ASCII - token_t::MOD_WIDE));
			//			  (1 << 1)  ==		(1 << (33 - 32))
			flags |= 1 << (((int)next->t - (int)token_t::MOD_WIDE));
			advance();
		}
		else
			break;
	}
	
	return flags;
}

bool RuleParser::parseConditionals()
{
	printf("parsing conditionals\n");

	int start = m_cursor;
	int end = m_cursor;
	
	CTokenPtr curTok;

	while ((curTok = peek()))
	{
		if (nextTokenIsSection(curTok->t) || curTok->t == token_t::CLOSE_BRACE)
		{
			break;
		}
		end++;
		advance();
	}


	CTokenMapView map(m_tokenMapView.data() + start, (end - start));
	ConditionalTree ctree{};

	ConditionalParser p(ctree, map);
	if (!p.parse())
	{
		printf("There was an error\n");
	}
	ctree.dumpTree();

	return false;
}

