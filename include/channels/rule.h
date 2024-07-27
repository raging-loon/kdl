#ifndef CHANNEL_RULE_H_
#define CHANNEL_RULE_H_

#include <string>
#include <unordered_map>
#include "type.h"

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
public:
	Rule();

public:
	/// Getters/Setters

	inline void setName(const std::string& newName) { m_name = newName; }

	inline const std::string& getName() { return m_name; }

	inline void setRuleParameters(RuleParameters rp) { m_params = rp; };


	/// 
	/// PURPOSE
	///		Parse 'rp' and set the rule parameter 
	///		Returns false if the rule parameter is invalid.
	/// 
	bool setRuleParameters(const std::string& rp);
	
	/// Returns a copy
	inline RuleParameters getRuleParamters() { return m_params; }

public:
	// name and value will be copied
	bool addRule(const std::string& name, const std::string& value, Variable::Type type, uint8_t flags = 0);

private:
	
	std::string m_name;
	RuleParameters m_params;

	std::unordered_map<std::string, Variable> m_variables;

};


}
#endif // CHANNEL_RULE_H_