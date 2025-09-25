#include "SourceManager.h"

namespace kdl
{

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

std::optional<FileInfo> SourceManager::getSourceFile(FileID id)
{
    auto i = m_fileMap.find(id);

    if (i == m_fileMap.cend())
        return std::nullopt;

    return i->second;
}



} // kdl