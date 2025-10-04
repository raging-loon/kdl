#include "ASTDumper.h"

#include "ASTNode.h"

#include <iostream>
#include <iomanip>

namespace kdl
{

void ASTDumper::dump(const NodePtr<ASTProgram>& root)
{
    root->accept(*this);
}

void ASTDumper::visit(const ASTNode& node)
{
}
void ASTDumper::visit(const ASTProgram& node)
{
    std::cout << "-=== PROGRAM ===-\n";
    
    m_indentLevel += 1;

    for (const auto& child : node.topDecls)
    {
        child->accept(*this);
    }
}
void ASTDumper::visit(const ASTIdentifier& node)
{
    printf("In identifier\n");

}
void ASTDumper::visit(const ASTLiteral& node)
{
}
void ASTDumper::visit(const ASTBinaryOperation& node)
{
}
void ASTDumper::visit(const ASTUnaryOperation& node)
{
}
void ASTDumper::visit(const ASTStmt& node)
{
}
void ASTDumper::visit(const ASTDecl& node)
{
}
void ASTDumper::visit(const ASTBlock& node)
{
}
void ASTDumper::visit(const ASTRule& node)
{
    std::cout << indentation() << "RULE " << node.name << '\n';

    m_indentLevel += 1;

    if (node.evtSource)
    {
        std::cout << indentation() << "EVT_SOURCE: ";
        node.evtSource->accept(*this);
    }

}

std::string ASTDumper::indentation()
{
    return { "\t", m_indentLevel};
}

}