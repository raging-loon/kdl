#include <antlr4-runtime.h>
#include "generated/KDLGrammarLexer.h"
#include "generated/KDLGrammarParser.h"
#include <string>
#include <iostream>

#include "ast/ASTBuilder.h"

using namespace antlr4;
int main()
{
    std::string test = R"kdl(
rule OfficeMacro {
	evt_source: process;
	predicate:
        ms_products = [
            "winword.exe", "excel.exe", "powerpnt.exe", "outlook.exe"
        ];
     
        cmd_processors = [
            "powershell.exe", "pwsh.exe", "cmd.exe", "cscript.exe", "wscript.exe"
        ];

        c1 = process.parent.name in ms_products;
        c2 = process.name in cmd_processors;
    
    condition:
        c1 and c2

    action:
        kill_process;
}
rule scheduled_task_persistance {
	evt_source: sched_task_create;
	
	predicate:
		c1 = process.parent.name not in ["taskschd.msc", "svchost.exe"];
		c2 = task.name != "Microsoft\\Windows\\Defrag\\ScheduledDefrag";		
    
    condition:
        c1

	action: remove_task, kill_process;
}
)kdl";

    ANTLRInputStream input(test);

    kdl_gen::KDLGrammarLexer lexer(&input);

    CommonTokenStream tokens(&lexer);

    kdl_gen::KDLGrammarParser parser(&tokens);
    kdl::ASTBuilder builder{};

    
    tree::ParseTree* tree = parser.startRule();
    builder.visit(tree);

    std::cout << '\n' << tree->toStringTree(&parser, true) << '\n';
}