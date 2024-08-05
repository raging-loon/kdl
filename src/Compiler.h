#ifndef COMPILER_H_
#define COMPILER_H_
#include <filesystem>

#include "parser/parser.h"
#include "parser/lex.h"
#include "channels/RulePool.h"
#include "channels/rule.h"
#include "conditional/ConditionalParser.h"
namespace kdl
{



///
/// PURPOSE
///		Manages Compilation
///		
///		Pipeline goes:
///		
///		+-+-+-+-+-+-+-+-+
///		|     Lexer     |	src/parser/lex.h
///		+-+-+-+-+-+-+-+-+
///			    ↓
///		+-+-+-+-+-+-+-+-+
///		|   Combinator  |	src/parser/Combinator.h
///		+-+-+-+-+-+-+-+-+
///			    ↓
///		+-+-+-+-+-+-+-+-+
///		|  Rule Parser  |	src/parser/RuleParser.h
///		+-+-+-+-+-+-+-+-+
///			    ↓
///		+-+-+-+-+-+-+-+-+
///		|   CTree Gen   |	src/conditional/ConditionalParser.h
///		+-+-+-+-+-+-+-+-+
///			    ↓
///		+-+-+-+-+-+-+-+-+
///		|   Reference   |	src/conditional/ConditionalReferenceValidator.h
///     |   Validator   |
///		+-+-+-+-+-+-+-+-+
///			    ↓
///		+-+-+-+-+-+-+-+-+
///		|    Code Gen   |	src/backend/<generator>/<generator-code>.h
///		+-+-+-+-+-+-+-+-+
///
class Compiler
{
public:
	Compiler();
	~Compiler();

	bool compileFile(const char* filename);
	bool compileSource(const char* source, int len);

	void writeFiles(const std::string&  directory);

private:
	RulePool m_rules;

	std::unordered_map<std::string, std::string> m_files;

	void writeFile(const std::string& name, const std::string& source);
	const char* m_currentFile;
};


} // kdl


#endif // COMPILER_H_