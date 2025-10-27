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
        rules.push_back(cur);
    }

    return rules;
}

} // kdl