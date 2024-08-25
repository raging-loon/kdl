#ifndef CONDITIONAL_CONDITIONAL_REFERENCE_VALIDATOR_H_
#define CONDITIONAL_CONDITIONAL_REFERENCE_VALIDATOR_H_

#include "channels/RulePool.h"
#include <span>
namespace kdl
{

class ConditionalReferenceValidator
{
public:
    ConditionalReferenceValidator(RulePool& rp);

    bool check();

private:

    bool validateRule(Rule& r);
    bool validateIdentifier(const CNode* const n, Rule& r);
    RulePool& m_pool;

};


} // kdl


#endif // CONDITIONAL_CONDITIONAL_REFERENCE_VALIDATOR_H_