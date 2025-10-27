#ifndef KDLC_INTERMEDIATE_SEMANTIC_ANALYZER_H_
#define KDLC_INTERMEDIATE_SEMANTIC_ANALYZER_H_

#include "Rule.h"

#include "TypeSystem.h"



namespace kdl
{



///
/// @brief
///     Perform Type Checking
/// 
class SemanticAnalyzer
{
public:

    ///
    /// @brief
    ///     Perform type analysis on a rule
    /// 
    bool analyzeRule(Rule& rule);

private:
    
    bool analyzeDeclarations(Rule& rule);

    bool analyzeConditionExpr(Rule& rule);

    BaseType typeExpression(NodePtr<ASTNode> node);

    BaseType typeLiteral(const NodePtr<ASTLiteral>& literal);
  
    BaseType typeBinaryExpression(const NodePtr<ASTBinaryOperation>& binOp);

    BaseType typeIdentifier(const std::string& name);

private:

    std::unordered_map<
        std::string,
        BaseType
    > m_localIdentifiers{};
};

} // kdl

#endif // KDLC_INTERMEDIATE_SEMANTIC_ANALYZER_H_