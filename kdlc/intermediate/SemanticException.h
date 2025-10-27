#ifndef KDLC_INTERMEDIATE_SEMANTIC_EXCEPTION_H_
#define KDLC_INTERMEDIATE_SEMANTIC_EXCEPTION_H_

#include <exception>
#include <string>

#include <ast/Nodes.h>

namespace kdl
{

class UnknownIdentifierException : public std::logic_error
{
public:

    UnknownIdentifierException(NodePtr<ASTIdentifier> perp);

    void print() const;

private:
    NodePtr<ASTIdentifier> m_perp;
};

} // kdl

#endif // KDLC_INTERMEDIATE_SEMANTIC_EXCEPTION_H_