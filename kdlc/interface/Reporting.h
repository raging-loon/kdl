#ifndef KDLC_INTERFACE_REPORTING_H_
#define KDLC_INTERFACE_REPORTING_H_

#include <string_view>
#include <format>
#include "context/SourceManager.h"

namespace kdl
{

enum class ReportLevel
{
    INFO,
    WARNING,
    ERROR
};

struct ReportInfo
{
    FileID fileID;
    SourceLocation location;
};

class Report
{
public:

    template <class... Args>
    static void Error(ReportInfo& reportInfo, std::string_view message, Args&&... args)
    {
        ReportInternal(ReportLevel::ERROR, reportInfo, message, std::forward<Args>(args)...);
    }

private:

    template <class... Args>
    static void ReportInternal(ReportLevel rl, ReportInfo& r, std::string_view message, Args&&... args)
    {
        std::string formattedMsg = std::vformat(message, std::make_format_args(args...));
        ReportFinal(rl, r, formattedMsg);
    }

    static void ReportFinal(ReportLevel rl, ReportInfo& r, const std::string& message);

    static void PrintErrorMessage(ReportLevel rl, ReportInfo& r, const std::string& message);

    static void PrintSourceSection(ReportInfo& r);
};


} // kdl



#endif // KDLC_INTERFACE_REPORTING_H_