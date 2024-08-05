#include "cli/TerminalInterface.h"
#include "kdlc.h"
#include "KdlOptions.h"
#include "Compiler.h"
#include <functional>
#include <algorithm>
#include <stdio.h>
#include <filesystem>

using kdl::TerminalInterface;
using kdl::KdlOptions;

static void printHelp()
{
	printf(
		"Kestrel Detection Language Compiler v%d.%d\n"
		"Usage: ./kdlc <options>\n"
		"OPTIONS\n"
		"  -f <file>            specify files to compile, wildcards are accepted\n"
		"  -o <dir>             directory for output\n"
		"  -v, --verbose        enable verbose output\n"
		"  -b, --backend        backend to use for code generation\n"
		"                       Supported backends: clang\n"
		"  -k                   !!unimplemented!! Rules should use kernel mode functions\n"
		"  -p, --platform       platform rules should be compiled for\n"
		"                       this option will filter any rules whose platform indicator does not match\n"
		"DEBUG OPTIONS\n"
		"  -d, --debug          Print general debug info\n"
		"  --dump-ctree         Print Conditional Trees\n"
		"  --dump-tokens        Print Tokens\n"
		"  --dump-blocks        Print Rule Blocks as they are found\n"
		"EXAMPLES\n"
		"  kdlc -f myrules.kdl -o myrules/\n"
		"  kdlc -f rules.kdl -o rules/ -b clang --platform linux\n"
		"  kdlc -f rules/*.kdl -o out/ -b clang\n"
		, kdl::versionMajor, kdl::versionMinor
	);
}

static void printNeedArgument(const char* arg)
{
	printf("%s requires an argument\n", arg);
}

TerminalInterface::TerminalInterface(int argc, char** argv)
	: argc(argc), argv(argv), m_backend(nullptr)
{
}

int TerminalInterface::run()
{
	if (argc == 1)
	{
		printHelp();
		return 0;
	}

	if (!parseArgs())
		return 1;

	if (!validateArgs())
		return 1;


	for (const auto& i : m_ruleFiles)
	{
		Compiler cmplr;
		if (!cmplr.compileFile(i.c_str()))
			return 1;
		cmplr.writeFiles(m_outdir);
	}


	return 0;
}

bool kdl::TerminalInterface::parseArgs()
{
	bool isParsingFiles = false;
	for (int i = 1; i < argc; i++)
	{
		if (isParsingFiles)
		{
			if (argv[i][0] == '-')
				isParsingFiles = false;
			else
			{
				m_ruleFiles.push_back(argv[i]);
				continue;
			}
		}

		
		if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--files") == 0)
			isParsingFiles = true;

		else if (strcmp(argv[i], "-o") == 0)
		{
			if (i == argc || argv[i + 1][0] == '-')
			{
				printNeedArgument(argv[i]);
				return false;
			}
			m_outdir = argv[++i];

		}
		
		else if (strcmp(argv[i], "-b") == 0 || strcmp(argv[i], "--backend") == 0)
		{
			if (i == argc || argv[i + 1][0] == '-')
			{
				printNeedArgument(argv[i]);
				return false;
			}
			m_backend = argv[++i];

		}

		else if (strcmp(argv[i], "-v") == 0 || strcmp(argv[i], "--verbose") == 0)
			KdlOptions::verbose = true;
		else if (strcmp(argv[i], "-d") == 0 || strcmp(argv[i], "--debug") == 0)
			KdlOptions::debug = true;

		else if (strcmp(argv[i], "--dump-ctree") == 0)
			KdlOptions::dumpCTrees = true;
		
		else if (strcmp(argv[i], "--dump-tokens") == 0)
			KdlOptions::dumpLexerTokens = true;
		
		else if (strcmp(argv[i], "--dump-blocks") == 0)
			KdlOptions::dumpBlocks = true;

		else if (strcmp(argv[i], "/?") == 0 || strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)
		{
			printHelp();
			return false;
		}

		else
		{
			printf("%d\n",i);
			printf("Unknown argument: %s\n", argv[i]);
			return false;
		}
	}


	return true;
}

bool TerminalInterface::validateArgs()
{

	return validateOutput()    && 
		   validateBackend()   && 
		   validateInputFiles();
}

bool TerminalInterface::validateInputFiles()
{
	if (m_ruleFiles.empty())
	{
		printf("kdlc requires at least one file\n");
		return false;
	}

	for (const auto& i : m_ruleFiles)
	{
		if (!i.ends_with(".kdl"))
			printf("Warning: %s does not have the '.kdl' extension\n", i.c_str());
	}

	return true;
}

bool TerminalInterface::validateOutput()
{
	if (m_outdir.empty())
	{
		printf("No output directory set. Defaulting to cwd.\n");
		m_outdir = "./";
	}

	if (!std::filesystem::exists(m_outdir))
	{
		if(KdlOptions::verbose)
			printf("Creating new directory %s...\n", (char*)m_outdir.c_str());
		
		if (!std::filesystem::create_directory(m_outdir))
		{
			printf("Failed to create new directory '%s'!\n", (char*)m_outdir.c_str());
		}
	}

	m_outdir = std::filesystem::absolute(m_outdir).string();
	

	std::replace(m_outdir.begin(), m_outdir.end(), '\\', '/');
	return true;
}

bool TerminalInterface::validateBackend()
{

	if (!m_backend)
	{
		printf("Need a backend. Specify with '-b'\n");
		return false;
	}



	return true;
}

