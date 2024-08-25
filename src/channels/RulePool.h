#ifndef CHANNELS_RULE_POOL_H_
#define CHANNELS_RULE_POOL_H_

#include "template/Array.h"
#include "rule.h"

namespace kdl
{

///
/// PURPOSE
///		Hold a list of rules
/// 
/*class RulePool
{
    friend class ConditionalReferenceValidator;
public:

private:


};*/

using RulePool = kdl::Array<Rule>;

} // kdl


#endif // CHANNELS_RULE_POOL_H_