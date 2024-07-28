#ifndef COMPILER_H_
#define COMPILER_H_

#include "parser/parser.h"
#include "parser/lex.h"
#include "channels/RulePool.h"
#include "channels/rule.h"
#include "conditional/ConditionalParser.h"


namespace kdl
{

class Compiler
{
public:
	Compiler();
	~Compiler();

	bool compileFile(const char* filename);
	bool compileSource(const char* source);


private:
	RulePool m_rules;


};


} // kdl


#endif // COMPILER_H_