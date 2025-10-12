#ifndef KDLC_CONTEXT_SOURCE_MANAGER_H_
#define KDLC_CONTEXT_SOURCE_MANAGER_H_

#include <cstdint>
#include <unordered_map>
#include <string>

#include <optional>

// antrl4/src/
#include <tree/TerminalNode.h>

namespace kdl
{

using FileID = uint16_t;


struct SourceLocation
{
    size_t lineNumber;
    size_t lineOffset;
    size_t locationStart;
    size_t locationEnd;

    SourceLocation(const antlr4::tree::TerminalNode* node);

};



struct FileInfo
{
    FileInfo() {}
    FileInfo(
        const std::string& filename,
        const std::string& source
    ) : filename(filename), contents{ source }
    {
    }

    FileInfo& operator=(const FileInfo&) = default;
    FileInfo& operator=(FileInfo&&) = default;
    FileInfo(const FileInfo&) = default;
    FileInfo(FileInfo&&) = default;

    std::string filename;
    std::string contents;
};

///
/// @brief
///     Manage access to and lifetimes of various source files
/// 
/// @details
///     When a file is added, it is given an auto-incrementing ID. 
/// 
class SourceManager
{
public:

    SourceManager();
    ~SourceManager() = default;

    FileID addSourceFile(const std::string& path);

    FileID addRawSource(const std::string& source);

    const FileInfo* getSourceFile(FileID id) const;


private:
    std::unordered_map<
        FileID,
        FileInfo
    > m_fileMap;

    static constexpr uint16_t BASE_KEY_INIT_VAL = 1;
    uint16_t m_baseKey = BASE_KEY_INIT_VAL;
};


} // kdl


#endif // KDLC_CONTEXT_SOURCE_MANAGER_H_