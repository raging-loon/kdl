#include "channels/rule.h"
#include "utils.h"
#include <string>
using kdl::Rule;

Rule::Rule()
    : m_name({}),
    m_params(RuleParameters::NONE),
    m_variables({}),
    m_conditions(0)
{
}

bool Rule::setRuleParameters(const std::string& rp)
{
    // todo: test for performance here
    std::string lowerCopy = rp;

    utils::toLowerCase(lowerCopy);

    if (lowerCopy == "net")
        m_params = RuleParameters::NET;

    else if (lowerCopy == "file")
        m_params = RuleParameters::FILE;

    else if (lowerCopy == "process")
        m_params = RuleParameters::PROCESS;
    else
        return false;

    return true;

}

bool Rule::addRule(const std::string& name, const std::string& value, Variable::Type type, uint8_t flags)
{
    if (m_variables.find(name) != m_variables.end())
        return false;


    m_variables[name] = { .searchstr = value, .type = type, .searchMods = flags };

    return true;
}
// todo: is this hacky?
void Rule::searchVariableName(const std::string& search, std::vector<const Variable*>& out)
{
    for (const auto& [rname, rvar] : m_variables)
    {
        if (rname.starts_with(search.c_str()))
            out.push_back(&rvar);
    }
}

