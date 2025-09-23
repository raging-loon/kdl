#ifndef TEST_VM_VIRTUAL_MACHINE_H_
#define TEST_VM_VIRTUAL_MACHINE_H_

#include <array>
#include <vector>

enum Register
{
    REG_R0,
    REG_R1,
    REG_R2,
    REG_R3,
    REG_PC,
    REG_COUNT
};


class VirtualMachine
{
public:

    VirtualMachine();
    ~VirtualMachine() = default;

    void loadCode(const std::vector<uint8_t>& code);

    void run();

private:




private:

    std::array<uint32_t, Register::REG_COUNT> m_registers;
    
    std::vector<uint8_t> m_code;


};

#endif // TEST_VM_VIRTUAL_MACHINE_H_