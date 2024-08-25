#ifndef BACKEND_CLANG_CL_RULE_COMPILER_H_
#define BACKEND_CLANG_CL_RULE_COMPILER_H_

#include <sstream>
#include "channels/rule.h"
namespace kdl
{
///
/// PURPOSE
///		Compile a rule into a C function
///		
///		Will produce a function body, then add the return value
///		Will also construct a function prototype
/// 
class ClRuleCompiler
{
public:
    ClRuleCompiler(Rule& rule);

    std::stringstream& compile();

    std::string getFunctionPrototype();

private:

    Rule& m_rule;

    std::stringstream m_return;
    std::stringstream m_fnbody;

private:

    void writeHeader();

    void writeNOfThem(int n);

    void writeConditional(const CNode* head);

    ///
    /// PURPOSE
    ///		
    ///		Adds the functions for all variables and 
    ///     connects them together with 2x of the connecter (e.g. '&' -> '&&')
    /// 
    void writeMultiOperatorConnectedCondition(char connector);

    ///
    /// PURPOSE
    ///		
    ///		Adds the functions for all variables referenced by taand 
    ///     connects them together with 2x of the connecter (e.g. '&' -> '&&')
    /// 
    void writeMultiVariableConnectedCondition(const CNode* target, char connector);

    void writeFunction(const Variable& var);
    /// 
    /// PURPOSE
    ///		Turn something like $s into ldu_xyzsrch(whatever $s is)
    /// 
    void writeSingleMatchVariable(const std::string& name);
    void handleOfCondition(const CNode* left, const CNode* right);

    void writeLogicalOperator(const std::string& left, token_t lop, int num);
};


} // kdl


#endif // BACKEND_CLANG_CL_RULE_COMPILER_H_