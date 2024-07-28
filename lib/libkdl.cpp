// libkdl.cpp : Defines the entry point for the application.
//

#include "libkdl.h"
#include "parser/lex.h"
#include "parser/Combinator.h"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include "parser/CompilerMessage.h"
int main()
{
	std::ifstream in("../../../tests/test.kdl");
	//buf << in.rdbuf();

	auto sz = std::filesystem::file_size("../../../tests/test.kdl");

	std::string buf(sz, 0);
	in.read((char*)buf.c_str(), sz);
	
	kdl::CompilerMessage::setSource(buf.c_str(), "tests/test.kdl", buf.length());
	kdl::Lexer lex(buf.c_str(), buf.length());

	if (lex.scan() < 0)
		return 1;
	kdl::Combinator parse(lex.getTokens());
	if (!parse.parse())
		printf("errrn\n");

	return 0;
}
