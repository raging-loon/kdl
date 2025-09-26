#include "CompilerContext.h"

#include "parser/Lexer.h"

namespace kdl
{

CompilerContext CompilerContext::s_instance{};

CompilerContext::CompilerContext()
    : m_srcMgr{}
{

}

CompilerContext::~CompilerContext()
{

}

bool CompilerContext::compileSource(
    const std::string& src
)
{
    FileID id = m_srcMgr.addRawSource(src);

    return true;
}

} // kdl

