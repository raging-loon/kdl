#ifndef BACK_END_CODE_GENERATOR_H_
#define BACK_END_CODE_GENERATOR_H_

#include "channels/RulePool.h"
#include <string>
namespace kdl
{


class CodeGenerator
{
public:
	
	virtual bool generate() = 0;
	virtual const std::stringstream& getOutput() = 0;

};


} // kdl


#endif // BACK_END_CODE_GENERATOR_H_