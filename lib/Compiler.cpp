#include "Compiler.h"
#include "parser/CompilerMessage.h"
#include "parser/Combinator.h"
#include "parser/RuleParser.h"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

using kdl::Compiler;

Compiler::Compiler()
{
}

Compiler::~Compiler()
{
}

bool Compiler::compileFile(const char* filename)
{
	return false;
}

bool Compiler::compileSource(const char* source)
{


	std::ifstream in("../../../tests/test.kdl");
	//buf << in.rdbuf();

	auto sz = std::filesystem::file_size("../../../tests/test.kdl");

	std::string buf(sz, 0);
	in.read((char*)buf.c_str(), sz);

	kdl::CompilerMessage::setSource(buf.c_str(), "tests/test.kdl", buf.length());
	kdl::Lexer lex(buf.c_str(), buf.length());

	if (lex.scan() < 0)
		return 1;

	RuleBlockList ruleBlocks;
	
	kdl::Combinator parse(lex.getTokens());
	if (!parse.parse())
		printf("errrn\n");

	printf("Found %zd rules\n",parse.getBlockList().size());
	for (auto& i : parse.getBlockList())
	{
		Rule r;
		RuleParser rp(i, r);
		rp.parse();
	}

	return true;
}
