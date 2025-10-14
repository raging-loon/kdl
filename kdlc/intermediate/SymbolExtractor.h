#ifndef KDLC_INTERMEDIATE_SYMBOL_EXTRACTOR_H_
#define KDLC_INTERMEDIATE_SYMBOL_EXTRACTOR_H_

#include "Rule.h"

namespace kdl
{

///
/// @brief
///     Extract symbols from the top declarations 
///     and from rules
/// 
class SymbolExtractor
{
public:

    ///
    /// @brief
    ///     Extract rules from the program root
    ///     Then for each rule, extract the symbols declared
    /// 
    std::vector<Rule> extractRules(NodePtr<ASTProgram> programRoot);
private:


    void extractSymbols(Rule& rule, NodePtr<ASTRule> root);
};

} // kdl


#endif // KDLC_INTERMEDIATE_SYMBOL_EXTRACTOR_H_