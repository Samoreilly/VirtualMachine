
#include "virtual_main.h"
#include <iostream>
#include <stdexcept>


void Virtual::add() {

    if(cpu.pc >= cpu.sp) {
        throw std::runtime_error("Stack Overflow");
    }

    int valA = cpu.stack[cpu.sp + 1];
    std::cout << "Value here: " << valA << "\n";

    int valB = cpu.stack[cpu.sp + 2];
    std::cout << "Value above: " << valB << "\n";

    cpu.stack[cpu.sp + 2] = (valA + valB);

    std::cout << "Added value sp: " << cpu.sp + 2 << "\n";
        
    cpu.stack[cpu.sp + 1] = 0;
    cpu.sp++;
    
    std::cout << "Printed add value prior: " << cpu.stack[cpu.sp] << "\n";
    std::cout << "Printed add value: " << cpu.stack[cpu.sp + 1] << "\n";

}

void Virtual::mul() {


}


void Virtual::printStack() {

    std::cout << "Print stack\n";

    for(int i = MEM_SIZE - 1;i > MEM_SIZE - 10;i--) {
        std::cout << cpu.stack[i] << "\n";
    }
}

void Virtual::push(int reg, int value) {
    
}

//load into register
void Virtual::load(int reg, int value) {
    
    cpu.reg[reg] = value;
}

int Virtual::init_vm() {

    while(cpu.is_running) {
        
        std::cout << "PC: " << cpu.pc << " SP: " << cpu.sp << "\n\n";
        //get current operation
        Instruction opcode = static_cast<Instruction>(cpu.stack[cpu.pc++]);
    
        switch (opcode) {

            case Instruction::HALT:
                cpu.is_running = false;
                break;

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

            case Instruction::POP: {

                int reg_idx = cpu.stack[cpu.pc];

                int value = cpu.stack[++cpu.sp];
                cpu.stack[cpu.sp] = 0;//remove value
                
                std::cout << "Popped value: " << value << "\n";
                cpu.reg[reg_idx] = value;

                break;
            }

            case Instruction::PRINT:
                printStack();
                break;
            
            default: throw std::runtime_error("NO OPCODE FOUND" + std::to_string(opcode));
        };
        
    }

    return 1;

}

int main(void) {
    

    std::cout << "\nStarting VirtualMachine\n\n";
    
    //Loads, push 15 and 20 seperately and adds them
    int program[] = {0x04, 0, 15, 0x05, 0, 0x04, 0, 20, 0x05, 0, 0x00, 0xFA, 0x06, 0xFA,
       0x04, 0, 15, 0x05, 0, 0xFA, 0xFF, 0xFF, 0xFF, 0xFF
    };
    
    constexpr std::size_t size = sizeof(program) / sizeof(program[0]);

    Virtual vm(program, size);

    vm.init_vm();

    return 0;

}
