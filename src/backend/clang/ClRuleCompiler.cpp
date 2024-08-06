#include "backend/clang/ClRuleCompiler.h"
#include "channels/rule.h"
#include "cli/KdlOptions.h"
using kdl::ClRuleCompiler;
using kdl::Rule;
using kdl::CNode;
using kdl::token_t;

constexpr bool matchLeftRight(const CNode* left, const CNode* right, token_t el, token_t er)
{
	return left->value->t == el && 
		   right->value->t == er;
}

ClRuleCompiler::ClRuleCompiler(Rule& rule)
	: m_rule(rule)
{
}

std::stringstream& ClRuleCompiler::compile()
{
	writeHeader();

	m_fnbody << "\n    return " << m_return.str() << ";\n}\n\n\n";
	
	return m_fnbody;
}

std::string kdl::ClRuleCompiler::getFunctionPrototype()
{
	return "bool kdl_rule_" + m_rule.getName() + 
		   "(const unsigned char*, unsigned int);\n";
}

void ClRuleCompiler::writeHeader()
{
	m_fnbody << "bool kdl_rule_" << m_rule.getName()
		     << "(const unsigned char* data, unsigned int len)\n{\n";
	
	writeConditional(m_rule.m_conditions.getHead());

}

void ClRuleCompiler::writeConditional(const CNode* head)
{
	if (head)
	{

		token_t ht = head->value->t;

		if (ht == token_t::CND_OF)
		{
			handleOfCondition(head->left, head->right);
			return;
		}
		
		if (ht == token_t::SINGLE_VAR_IDENTIFIER)
		{
			writeSingleMatchVariable(head->value->val);
			return;
		}


		if(head->left && head->right)
		{ 
			m_return << " ( ";

			writeConditional(head->left);
			if (ht == token_t::KW_AND)
				m_return << " && ";
			else if(ht == token_t::KW_OR)
				m_return << " || ";
			writeConditional(head->right);
			m_return << " ) ";

		}

	}
}


void kdl::ClRuleCompiler::writeMultiOperatorConnectedCondition(char connector)
{
	assert(connector == '&' || connector == '|');
	static int numTimesCalled = 0;
	
	auto start = m_rule.cbegin();
	auto end = m_rule.cend();

	m_fnbody << "     bool " << (connector == '&' ? "all" : "any") << "_of_them" << ++numTimesCalled << " =\n\t ";

	while (start != end)
	{
		auto& s = start->second;
		writeFunction(s);
		++start;
		if (start != end)
			m_fnbody << "\n " << connector << connector << " ";
	}

	m_fnbody << ";\n";

	m_return << " _any_of_them ";
}

void kdl::ClRuleCompiler::writeMultiVariableConnectedCondition(const CNode* target, char connector)
{
	assert(connector == '&' || connector == '|');
	static int numTimesCalled = 0;

	std::vector<const Variable*> mvs;
	m_rule.searchVariableName(target->value->val, mvs);

	m_fnbody << "\n     bool " << target->value->val << "_detector" << ++numTimesCalled << " = \n    ";

	for (int i = 0; i < mvs.size(); i++)
	{
		if (i != 0)
			m_fnbody << "\n " << connector << connector << " ";

		writeFunction(*mvs[i]);
	}

	m_fnbody << ";\n";

	m_return << target->value->val << "_detector";
}

void ClRuleCompiler::writeFunction(const Variable& var)
{
	switch (var.type)
	{
		case Variable::BYTE_SEQUENCE:
		case Variable::STRING:
			m_fnbody << " ldu_bytesrch(\"" << var.searchstr << "\", data, " << var.searchstr.length() << ", len) ";
			break;
			//m_fnbody << " ldu_strsrch(\"" << var.searchstr << "\", data, " << var.searchstr.length() << ") ";
			//break;
	}
}

void ClRuleCompiler::writeSingleMatchVariable(const std::string& name)
{
	m_fnbody << "\n     bool _try_match_" << name << " = \n     ";
	writeFunction(m_rule.getVariable(name));
	m_fnbody << ";\n";

	m_return << " _try_match_" << name << " ";

}

void ClRuleCompiler::handleOfCondition(const CNode* left, const CNode* right)
{
	if (!left || !right)
		return;
	if (matchLeftRight(left, right, token_t::CND_ANY, token_t::CND_THEM))
		writeMultiOperatorConnectedCondition('|');

	else if (matchLeftRight(left, right, token_t::CND_ANY, token_t::MULTI_VAR_IDENTIFIER))
		writeMultiVariableConnectedCondition(right, '|');

	else if (matchLeftRight(left, right, token_t::CND_ALL, token_t::MULTI_VAR_IDENTIFIER))
		writeMultiVariableConnectedCondition(right, '&');

	else if (matchLeftRight(left, right, token_t::CND_ALL, token_t::CND_THEM))
		writeMultiOperatorConnectedCondition('&');

}
