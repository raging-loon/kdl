#ifndef CLI_KDL_OPTIONS_H_
#define CLI_KDL_OPTIONS_H_


namespace kdl
{

class KdlOptions
{
public:


	static bool verbose;
	static bool debug;

	static bool dumpCTrees;
	static bool dumpLexerTokens;
	static bool dumpBlocks;
	static bool dumpRules;
};

#define DEBUG_PRINT(...) if(kdl::KdlOptions::debug) { printf(##__VA_ARGS__);}
#define IF_DEBUG(...) if(kdl::KdlOptions::debug) { __VAR_ARGS__; }

} // kdl


#endif // CLI_KDL_OPTIONS_H_