#include <stdio.h>
#include "parser/Lexer.h"
#include "context/CompilerContext.h"
#include "parser/Parser.h"
#include "parser/Token.h"

int main(int argc, char** argv)
{
    std::string test = R"kdl(
#rule OfficeMacro {
#	evt_source: process;
#	predicate:
#        $ms_products = [
#            "winword.exe", "excel.exe", "powerpnt.exe", "outlook.exe"
#        ];
#     
#        $cmd_processors = [
#            "powershell.exe", "pwsh.exe", "cmd.exe", "cscript.exe", "wscript.exe"
#        ];
#
#        $c1 = process.parent.name in $ms_products;
#        $c2 = process.name in $cmd_processors;
#}

rule LSASSRead
{
    evt_source: memory;
    predicate:
        $c1 = dest_process.name == 'lsass.exe';
        $c2 = source_process.name not in ["wininit.exe", "task_mgr.exe"];
    condition:
        $c1 and $c2
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