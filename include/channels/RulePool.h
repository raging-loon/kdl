#ifndef CHANNELS_RULE_POOL_H_
#define CHANNELS_RULE_POOL_H_

#include <vector>

#include "rule.h"

namespace kdl
{

///
/// PURPOSE
///		Hold a list of rules
///		Handles allocation of rules so they are organized
///		in a cache friendly manner. This way they can be
///		iterated over quickly (via getIterator/getIteratorEnd)
/// 
class RulePool
{

public:

	using RuleList = std::vector<Rule>;
	using CRuleListIter = std::vector<Rule>::const_iterator;

	RulePool() : m_rules({}) {}

	Rule& allocateNewRule()
	{
		m_rules.push_back({});
		return m_rules.back();
	}

	inline CRuleListIter getIterator() { return m_rules.cbegin(); }
	inline CRuleListIter getIteratorEnd() { return m_rules.cend(); }

private:
	RuleList m_rules;


};


} // kdl


#endif // CHANNELS_RULE_POOL_H_