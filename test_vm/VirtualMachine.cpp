#include "VirtualMachine.h"

#include <iostream>


VirtualMachine::VirtualMachine()
    : m_registers{ 0 },
    m_code{}
{

}

void VirtualMachine::loadCode(
    const std::vector<uint8_t>& code
)
{
    m_code.insert(
        m_code.cend(),
        code.cbegin(),
        code.cend()
    );
}


void VirtualMachine::run()
{
    std::cout << "Running " << m_code.size() << " bytes\n";
}