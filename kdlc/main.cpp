#include <stdio.h>
#include "parser/lexer/Lexer.h"
#include "context/CompilerContext.h"
int main(int argc, char** argv)
{
    printf("hello\n");

    std::string test = R"kdl(
# this is a comment
# ============
[][]{}[]{} # this is a comnet ****
[]{}()!="hello"
# hello
)kdl";
    kdl::FileID id= SRC_MGR.addRawSource(test);
    kdl::Lexer l{ id };
    l.scan();
    l.dumpTokens();
    return 0;
}