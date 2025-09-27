#include "Reporting.h"

#include "context/CompilerContext.h"

#include <sstream>
#include <string_view>

using std::string, std::stringstream, std::string_view;

constexpr string_view BOLD_ESCAPE_CODE  = "\033[;4m";
constexpr string_view CLEAR_ESCAPE_CODE = "\033[0m";

namespace kdl
{

void Report::ReportFinal(
    ReportLevel level,
    ReportInfo& reportInfo,
    const std::string& message
)
{
    PrintErrorMessage(level, reportInfo, message);

    PrintSourceSection(reportInfo);
}

void Report::PrintErrorMessage(
    ReportLevel rl, 
    ReportInfo& r, 
    const std::string& message
)
{
    const auto* fi = SRC_MGR.getSourceFile(r.fileID);
    const auto& srcLoc = r.location;
    std::printf(
        "%s: %s:%d:%d\n",
        message.c_str(),
        fi->filename.c_str(),
        srcLoc.lineNumber,
        srcLoc.locationStart
    );
}

void Report::PrintSourceSection(ReportInfo& reportInfo)
{
    const auto* fi = SRC_MGR.getSourceFile(reportInfo.fileID);
    const auto& srcLoc = reportInfo.location;

    if (!fi)
        return;

    stringstream buffer{};

    string lineNo = std::format("\t {} | ", srcLoc.lineNumber);

    int pad = lineNo.length();

    buffer << lineNo;

    int startChar = srcLoc.locationStart;
    int endChar = srcLoc.locationEnd;
    if (endChar == 0)
        endChar = srcLoc.locationStart;

    // write the line
    for (int i = srcLoc.lineOffset - 1; i < fi->contents.length(); i++)
    {
        if (i == startChar)
            buffer << BOLD_ESCAPE_CODE;

        buffer << fi->contents[i];

        if (i == endChar)
            buffer << CLEAR_ESCAPE_CODE;

        if (fi->contents[i] == '\n')
            break;
    }
    // make sure bold is off
    buffer << CLEAR_ESCAPE_CODE << '\t';

    // padding for new line
    for (int i = 0; i < (pad - 3); i++)
        buffer << ' ';
 
    buffer << "| ";

    // pad before squiggle thing
    for (int i = srcLoc.lineOffset; i < startChar; i++)
    {
        buffer << ' ';
    }

    // draw squiggle that points to source
    for (int i = startChar; i <= endChar; i++)
    {
        buffer << '^';
    }


    std::printf("%s\n", buffer.str().c_str());
}



} // kdl