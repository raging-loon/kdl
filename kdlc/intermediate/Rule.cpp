#include "Rule.h"

namespace kdl
{

Rule::Rule()
    : m_untypedRoot{ nullptr }
{

}
Rule::Rule(NodePtr<ASTRule> root)
    : m_untypedRoot{ root }
{

}

void Rule::addSymbol(const std::string& name, const Symbol& value)
{
    m_symbols[name] = value;
}

} // kdl