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
#include <iostream>
#include <iomanip>
#include <filesystem>

using kdl::Compiler;

static void strtohex(std::string& str)
{
	std::stringstream ss;

	for (unsigned char i : str)
		ss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(i);

	str = ss.str();
}

static void formatTableEntry(std::string& str, bool isHex, int maxlen)
{
	if (isHex)
		strtohex(str);

	if (str.length() > maxlen)
		str = str.substr(0, maxlen - 3) + "...";

}

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
		m_outputFiles[file] = src->str();

	return true;
}

void Compiler::writeFiles(const std::string& directory)
{
	for (const auto& [file, src] : m_outputFiles)
	{
	
		writeFile(directory + "/" + file, src);
			
	}
}

void Compiler::dumpRules()
{

	for (int i = 0; i < m_rules.size(); i++)
	{
		auto& rule = *m_rules[i];
		auto start = rule.cbegin();
		auto end =   rule.cend();
		
		std::cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n"
				  << "|  " << std::left << std::setw(57) << rule.getName() << "|\n" 
			      << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";
		std::string value;
		std::string varname;

		while (start != end)
		{
			value = start->second.searchstr;
			varname = start->first;
			formatTableEntry(value, start->second.type == Variable::BYTE_SEQUENCE, 29);
			formatTableEntry(varname, false, 10);


			std::cout << "|  " << std::left << std::setw(10) << varname
				<< "|  " << std::left << std::setw(12) 
				<< (start->second.type == Variable::BYTE_SEQUENCE ? "BYTE_SEQ" : "STRING")
				<< "|  " << std::left << std::setw(29) << value << "|\n";

			++start;
		}
		std::cout << "+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n";

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
