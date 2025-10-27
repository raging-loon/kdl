#include <antlr4-runtime.h>
#include "generated/KDLGrammarLexer.h"
#include "generated/KDLGrammarParser.h"
#include <string>
#include <iostream>

#include "ast/ASTBuilder.h"
#include "ast/ASTDumper.h"

#include "context/CompilerContext.h"
#include "intermediate/SymbolExtractor.h"
#include "intermediate/SemanticAnalyzer.h"
using namespace antlr4;
int main()
{
    std::string test = R"kdl(
rule OfficeMacro {
	evt_source: process;
	predicate:
#        ms_products = [
#            "winword.exe", "excel.exe", "powerpnt.exe", "outlook.exe"
#        ];
#     
#        cmd_processors = [
#            "powershell.exe", "pwsh.exe", "cmd.exe", "cscript.exe", "wscript.exe"
#        ];
#
#        c1 = "powershell.exe" in ms_products;
#        c2 = "winword.exe" in cmd_processors;

        test1 = 1;
        test3 = 400;
        test4 = test1 == 2;
        test5 = test4 == test3;
    
    condition:
        c1 and c2

    action:
        kill_process;
}

)kdl";

    kdl::FileID id = SRC_MGR.addRawSource(test);


    ANTLRInputStream input(test);

    kdl_gen::KDLGrammarLexer lexer(&input);

    CommonTokenStream tokens(&lexer);

    kdl_gen::KDLGrammarParser parser(&tokens);
    kdl::ASTBuilder builder{};


    tree::ParseTree* tree = parser.startRule();
    builder.visit(tree);
    kdl::ASTDumper dumper{};
    dumper.dump(builder.getASTRoot());
    kdl::SymbolExtractor sym{};

    auto rules = sym.extractRules(builder.getASTRoot());

    kdl::SemanticAnalyzer sema{};

    sema.analyzeRule(rules[0]);
}