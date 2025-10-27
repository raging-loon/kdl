#include "SemanticException.h"

#include <string_view>
#include <format>

#include "interface/Reporting.h"

namespace kdl
{
constexpr std::string_view UNKNOWN_IDENTIFIER_FMT_MST = "Unknown Identifier {}";

UnknownIdentifierException::UnknownIdentifierException(NodePtr<ASTIdentifier> perp)
    : m_perp{ perp },
    std::logic_error{ std::format(UNKNOWN_IDENTIFIER_FMT_MST, perp->name) }
{
}

void UnknownIdentifierException::print() const
{
}

} // kdl