#include "Compiler.h"
#include "parser/CompilerMessage.h"
#include "parser/Combinator.h"
#include "parser/RuleParser.h"
#include "channels/RulePool.h"
#include "conditional/ConditionalReferenceValidator.h"
#include "cli/KdlOptions.h"
#include "backend/clang/CLangCG.h"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>

using kdl::Compiler;

Compiler::Compiler()
	: m_currentFile(nullptr)
{
}

Compiler::~Compiler()
{
}

bool Compiler::compileFile(const char* filename)
{
	std::ifstream in(filename);
	if (!in.good())
	{
		printf("Failed to open %s\n",filename);
		return false;
	}
	auto sz = std::filesystem::file_size(filename);
	std::string buffer(sz, 0);

	in.read((char*)buffer.c_str(), sz);
	kdl::CompilerMessage::setSource(buffer.c_str(), filename, buffer.length());
	m_currentFile = filename;
	return compileSource(buffer.c_str(), sz);
}

bool Compiler::compileSource(const char* source, int len)
{
	kdl::Lexer lex(source, len);

	if (lex.scan() < 0)
		return 1;

	if (KdlOptions::dumpLexerTokens)
		lex.dumpTokens();

	
	kdl::Combinator parse(lex.getTokens());
	if (!parse.parse())
		printf("errrn\n");

	if (KdlOptions::dumpBlocks)
		parse.dumpBlocks();

	m_rules.resize(parse.getBlockList().size());
	for (auto& i : parse.getBlockList())
	{
		RuleParser rp(i, *m_rules.add());
		rp.parse();
	} 



	ConditionalReferenceValidator crv(m_rules);
	if (!crv.check())
		printf("err0r\n");

	ClangCodeGen clcg(m_currentFile, m_rules);

	clcg.generate();

	for (const auto& [file, src] : clcg.getFileMap())
		m_files[file] = src->str();

	return true;
}

void Compiler::writeFiles(const std::string& directory)
{
	for (const auto& [file, src] : m_files)
	{
	
		writeFile(directory + "/" + file, src);
			
	}
}

void Compiler::writeFile(const std::string& name, const std::string& source)
{
	std::ofstream out(name, std::ios::out);
	
	if (!out.is_open())
	{
		printf("Failed to open %s for writing!\n", name.c_str());
		return;
	}

	if (KdlOptions::verbose)
		printf("Writing to %s\n", name.c_str());
	
	out.write(source.c_str(), source.length());

}
