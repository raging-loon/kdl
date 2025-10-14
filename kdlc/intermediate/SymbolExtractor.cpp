#include "SymbolExtractor.h"

namespace kdl
{

std::vector<Rule> SymbolExtractor::extractRules(NodePtr<ASTProgram> programRoot)
{
    std::vector<Rule> rules{};

    for (auto& decl : programRoot->topDecls)
    {
        Rule cur{ decl };

        std::cout << "Found rule: " << decl->name << '\n';

        extractSymbols(cur, decl);
    }

    return rules;
}
void SymbolExtractor::extractSymbols(Rule& rule, NodePtr<ASTRule> root)
{
    NodePtr<ASTBlock> predicate = root->predicate;

    for (const auto& decl : predicate->statements)
    {
        Symbol sym{
            .srcLoc = decl->srcLoc
        };

        std::cout << "\tFound Symbol: " << decl->name << '\n';

        rule.addSymbol(
            decl->name,
            sym
        );
    }
}

} // kdl