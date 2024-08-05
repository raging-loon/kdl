#ifndef CHANNEL_RULE_H_
#define CHANNEL_RULE_H_

#include <string>
#include <unordered_map>
#include "type.h"
#include "conditional/ConditionalTree.h"

namespace kdl
{

enum class RuleParameters : uint16_t
{
	NONE,
	NET,
	FILE,
	PROCESS

};

///
/// PURPOSE
///		Hold information about a single rule
///			1. Name
///			2. Parameters
///			3. Variables
///			4. Condition Tree
///			5. Meta info
/// 
class Rule
{
	friend class ConditionalReferenceValidator;

public:
	Rule();
	~Rule() {}

public:

	using CVariableIter = std::unordered_map<std::string, Variable>::const_iterator;


	/// Getters/Setters

	inline void setName(const std::string& newName) { m_name = newName; }

	inline const std::string& getName() const { return m_name; }

	inline void setRuleParameters(RuleParameters rp) { m_params = rp; };


	/// 
	/// PURPOSE
	///		Parse 'rp' and set the rule parameter 
	///		Returns false if the rule parameter is invalid.
	/// 
	bool setRuleParameters(const std::string& rp);
	
	/// Returns a copy
	inline RuleParameters getRuleParamters() { return m_params; }

	inline const Variable& getVariable(const std::string& name) 
	{
		return m_variables[name];
	}

	inline CVariableIter cbegin()
	{
		return m_variables.cbegin();
	}
	
	inline CVariableIter cend()
	{
		return m_variables.cend();
	}
	// name and value will be copied
	bool addRule(const std::string& name, const std::string& value, Variable::Type type, uint8_t flags = 0);

	ConditionalTree m_conditions;

	void searchVariableName(const std::string& search, std::vector<const Variable*> &);

private:
	
	std::string m_name;
	RuleParameters m_params;

	std::unordered_map<std::string, Variable> m_variables;
};


}
#endif // CHANNEL_RULE_H_