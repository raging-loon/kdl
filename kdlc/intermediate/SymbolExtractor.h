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
    /// 
    std::vector<Rule> extractRules(NodePtr<ASTProgram> programRoot);
private:
};

} // kdl


#endif // KDLC_INTERMEDIATE_SYMBOL_EXTRACTOR_H_