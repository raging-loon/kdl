#include "SemanticAnalyzer.h"
#include "SemanticException.h"
namespace kdl
{

static void PrintType(const std::string& name, BaseType type)
{
    std::string typeName{};

    switch (type)
    {
        case  BaseType::UNTYPED:
            typeName = "UNTYPED";
            break;
        case BaseType::STRING:
            typeName = "STRING";
            break;
        case BaseType::INT:
            typeName = "INT";
            break;
    }

    printf("%s -> %s\n", name.c_str(), typeName.c_str());
}

bool SemanticAnalyzer::analyzeRule(Rule& rule)
{
    m_localIdentifiers.clear();

    return analyzeDeclarations(rule);
}

bool SemanticAnalyzer::analyzeDeclarations(Rule& rule)
{
    auto root = rule.getUntypedRoot();
    for (const auto& decl : root->predicate->statements)
    {
        auto type = typeExpression(decl->value);
        PrintType(decl->name, type);

        Symbol newSym{};
        newSym.srcLoc = decl->srcLoc;
        newSym.type = type;
        m_localIdentifiers[decl->name] = type;
        rule.addSymbol(decl->name, newSym);
    }
    return false;
}

bool SemanticAnalyzer::analyzeConditionExpr(Rule& rule)
{
    return false;
}

BaseType SemanticAnalyzer::typeExpression(NodePtr<ASTNode> node)
{
    switch (node->getType())
    {
        case NodeType::LITERAL:
        {
            auto dwncst = std::reinterpret_pointer_cast<ASTLiteral>(node);
            return typeLiteral(dwncst);
        }

        case NodeType::BINARY_OP:
        {
            auto dwncst = std::reinterpret_pointer_cast<ASTBinaryOperation>(node);
            return typeBinaryExpression(dwncst);
        }

        case NodeType::IDENTIFIER: 
        {
            auto dwncst = std::reinterpret_pointer_cast<ASTIdentifier>(node);
            return typeIdentifier(dwncst->name);
        }
    }

    return BaseType::UNTYPED;

}


BaseType SemanticAnalyzer::typeLiteral(const NodePtr<ASTLiteral>& literal)
{
    switch (literal->type)
    {
        case ASTLiteral::STRING:
            return BaseType::STRING;
        case ASTLiteral::INTEGER:
            return BaseType::INT;
        default:
            return BaseType::UNTYPED;
    }
}



BaseType SemanticAnalyzer::typeBinaryExpression(const NodePtr<ASTBinaryOperation>& binOp)
{
    BaseType lhsTy = typeExpression(binOp->lhs);
    BaseType rhsTy = typeExpression(binOp->rhs);
    PrintType("lhs", lhsTy);
    PrintType("rhs", rhsTy);




    return BaseType::UNTYPED;

}

BaseType SemanticAnalyzer::typeIdentifier(const std::string& name)
{
    auto found = m_localIdentifiers.find(name);

    if (found == m_localIdentifiers.end())
        return BaseType::UNTYPED;

    return found->second;
}

} // kdl