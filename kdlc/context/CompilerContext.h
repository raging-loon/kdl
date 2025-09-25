#ifndef KDLC_CONTEXT_COMPILER_CONTEXT_H_
#define KDLC_CONTEXT_COMPILER_CONTEXT_H_

#include <string>


#include "SourceManager.h"

namespace kdl
{

///
/// @brief
///     Manages all thing compiler related
/// 
class CompilerContext
{
public:

    /// bool compileFile(const std::string& path);
    
    bool compileSource(const std::string& source);


public:
    // getters

    SourceManager& getSourceManager() { return m_srcMgr; }
private:

    SourceManager m_srcMgr;

public:
    ////// SINGLETON BOILERPLATE //////

    inline static CompilerContext& getContext()
    {
        return s_instance;
    }

    CompilerContext(const CompilerContext&) = delete;
    CompilerContext(CompilerContext&&) = delete;
    
    CompilerContext& operator=(const CompilerContext&) = delete;
    CompilerContext& operator=(CompilerContext&&) = delete;

private:
    static CompilerContext s_instance;

    CompilerContext();
    ~CompilerContext();

};

} // kdl

#define COMPILER_CTX    kdl::CompilerContext::getContext()

#endif // KDLC_CONTEXT_COMPILER_CONTEXT_H_