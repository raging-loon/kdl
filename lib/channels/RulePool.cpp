#include "channels/RulePool.h"

using kdl::RulePool;
using kdl::Rule;

Rule& RulePool::allocateNewRule()
{
	m_rules.push_back({  });
	return m_rules.back();
}