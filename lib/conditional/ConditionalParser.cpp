#include "conditional/ConditionalParser.h"
#include "parser/ErrorPrinter.h"
using kdl::ConditionalParser;
using kdl::ConditionalTree;
using kdl::Token;
using kdl::token_t;
using kdl::CTokenPtr;


static constexpr bool isComparisonOperator(token_t t)
{
	return t >= token_t::CND_OF && t <= token_t::KW_EQUALITY;
}

static constexpr bool isValidLeftRightValue(token_t t)
{
	return t >= token_t::STRING && t <= token_t::IDENTIFIER;
}

static constexpr bool isJunctionOperator(token_t t)
{
	return t >= token_t::KW_OR && t <= token_t::KW_AND_OP;
}



ConditionalParser::ConditionalParser(ConditionalTree& ctree, CTokenMapView& ctmv)
	: m_ctree(ctree), m_tokens(ctmv), m_numTokens(ctmv.size())
{
	m_cursor = 0;
}

bool kdl::ConditionalParser::parse()
{
	CTokenPtr start = advance();
	
	if (m_cursor == 1 && isJunctionOperator(start->t))
	{
		ErrorPrinter::printViaToken(
			ErrorPrinter::SYNTAX_ERROR,
			"Condition cannot start with 'and' or 'or'",
			start
		);
		return false;
	}


	if (isAtEnd()) 
		return true;

	
	// TODO: move these branches into another function
	CTokenPtr next = peek();

	// test for a subcondition
	if (isComparisonOperator(next->t))
	{

		advance(); 
		printf("Sub Condition Found: %s %s %s\n",
			kdl::getTokenName(start->t),
			kdl::getTokenName(previous()->t),
			kdl::getTokenName(peek()->t)
		);
		if (!m_ctree.addSubCondition(previous(), start, peek(), pLevel))
			return false;
		return parse();

	}

	else if (isJunctionOperator(next->t))
	{
		// If this is a junction the last token
		// throw an error
		if (m_cursor + 1 == m_numTokens)
		{
			ErrorPrinter::printViaToken(
				ErrorPrinter::SYNTAX_ERROR,
				"Condition cannot end with 'and' or 'or'",
				previous()
			);
			return false;
		}

		printf("Junction Found: %s\n", kdl::getTokenName(next->t));
		if (!m_ctree.addJunction(next, pLevel))
			return false;
		advance();

		return parse();
		
	}

	else if (next->t == token_t::CLOSE_PARENTHESIS)
	{
		if ((pLevel - 1) < 0)
			return false;
		m_ctree.merge(pLevel);
		pLevel--;

		return parse();
	}
	else if (start->t == token_t::OPEN_PARENTHESIS)
	{
		pLevel++;
		return parse();
	}
	
	else if (pLevel != 0)
	{

	}

	return true;
}

bool kdl::ConditionalParser::isAtEnd()
{
	return m_cursor >= m_numTokens;
}

CTokenPtr kdl::ConditionalParser::advance()
{
	return &m_tokens[m_cursor++];
}

CTokenPtr kdl::ConditionalParser::peek()
{
	if (isAtEnd()) return nullptr;
	return &m_tokens[m_cursor];
}

CTokenPtr kdl::ConditionalParser::previous()
{
	if (m_cursor == 0) return nullptr;
	return &m_tokens[m_cursor - 1];
}

bool kdl::ConditionalParser::check(token_t t)
{
	if (isAtEnd()) return false;
	return peek()->t == t;
}
