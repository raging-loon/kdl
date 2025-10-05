#include <stdio.h>
#include "parser/Lexer.h"
#include "context/CompilerContext.h"
#include "parser/Parser.h"
#include "parser/Token.h"

int main(int argc, char** argv)
{
    std::string test = R"kdl(
rule OfficeMacro {
	evt_source: process;
	predicate:
        $test = "windows";
        $test_bool = not 1;
        $test_bool = 1 + 1 * 2 / 1;
        $test2 = 1 == 1;
}


)kdl";
    kdl::FileID id= SRC_MGR.addRawSource(test);
    kdl::Lexer l{ id };
    l.scan();
    l.dumpTokens();

    kdl::Parser p{ l.getTokens(), id };

    p.parse();
    return 0;
}