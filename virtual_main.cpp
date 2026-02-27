
#include "virtual_main.h"
#include <iostream>
#include <stdexcept>


void Virtual::add(Instruction opcode) {

    if(opcode == Instruction::ADD_STACK) {
   
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
    
    }else {
        //register addtion - expects two register
        // e.g. ADD_REG, 0, 1 where those numbers are registers

        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] += cpu.reg[second_reg];

    }
}

void Virtual::mul(Instruction opcode) {

    if(opcode == MUL_STACK) {

        if(cpu.pc >= cpu.sp) {
            throw std::runtime_error("Stack Overflow");
        }
        
        std::cout << "Before multiplication on stack: " << "\n";
        std::cout << "First value" << cpu.stack[cpu.sp + 2] << "\n";
        std::cout << "Second value" << cpu.stack[cpu.sp + 1] << "\n";

        cpu.stack[cpu.sp + 2] *= cpu.stack[cpu.sp + 1];
        std::cout << "After: " << cpu.stack[cpu.sp + 2];

        cpu.sp++;
    
    }else {
        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] *= cpu.reg[second_reg];

    }
}

void Virtual::sub(Instruction opcode) {

    if(opcode == Instruction::SUB_STACK) {
         
        if(cpu.pc >= cpu.sp) {
            throw std::runtime_error("Stack Overflow");
        }

        //stack grows down so larger stack index is first value
        int firstVal = cpu.stack[cpu.sp + 2];
        int secVal = cpu.stack[cpu.sp + 1];
     
        std::cout << "First value: " << firstVal << "Second value: " << secVal << "\n";

        cpu.stack[cpu.sp + 2] = firstVal - secVal;
        std::cout << "Subtraction: " << cpu.stack[cpu.sp + 2];
        cpu.sp++;

    }else {
    
        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] -= cpu.reg[second_reg];
    }

}

void Virtual::div(Instruction opcode) {

    if(opcode == Instruction::DIV_STACK) {

        if(cpu.pc >= cpu.sp) {
            throw std::runtime_error("Stack Overflow");
        }

        int firstVal = cpu.stack[cpu.sp + 2];
        int secondVal = cpu.stack[cpu.sp + 1];

        if(secondVal == 0) {
            throw std::runtime_error("Dividing by 0 is undefined behaviour");
        }

        cpu.stack[cpu.sp + 2] = firstVal / secondVal;
        cpu.sp++;

    }else {

        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];
    
        cpu.reg[first_reg] /= cpu.reg[second_reg];
    
    }
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
 
            case Instruction::ADD_REG:
            case Instruction::ADD_STACK:
 
                add(opcode);
  
                break;

            case Instruction::SUB_REG:
            case Instruction::SUB_STACK: {
                
                sub(opcode);

                break;
            }

            case Instruction::MUL_REG:
            case Instruction::MUL_STACK:
                
                mul(opcode);

                break;
            
            case Instruction::DIV_REG:
            case Instruction::DIV_STACK: {
                
                div(opcode);
    
                break;
            }

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

            //pops off stack and moves into register
            case Instruction::POP: {

                int reg_idx = cpu.stack[cpu.pc++];
                int value = cpu.stack[++cpu.sp];

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
    
    int program[] = {
        // Program data
        0xB0, 0, 50,      // load 50 into register 0
        0xB0, 1, 20,      // load 20 into register 1 
        
        0x01, 0, 1,       // add registers 0 and 1
        
        0xB1, 0,          // push reg 0 onto stack
        
        0xB0, 0, 10,      // load value 10 into reg 0
        0xB1, 0,          //push onto stack
        
        0x02,             //subtraction on stack
        
        0xFA,             // print
        0xB2, 0,          //pop
        0xFF              //halt program
    };

    constexpr std::size_t size = sizeof(program) / sizeof(program[0]);

    Virtual vm(program, size);

    vm.init_vm();

    return 0;

}
