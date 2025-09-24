#include <stdio.h>
#include "parser/lexer/Lexer.h"

int main(int argc, char** argv)
{
    printf("hello\n");

    std::string test = R"kdl(
# this is a comment
# ============
[][]{}[]{} # this is a comnet ****
[]{}()!=
# hello
)kdl";

    kdl::Lexer l{ test };
    l.scan();
    l.dumpTokens();
    return 0;
}