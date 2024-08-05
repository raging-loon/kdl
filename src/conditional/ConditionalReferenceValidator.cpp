#include "conditional/ConditionalReferenceValidator.h"
#include "parser/CompilerMessage.h"
using kdl::ConditionalReferenceValidator;
using kdl::Rule;


ConditionalReferenceValidator::ConditionalReferenceValidator(RulePool& rp)
	: m_pool(rp)
{
}

bool ConditionalReferenceValidator::check()
{
	
	for (int i = 0; i < m_pool.size(); i++)
	{
		if (!validateRule(*m_pool[i]))
			return false;
	}

	return true;
}

bool ConditionalReferenceValidator::validateRule(Rule& r)
{
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
		{
			CompilerMessage::error(
				message_class_t::UNKNOWN_VARIABLE,
				n->value,
				"%s Not found in rule %s",
				name.c_str(), r.m_name.c_str()
			);
			return false;
		}
	}

	else if (n->value->t == token_t::MULTI_VAR_IDENTIFIER)
	{
		auto& prefix = n->value->val;

		for (const auto& [rname, rvar] : r.m_variables)
		{
			if (rname.starts_with(prefix.c_str()))
				return true;
		}

		CompilerMessage::error(
			message_class_t::UNKNOWN_ERROR,
			n->value,
			"$%s* does not match any variables found in %s",
			prefix.c_str(), r.m_name.c_str()
		);

		return false;

	}

	if (!n->left || !n->right)
		return true;

	if (!validateIdentifier(n->left, r))
		return false;
	if (!validateIdentifier(n->right, r))
		return false;
	return true;
}


