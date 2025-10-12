#include "SourceManager.h"

#include <Token.h>

#include <cassert>

namespace kdl
{

SourceLocation::SourceLocation(const antlr4::tree::TerminalNode* node)
{
    assert(node);
   
    auto* symbol = node->getSymbol();

    assert(symbol);

    lineNumber = symbol->getLine();
    lineOffset = symbol->getCharPositionInLine();
    locationStart = symbol->getStartIndex();
    locationEnd = symbol->getStopIndex();
}

SourceManager::SourceManager()
    : m_fileMap{}
{

}

FileID SourceManager::addSourceFile(const std::string& path)
{
    return FileID();
}

FileID SourceManager::addRawSource(const std::string& source)
{
    FileID id = m_baseKey++;
    m_fileMap[id] = { "?", source };
    return id;
}

const FileInfo* SourceManager::getSourceFile(FileID id) const
{
    auto i = m_fileMap.find(id);

    if (i == m_fileMap.cend())
        return nullptr;

    return &i->second;
}



} // kdl