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
concept IsAstNode = std::is_base_of_v<ASTNode, N>;

template <class T> 
    requires IsAstNode<T>
using NodePtr = std::unique_ptr<T>;

template <class T> 
    requires IsAstNode<T>
using NodeList = std::vector<T>;

template <class T, class... Args> 
    requires IsAstNode<T>
constexpr NodePtr<T> MakeNode(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}


enum class NodeType
{
    UNTYPED,
    PROGRAM,
    RULE,
    PREDICATE,
    IDENTIFIER,
    LITERAL,
    BINARY_OP,
    UNARY_OP,
    STMT,
    DECL,
    BLOCK
};

#define NEW_NODE_TYPE(type) \
    using ASTNode::ASTNode; \
    virtual inline NodeType getNodeType() const override  { return type; }
    

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

    virtual inline NodeType getNodeType() const { return NodeType::UNTYPED; }
};

///
/// @brief
///     Top level program
///     Represents all rules parsed
/// 
struct ASTProgram : ASTNode
{
    NEW_NODE_TYPE(NodeType::PROGRAM);

    std::vector<NodePtr<ASTNode>> topDecls;


};

struct ASTIdentifier : ASTNode
{
    NEW_NODE_TYPE(NodeType::IDENTIFIER);

    std::string name;
};

struct ASTLiteral : ASTNode
{
    NEW_NODE_TYPE(NodeType::LITERAL);

    enum Type {
        INTEGER = KDL_T_INTEGER,
        STRING  = KDL_T_STRING,
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
    NEW_NODE_TYPE(NodeType::BINARY_OP);

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
    NEW_NODE_TYPE(NodeType::UNARY_OP);

    token_t operation;
    NodePtr<ASTNode> operand;
};

struct ASTStmt : ASTNode
{
    NEW_NODE_TYPE(NodeType::STMT);

    NodePtr<ASTNode> stmt;
};

struct ASTDecl : ASTNode
{
    NEW_NODE_TYPE(NodeType::DECL);

    std::string name;

    NodePtr<ASTNode> value;
};

///
/// @brief
///     Arbitrary block of statements
struct ASTBlock : ASTNode 
{
    NEW_NODE_TYPE(NodeType::BLOCK);

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
    NEW_NODE_TYPE(NodeType::RULE);

    std::string name;

    NodePtr<ASTNode> evtSource;
    NodePtr<ASTBlock> predicate;
};



} // kdl


#endif // KDLC_PARSER_AST_AST_NODE_H_