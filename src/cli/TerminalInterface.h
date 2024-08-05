#ifndef CLI_TERMINAL_INTERFACE_H_
#define CLI_TERMINAL_INTERFACE_H_

#include <vector>
#include <string>

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
	bool validateInputFiles();
	bool validateOutput();
	bool validateBackend();
private:

	char** argv;
	int argc;
	std::vector<std::string> m_ruleFiles;
	const char* m_outdir;
	const char* m_backend;
};

};


#endif // CLI__TERMINAL_INTERFACE_H_