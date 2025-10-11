#include "ASTDumper.h"

#include "Nodes.h"

#include <iostream>
#include <iomanip>


#include <unordered_map>
#include <string_view>



namespace kdl
{

static std::unordered_map <
    Operation, std::string_view
> s_operation_str = {
    {Operation::EQUALS,    "EQUALS"},
    {Operation::NE,        "NE"},
    {Operation::NOT,       "NOT"},
    {Operation::NOT_IN,    "NOT_IN"},
    {Operation::IN,        "IN"},
    {Operation::GT,        "GT"},
    {Operation::GEQ,       "GEQ"},
    {Operation::LT,        "LT"},
    {Operation::LEQ,       "LEQ"},
    {Operation::AND,       "AND"},
    {Operation::OR,        "OR"},
    {Operation::INVALID,   "INVALID"},
};

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
    m_indentLevel -= 1;
}
void ASTDumper::visit(const ASTIdentifier& node)
{
    indentation();
    std::cout << "IDENTIFIER: " << node.name << '\n';
}

void ASTDumper::visit(const ASTLiteral& node)
{
    indentation();
    //<<    "LITERAL -> ";
    switch (node.type)
    {
        case ASTLiteral::BOOLEAN:
            std::cout << "BOOLEAN";
            break;
        case ASTLiteral::STRING:
            std::cout << "STRING";
            break;
        case ASTLiteral::INTEGER:
            std::cout << "INTEGER";
            break;
        default:
            std::cout << "UNKNOWN";
            break;
    }
    std::cout << " LITERAL -> ";
    std::cout << node.value << '\n';
}

void ASTDumper::visit(const ASTArrayLiteral& node)
{
    indentation();
    std::cout << "ARRAY LITERAL\n";
    m_indentLevel++;

    for (const auto& value : node.values)
    {
        value->accept(*this);
    }


    m_indentLevel--;
}
void ASTDumper::visit(const ASTBinaryOperation& node)
{
    indentation();
    std::cout << "BINARY_OP: " << s_operation_str[node.operation] << '\n';

    m_indentLevel++;

    node.lhs->accept(*this);
    node.rhs->accept(*this);


    m_indentLevel--;
}

void ASTDumper::visit(const ASTUnaryOperation& node)
{
    indentation();
    std::cout << "UNARY_OP: " << s_operation_str[node.operation] << '\n';
    m_indentLevel++;

    node.operand->accept(*this);

    m_indentLevel--;
}

void ASTDumper::visit(const ASTDecl& node)
{
    indentation();
    std::cout << "DECL: " << node.name << '\n';
    m_indentLevel++;
    node.value->accept(*this);
    m_indentLevel--;
}

void ASTDumper::visit(const ASTBlock& node)
{
    m_indentLevel++;
    indentation();
    std::cout << "BLOCK: \n";
    m_indentLevel++;
    for (const auto& stmt : node.statements)
    {
        stmt->accept(*this);
    }
    m_indentLevel--;

}

void ASTDumper::visit(const ASTRule& node)
{
    std::cout << "RULE " << node.name << '\n';

    m_indentLevel += 1;

    if (node.evtSource)
    {
        indentation();
        m_indentLevel += 1;
        std::cout << "EVT_SOURCE\n";
        node.evtSource->accept(*this);
        m_indentLevel -= 1;
    }

    if (node.predicate)
    {
        indentation();
        std::cout << "PREDICATE\n";
        m_indentLevel += 1;
        node.predicate->accept(*this);
        m_indentLevel -= 1;
    }

    m_indentLevel--;

}

void ASTDumper::visit(const ASTFieldAccess& node)
{
    indentation();
    std::cout << "FIELD_ACCESS: " << node.fieldName << '\n';

    if (node.target)
    {
        m_indentLevel++;

        node.target->accept(*this);

        m_indentLevel--;
    }
}

void ASTDumper::indentation()
{
    for (int i = 0; i < m_indentLevel * 2; i++)
    {
        std::cout.put(' ');
    }
}

}