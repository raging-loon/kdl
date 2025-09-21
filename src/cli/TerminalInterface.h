#ifndef CLI_TERMINAL_INTERFACE_H_
#define CLI_TERMINAL_INTERFACE_H_

#include <vector>
#include <string>
#include <filesystem>
namespace kdl
{

class TerminalInterface
{
public:
    TerminalInterface(int argc, char** argv);

    int run();

private:

    bool parseArgs();
    bool validateArgs();

    /// 
    /// PURPOSE
    ///		Make sure input files are set and exist
    ///		Will produce a warning if a file does not end 
    ///		in '.kdl'
    /// 
    bool validateInputFiles();

    ///
    /// PURPOSE
    ///		Ensure output directory is set
    ///		Will create it if it does not exist
    /// 
    bool validateOutput();

    bool validateBackend();

private:

    char** argv;
    int argc;
    std::vector<std::string> m_ruleFiles;
    std::string m_outdir;
    const char* m_backend;
};

};


#endif // CLI__TERMINAL_INTERFACE_H_