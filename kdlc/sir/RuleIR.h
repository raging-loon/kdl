#ifndef KDLC_SIR_RULE_IR_H_
#define KDLC_SIR_RULE_IR_H_

#include "ast/Nodes.h"

namespace kdl
{

///
/// @brief
///     Intermediate representation of an un-compiled rule
/// 
class RuleIR
{
public:
     
private:
    NodePtr<ASTBlock> m_predicate;
    NodePtr<ASTNode> m_condition;
    
};


} // kdl


#endif // KDLC_SIR_RULE_IR_H_