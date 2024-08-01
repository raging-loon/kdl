#ifndef BACKEND_CLANG_CLANG_CG_H_
#define BACKEND_CLANG_CLANG_CG_H_

#include <sstream>

#include "backend/CodeGenerator.h"
#include "channels/RulePool.h"
namespace kdl
{

class ClangCodeGen : public CodeGenerator
{
public:
	ClangCodeGen(RulePool& rp);

	bool generate() override;
	const std::stringstream& getOutput() override
	{
		return m_output;
	}

private:

	

	RulePool& m_rules;

	std::stringstream m_output;

};

} // kdl

#endif // BACKEND_CLANG_CLANG_CG_H_