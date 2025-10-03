#ifndef KDLC_PARSER_AST_AST_NODE_H_
#define KDLC_PARSER_AST_AST_NODE_H_

#include "context/SourceManager.h"
#include "parser/Token.h"

#include <memory>
#include <string>
#include <vector>
#include <variant>
#include <concepts>
#include <type_traits>

namespace kdl
{
struct ASTNode;
template <class N>
concept ISAstNode = std::is_base_of_v<ASTNode, N>;

template <class T> 
    requires ISAstNode<T>
using NodePtr = std::unique_ptr<T>;

template <class T> 
    requires ISAstNode<T>
using NodeList = std::vector<T>;

template <class T, class... Args> 
    requires ISAstNode<T>
constexpr NodePtr<T> MakeNode(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

///
/// @brief
///     Base AST Node
/// 
struct ASTNode
{

    ASTNode(const TokenPtr tp)
    {
        if (!tp)
            return;
        sloc.lineNumber = tp->lineNumber;
        sloc.lineOffset = tp->lineStart;
        sloc.locationStart = tp->startPosition;
        if (!tp->value.empty())
            sloc.locationEnd = sloc.locationStart + tp->value.size();
    }

    /// For diagnostics
    SourceLocation sloc{};

    virtual ~ASTNode() = default;
};

///
/// @brief
///     Top level program
///     Represents all rules parsed
/// 
struct ASTProgram : ASTNode
{
    using ASTNode::ASTNode;
    std::vector<NodePtr<ASTNode>> topDecls;
};

struct ASTIdentifier : ASTNode
{
    using ASTNode::ASTNode;

    std::string name;
};

struct ASTLiteral : ASTNode
{
    using ASTNode::ASTNode;

    enum Type {
        INTEGER = KDL_T_INTEGER,
        STRING = KDL_T_STRING,
        BOOLEAN
    };

    Type type;

    std::variant<
        std::string,
        NodePtr<ASTNode>
    > value;

};



///
/// @brief
///     Binary operation, e.g. 1 + 1
///     
struct ASTBinaryOperation : ASTNode
{
    using ASTNode::ASTNode;

    token_t operation;

    NodePtr<ASTNode> lhs;
    NodePtr<ASTNode> rhs;
};

///
/// @brief
///     Unary operation such as negation
/// 
struct ASTUnaryOperation : ASTNode
{
    using ASTNode::ASTNode;

    token_t operation;
    NodePtr<ASTNode> operand;
};

struct ASTStmt : ASTNode
{
    using ASTNode::ASTNode;

    NodePtr<ASTNode> stmt;
};

struct ASTDecl : ASTNode
{
    using ASTNode::ASTNode;

    std::string name;

    NodePtr<ASTNode> value;
};

///
/// @brief
///     Arbitrary block of statements
struct ASTBlock : ASTNode 
{
    using ASTNode::ASTNode;

    std::vector<
        NodePtr<ASTDecl>
    > statements;
};

///
/// @brief
///     A rule block 
/// 
struct ASTRule : ASTNode
{
    using ASTNode::ASTNode;

    std::string name;

    NodePtr<ASTNode> evtSource;
    NodePtr<ASTBlock> predicate;
};



} // kdl


#endif // KDLC_PARSER_AST_AST_NODE_H_