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

private:

	Rule& m_rule;

	std::stringstream m_return;
	std::stringstream m_fnbody;

private:

	void writeHeader();
	
	void writeAllOfX();
	void writeNumOfX();


	void writeAnyOfThem();
	void writeAnyOfMultiVar(const std::string& prefix);
	void writeConditional(const CNode* head);

	void writeFunction(const Variable& var);
	/// 
	/// PURPOSE
	///		Turn something like $s into ldu_xyzsrch(whatever $s is)
	/// 
	void writeSingleMatchVariable(const std::string& name);
	void handleOfCondition(const CNode* left, const CNode* right);

};


} // kdl


#endif // BACKEND_CLANG_CL_RULE_COMPILER_H_