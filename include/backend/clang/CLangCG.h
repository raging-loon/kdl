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
	ClangCodeGen(const char* baseFileName, RulePool& rp);

	bool generate() override;
	const std::stringstream& getOutput() override
	{
		return m_output;
	}

	const auto& getFileMap() const { return m_fileMap; }

private:

	std::unordered_map<std::string, std::stringstream*> m_fileMap;

	RulePool& m_rules;

	std::stringstream m_output;
	std::stringstream m_header;


};

} // kdl

#endif // BACKEND_CLANG_CLANG_CG_H_