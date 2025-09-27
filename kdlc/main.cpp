#include <stdio.h>
#include "parser/Lexer.h"
#include "context/CompilerContext.h"
int main(int argc, char** argv)
{
    std::string test = R"kdl(
rule SimpleEICARTest {
    evt_source: process;
    condition:
        process.file.contains("EICAR-STANDARD-ANTIVIRUS-TEST-FILE");

}

)kdl";
    kdl::FileID id= SRC_MGR.addRawSource(test);
    kdl::Lexer l{ id };
    l.scan();
    l.dumpTokens();
    return 0;
}