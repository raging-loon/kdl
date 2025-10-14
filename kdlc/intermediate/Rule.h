#ifndef KDLC_INTERMEDIATE_RULE_H_
#define KDLC_INTERMEDIATE_RULE_H_

#include "ast/Nodes.h"
#include "Symbol.h"
namespace kdl
{

class SymbolExtractor;

///
/// @brief
///     IR of a Rule
/// 
class Rule
{
    friend class SymbolExtractor;
public:

    Rule();
    Rule(NodePtr<ASTRule> root);


    auto getUntypedRoot() { return m_untypedRoot; }

private:

    void addSymbol(
        const std::string& name,
        const Symbol& value
    );


private:
    NodePtr<ASTRule> m_untypedRoot;

    std::unordered_map<std::string, Symbol> m_symbols;
};

} // kdl


#endif // KDLC_INTERMEDIATE_RULE_H_