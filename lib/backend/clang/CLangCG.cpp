#include "backend/clang/CLangCG.h"
#include "backend/clang/ClRuleCompiler.h"
using kdl::ClangCodeGen;
using kdl::RulePool;
using kdl::ClRuleCompiler;

ClangCodeGen::ClangCodeGen(RulePool& rp)
	: m_rules(rp)
{
}

bool ClangCodeGen::generate()
{
	for(int i = 0; i < m_rules.size(); i++)
	{
		ClRuleCompiler clrc(*m_rules[i]);
		m_output << clrc.compile().str();
	}
	return false;
}

