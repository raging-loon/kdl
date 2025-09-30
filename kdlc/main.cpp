#include <stdio.h>
#include "parser/Lexer.h"
#include "context/CompilerContext.h"
int main(int argc, char** argv)
{
    std::string test = R"kdl(
rule OfficeMacro {
	evt_source: process;
	
	predicate:
		ms_products = [
			"winword.exe", "excel.exe", "powerpnt.exe", "outlook.exe"
		];
		
		cmd_processors = [
			"powershell.exe", "pwsh.exe", "cmd.exe", "wscript.exe", "cscript.exe"
		];
		
		c1 = process.parent.name in ms_products;
		c2 = process.name in cmd_processors;
		
	condition:
		c1 and c2;

	action:
		kill_process;
}


)kdl";
    kdl::FileID id= SRC_MGR.addRawSource(test);
    kdl::Lexer l{ id };
    l.scan();
    l.dumpTokens();
    return 0;
}