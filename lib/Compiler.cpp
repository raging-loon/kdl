#include "Compiler.h"
#include "parser/CompilerMessage.h"
#include "parser/Combinator.h"
#include "parser/RuleParser.h"
#include "channels/RulePool.h"
#include "conditional/ConditionalReferenceValidator.h"
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
	std::ifstream in(filename);
	auto sz = std::filesystem::file_size(filename);

	std::string buffer(sz, 0);

	in.read((char*)buffer.c_str(), sz);
	kdl::CompilerMessage::setSource(buffer.c_str(), filename, buffer.length());

	return compileSource(buffer.c_str(), sz);
}

bool Compiler::compileSource(const char* source, int len)
{
	kdl::Lexer lex(source, len);

	if (lex.scan() < 0)
		return 1;

	RuleBlockList ruleBlocks;
	
	kdl::Combinator parse(lex.getTokens());
	if (!parse.parse())
		printf("errrn\n");

	Rule* list = new Rule[parse.getBlockList().size()];
	memset(list, 0, sizeof(Rule));
	int listSize = 0;

	for (auto& i : parse.getBlockList())
	{
		Rule* curPos = &list[listSize++];
		Rule* cur = new (curPos) Rule();
		RuleParser rp(i, *cur);
		rp.parse();
	}


	delete[] list;
	return true;
}
