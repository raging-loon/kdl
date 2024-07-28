#ifndef CONDITIONAL_CONDITIONAL_SEMANTIC_CHECKER_H_
#define CONDITIONAL_CONDITIONAL_SEMANTIC_CHECKER_H_

#include "parser/parser.h"

namespace kdl
{

class ConditionalSemanticChecker
{
public:

	static bool isValidSubCondition(CTokenPtr op, CTokenPtr left, CTokenPtr right);

	static bool isValid_CND_OF_Operation(CTokenPtr left, CTokenPtr right);
	
	static bool isValidRelationalOperation(CTokenPtr left, CTokenPtr right);
private:

	static bool error(CTokenPtr bad, const char* message);
};

} // kdl


#endif // CONDITIONAL_CONDITIONAL_SEMANTIC_CHECKER_H_