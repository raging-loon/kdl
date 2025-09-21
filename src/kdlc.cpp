#include <cstdio>

#include "cli/TerminalInterface.h"

int main(int argc, char** argv)
{
    kdl::TerminalInterface t(argc, argv);

    return t.run();
}