// libkdl.cpp : Defines the entry point for the application.
//

#include "libkdl.h"
#include "parser/lex.h"
#include "parser/Combinator.h"

#include <fstream>
#include <string>
#include <sstream>
#include <filesystem>
#include "parser/ErrorPrinter.h"
int main()
{
	std::ifstream in("../../../tests/test.kdl");
	//buf << in.rdbuf();

	auto sz = std::filesystem::file_size("../../../tests/test.kdl");

	std::string buf(sz, 0);
	in.read((char*)buf.c_str(), sz);
	
	kdl::ErrorPrinter::setCurrentSource("tests/test.kdl", buf.c_str(), buf.length());

	kdl::Lexer lex("tests/test.kdl", buf.c_str(), buf.length());

	if (lex.scan() < 0)
		return 1;
	kdl::Combinator parse(lex.getTokens());
	if (!parse.parse())
		printf("errrn\n");

	return 0;
}
