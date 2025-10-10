#ifndef KDLC_AST_NODES_H_
#define KDLC_AST_NODES_H_

#include <memory>
#include <utility>
#include <vector>
#include <string>
#include <type_traits>


namespace kdl {

enum class NodeType
{
    UNTYPED,
    PROGRAM,
    RULE,
    BLOCK,
    FIELD_ACCESS,
    LITERAL,
    ARRAY_LITERAL,
    IDENTIFIER,
    BINARY_OP,
    UNARY_OP,
    DECL,
    ERROR
};


enum class Operation
{
    EQUALS,
    NE,
    NOT,
    NOT_IN,
    IN,
    GT,
    GEQ,
    LT,
    LEQ,
    AND,
    OR,
    INVALID
};

struct Node;

template <class T>
using NodePtr = std::shared_ptr<T>;

template <class T, class... Args>
constexpr NodePtr<T> MakeNode(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

#define NEW_NODE_TYPE(type) \
    inline NodeType getType() const override { return type ; }

struct ASTNode
{
    virtual ~ASTNode() = default;
    virtual inline NodeType getType() const { return NodeType::UNTYPED; }

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
        STRING,
        INTEGER,
        BOOLEAN,
        ARRAY,
        INVALID
    };

    Type type;

    std::string value;

};

struct ASTArrayLiteral : ASTNode
{
    NEW_NODE_TYPE(NodeType::ARRAY_LITERAL);

    std::vector<NodePtr<ASTNode>> values;
};


///
/// @brief
///     Binary operation, e.g. 1 + 1
///     
struct ASTBinaryOperation : ASTNode
{
    NEW_NODE_TYPE(NodeType::BINARY_OP);

    Operation operation;

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

    Operation operation;
    NodePtr<ASTNode> operand;
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

    NodePtr<ASTIdentifier> evtSource;
    NodePtr<ASTBlock> predicate;
    NodePtr<ASTBinaryOperation> condition;
};

struct ASTFieldAccess : ASTNode
{
    NEW_NODE_TYPE(NodeType::FIELD_ACCESS);

    std::string fieldName;
    NodePtr<ASTFieldAccess> target{ nullptr };

};


} // kdl


#endif // KDLC_AST_NODES_H_