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

void kdl::ClRuleCompiler::writeNOfThem(int n)
{
	std::string prefix = "counter_" + std::to_string(n);
	m_fnbody << "\n\tint " << prefix << "_of_them = 0;\n";

	auto start = m_rule.cbegin();
	auto end = m_rule.cend();

	int index = 0;
	while (start != end)
	{
		m_fnbody << prefix << "_of_them += ";
		writeFunction(start->second);

		m_fnbody << ";\nif(" << prefix << "_of_them ==" << n << ")"
			     << "\n\t\tgoto __" << prefix << ";\n";


		++start;
	}

	m_fnbody << "__" << prefix << ":\n";

	m_return << "(" << prefix << "_of_them == " << n << ")";
}


///
/// ORDER OF OPERATOR DETECTION
///		1. OF
///		2. >=, <=, ==, !=, <, >
///		3. singlevar
/// 
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

	std::string varName(connector == '&' ? "all" : "any");
	varName += "_of_them";

	m_fnbody << "     bool " <<varName << ++numTimesCalled << " =\n\t ";

	while (start != end)
	{
		auto& s = start->second;
		writeFunction(s);
		++start;
		if (start != end)
			m_fnbody << "\n " << connector << connector << " ";
	}

	m_fnbody << ";\n";

	m_return << " " << varName << numTimesCalled << " ";
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

	m_return << target->value->val << "_detector" << numTimesCalled;
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

	else if (left->value->t == token_t::INTEGER)
	{
		int val = std::stoi(left->value->val);
		if (val == 1)
		{
			// treat '1 of them' as 'any of them'
			if(right->value->t == token_t::CND_THEM)
				writeMultiVariableConnectedCondition(right, '|');
			// treat '1 of $str*' as 'any of $str*'
			else if(right->value->t == token_t::MULTI_VAR_IDENTIFIER)
				writeMultiVariableConnectedCondition(right, '|');

		}
		else if (val == m_rule.getNumVariables())
		{
			// treat '1 of them' as 'any of them'
			if (right->value->t == token_t::CND_THEM)
				writeMultiVariableConnectedCondition(right, '&');
			// treat '1 of $str*' as 'any of $str*'
			else if (right->value->t == token_t::MULTI_VAR_IDENTIFIER)
				writeMultiVariableConnectedCondition(right, '&');
		}
		else
		{
			if (right->value->t == token_t::CND_THEM)
				writeNOfThem(val);
		}

	}

}
