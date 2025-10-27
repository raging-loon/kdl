#ifndef KDLC_INTERMEDIATE_SYMBOL_H_
#define KDLC_INTERMEDIATE_SYMBOL_H_

#include <string>

#include "TypeSystem.h"
#include "context/SourceManager.h"
namespace kdl
{

struct Symbol
{
    SourceLocation srcLoc;
    BaseType type{ BaseType::UNTYPED };
};




} // kdl



#endif // KDLC_INTERMEDIATE_SYMBOL_H_