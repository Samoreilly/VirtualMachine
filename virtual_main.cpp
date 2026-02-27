
#include "virtual_main.h"
#include <iostream>
#include <stdexcept>


void Virtual::add() {

    if(cpu.pc >= cpu.sp) {
        throw std::runtime_error("Stack Overflow");
    }

    int valA = cpu.stack[cpu.sp];
    int valB = cpu.stack[cpu.sp + 1];

    cpu.sp++;

    cpu.stack[cpu.sp] = (valA + valB);

}

void Virtual::push(int reg, int value) {
    
}

//load into register
void Virtual::load(int reg, int value) {
    
    cpu.reg[reg] = value;
}

int Virtual::init_vm() {

    for(;;) {
        
        //get current operation
        Instruction opcode = static_cast<Instruction>(cpu.stack[cpu.pc++]);
    
        switch (opcode) {
            
            case Instruction::ADD:
                add();  
                break;

            case Instruction::SUB:

                break;

            case Instruction::MUL:

                break;

            case Instruction::DIV:

                break;

            case Instruction::LOAD: {

                int reg_idx = cpu.stack[cpu.pc++];
                int value_to_load = cpu.stack[cpu.pc++];

                load(reg_idx, value_to_load);

                break;
            }
            
            case Instruction::PUSH: {

                int reg_idx = cpu.stack[cpu.pc++];
                int value_at_reg = cpu.reg[reg_idx];

                cpu.stack[cpu.sp--] = value_at_reg;

                break;
            }
        };
        
    }

}

int main(void) {

    Virtual vm;
    std::cout << "\nStarting VirtualMachine\n\n";

    vm.init_vm();

    return 0;

}
