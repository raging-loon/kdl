#include "backend/clang/CLangCG.h"
#include "backend/clang/ClRuleCompiler.h"
using kdl::ClangCodeGen;
using kdl::RulePool;
using kdl::ClRuleCompiler;

ClangCodeGen::ClangCodeGen(const char* baseFileName, RulePool& rp)
	: m_rules(rp)
{
	std::string fileName(baseFileName);

	fileName = fileName.substr(0, fileName.rfind("."));

	m_fileMap[fileName + ".c"] = &m_output;
	m_fileMap[fileName + ".h"] = &m_header;
	
	m_output << "// THIS FILE WAS AUTOMATICALLY GENERATED BY KDL\n"
		        "// KDL is licensed under GPLv3\n\n"
		        "#include \"" << fileName + ".h" <<  "\"\n";
}
/*
#ifdef KSR_COMPILE_AS_DYNLIB
#ifndef KSR_API
# ifdef _MSC_VER
#endif // _MSC_VER

#endif // KSR_API

*/
bool ClangCodeGen::generate()
{
	m_header << "// THIS FILE WAS AUTOMATICALLY GENERATED BY KDL\n"
		        "// KDL is licensed under GPLv3.\n\n"
		        "#ifdef KSR_COMPILE_AS_DYNLIB\n"
		        "# ifndef KSR_API\n"
		        "#  if defined(_MSC_VER)\n"
		        "#   define KSR_API __declspec(dllexport)\n"
		        "#  elif defined(__GNUC__)\n"
		        "#   define KSR_API __attribute__((visibility(\"default\")))\n"
		        "#  else\n#    error \"Unsupported compiler!\"\n"
				"#  endif\n# endif\n"
				"#else\n# define KSR_API\n#endif // KSR_COMPILE_AS_DYNLIB\n\n";

	
	m_header << "\n\n"
		        "#ifdef __cplusplus\n"
		        "extern \"C\"{\n"
				"#endif // __cplusplus\n\n\n";

	m_header << "#include <stdbool.h>\n\n";

	m_output << "#include \"libdetutil/string.h\"\n\n";


	for(int i = 0; i < m_rules.size(); i++)
	{
		ClRuleCompiler clrc(*m_rules[i]);
		m_output << clrc.compile().str();
		m_header << "KSR_API " << clrc.getFunctionPrototype() << "\n";
	}
	


	m_header << "\n\n"
		"#ifdef __cplusplus\n"
		"}\n"
		"#endif // __cplusplus\n";


	m_header << "// End of auto-generated header file";

	return true;
}

