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
	friend class ConditionalReferenceValidator;
public:

private:
	
};


} // kdl


#endif // CHANNELS_RULE_POOL_H_