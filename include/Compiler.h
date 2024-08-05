#ifndef COMPILER_H_
#define COMPILER_H_

#include "parser/parser.h"
#include "parser/lex.h"
#include "channels/RulePool.h"
#include "channels/rule.h"
#include "conditional/ConditionalParser.h"

#include "libkdl.h"
namespace kdl
{

class Compiler
{
public:
	Compiler();
	~Compiler();

	bool compileFile(const char* filename);
	bool compileSource(const char* source, int len);

	void writeFiles(const char* directory);

private:
	RulePool m_rules;

	std::unordered_map<std::string, std::string> m_files;

	void writeFile(const std::string& name, const std::string& source);
};


} // kdl


#endif // COMPILER_H_