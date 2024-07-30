﻿// libkdl.cpp : Defines the entry point for the application.
//

#include "libkdl.h"
#include "parser/lex.h"
#include "parser/Combinator.h"

#include "Compiler.h"
int main()
{

	kdl::Compiler c;
	
	c.compileFile("../../../tests/test.kdl");

	return 0;
}