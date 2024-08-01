#include "Compiler.h"
#include "parser/CompilerMessage.h"
#include "parser/Combinator.h"
#include "parser/RuleParser.h"
#include "channels/RulePool.h"
#include "conditional/ConditionalReferenceValidator.h"

#include "backend/clang/CLangCG.h"

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

	m_rules.resize(parse.getBlockList().size());
	for (auto& i : parse.getBlockList())
	{
		RuleParser rp(i, *m_rules.add());
		rp.parse();
	} 

	ConditionalReferenceValidator crv(m_rules);
	if (!crv.check())
		printf("err0r\n");

	ClangCodeGen clcg("test.kdl", m_rules);

	clcg.generate();

	for (const auto& [file, src] : clcg.getFileMap())
		m_files[file] = src->str();

	return true;
}

void Compiler::writeFiles(const char* directory)
{
	for (const auto& [file, src] : m_files)
	{
		writeFile(std::string(directory) + "/" + file, src);
			
	}
}

void Compiler::writeFile(const std::string& name, const std::string& source)
{
	std::ofstream out(name);

	out.write(source.c_str(), source.length());

}
