#include "conditional/ConditionalReferenceValidator.h"

using kdl::ConditionalReferenceValidator;
using kdl::Rule;


ConditionalReferenceValidator::ConditionalReferenceValidator(RulePool& rp)
	: m_pool(rp)
{
}

bool ConditionalReferenceValidator::check()
{
	
	for (int i = 0; i < m_pool.m_rules.size(); i++)
	{
		if (!validateRule(m_pool.m_rules[i]))
			return false;
	}

	return true;
}

bool ConditionalReferenceValidator::validateRule(Rule& r)
{
	printf("%p\n", r);

	auto* n = r.m_conditions.getHead();
	return validateIdentifier(n, r);
}

bool kdl::ConditionalReferenceValidator::validateIdentifier(const CNode* const n, Rule& r)
{
	if (!n || !n->value)
		return true;

	if (n->value->t == token_t::IDENTIFIER)// || n.value->t == token_t::MULTI_VAR_IDENTIFIER)
	{
		auto& name = n->value->val;
		if (r.m_variables.find(name) == r.m_variables.end())
			return false;
	}

	if (!n->left || !n->right)
		return true;

	if (!validateIdentifier(n->left, r))
		return false;
	if (!validateIdentifier(n->right, r))
		return false;
	return false;
}


