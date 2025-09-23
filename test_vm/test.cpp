#include <iostream>

#include "VirtualMachine.h"

int main()
{

    VirtualMachine vm{};


    std::vector<uint8_t> code = {

    };

    vm.loadCode(code);

    vm.run();

    return 0;
}