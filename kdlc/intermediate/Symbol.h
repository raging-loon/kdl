#ifndef KDLC_INTERMEDIATE_SYMBOL_H_
#define KDLC_INTERMEDIATE_SYMBOL_H_

#include <string>

#include "TypedNodes.h"
#include "context/SourceManager.h"
namespace kdl
{

struct Symbol
{
    SourceLocation srcLoc;
    Type type{ Type::UNTYPED };
};




} // kdl



#endif // KDLC_INTERMEDIATE_SYMBOL_H_