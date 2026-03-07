
#include "virtual_main.h"
#include "../compiler/ast/parser.h"
#include <fstream>
#include <iostream>
#include <stdexcept>
#include "../compiler/lexer/lexer.h"
#include "Assembler.h"

//NOTE: to comment blocks its VISUAL MODE -> Select lines -> G -> B
#include "../compiler/codegen/code_generator.h"
#include <chrono>

void Virtual::add(Instruction opcode) {

    if(opcode == Instruction::ADD_STACK) {
   
        if(cpu.pc >= cpu.sp) {
            throw std::runtime_error("Stack Overflow");
        }

        int valA = cpu.stack[cpu.sp + 1];
        int valB = cpu.stack[cpu.sp + 2];

        cpu.stack[cpu.sp + 2] = (valA + valB);

        cpu.last_value = cpu.stack[cpu.sp + 2];
            
        cpu.stack[cpu.sp + 1] = 0;
        cpu.sp++;
    
    }else {
        //register addtion - expects two register
        // e.g. ADD_REG, 0, 1 where those numbers are registers

        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] += cpu.reg[second_reg];
        
        cpu.last_value = cpu.reg[first_reg];
    }
}

void Virtual::mul(Instruction opcode) {

    if(opcode == MUL_STACK) {

        if(cpu.pc >= cpu.sp) {
            throw std::runtime_error("Stack Overflow");
        }
        
        cpu.stack[cpu.sp + 2] *= cpu.stack[cpu.sp + 1];
        
        cpu.last_value = cpu.stack[cpu.sp + 2];

        cpu.stack[cpu.sp + 1] = 0; 
        cpu.sp++;
    
    }else {

        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];
        
        cpu.reg[first_reg] *= cpu.reg[second_reg];
        
        cpu.last_value = cpu.reg[first_reg];
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
     


        cpu.stack[cpu.sp + 2] = firstVal - secVal;
        cpu.stack[cpu.sp + 1] = 0;

        cpu.last_value = cpu.stack[cpu.sp + 2];
        

        cpu.sp++;

    }else {
    
        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] -= cpu.reg[second_reg];
        
        cpu.last_value = cpu.reg[first_reg];

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
        
        cpu.last_value = cpu.stack[cpu.sp + 2];

        cpu.stack[cpu.sp + 1] = 0; 
        cpu.sp++;

    }else {

        int first_reg = cpu.stack[cpu.pc++];
        int second_reg = cpu.stack[cpu.pc++];

        cpu.reg[first_reg] /= cpu.reg[second_reg];

        cpu.last_value = cpu.reg[first_reg];

    }
}


void Virtual::printStack() {



    for(int i = MEM_SIZE - 1;i >= MEM_SIZE - 47;i--) {
        std::cout << cpu.stack[i] << "\n";
    }
}

void Virtual::push(int return_address) {

    //push frame pointer onto stack
    if(return_address != -1) {
        cpu.stack[cpu.sp--] = return_address;
        return;
    }

    if(cpu.pc >= cpu.sp) throw std::runtime_error("Stack Overflow");
        
    int reg_idx = cpu.stack[cpu.pc++];
    int value_at_reg = cpu.reg[reg_idx];

    cpu.stack[cpu.sp--] = value_at_reg;
    cpu.last_value = value_at_reg;

}

int Virtual::pop(Instruction opcode) {

    if(opcode == Instruction::POP_REG) {

        if(cpu.sp >= MEM_SIZE - 1) throw std::runtime_error("Stack Underflow");
        
        int reg_idx = cpu.stack[cpu.pc++];
        
        //get the value to be popped
        int value = cpu.stack[++cpu.sp];


        cpu.reg[reg_idx] = value;
        return -1;
    }else {

        if(cpu.sp >= MEM_SIZE - 1) {
            throw std::runtime_error("Stack underflow");
        }
        //values only needed when RET is called and address is needed
        return cpu.stack[++cpu.sp];
    }


}

//load into register
void Virtual::load(int reg, int value) {
    
    cpu.reg[reg] = value;
}

int Virtual::init_vm() {

    try{

        while(cpu.is_running) {
            
           // std::cout << "PC: " << cpu.pc << " SP: " << cpu.sp << " FP: " << cpu.reg[5] << "\n\n";
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

                    push();
                    break;
                }

                //pops off stack and moves into register
                case Instruction::POP_REG:
                case Instruction::POP_NOREG: {

                    pop(opcode);

                    break;
                }

                case Instruction::JNZ: {
                    int dest = cpu.stack[cpu.pc++];

                    if(cpu.last_value != 0) {
                        cpu.pc = dest;
                    }

                    break;
                }

                case Instruction::JZ:
                case Instruction::JUMP: {

                    if(opcode == Instruction::JUMP) {
                        int dest = cpu.stack[cpu.pc++];
                        cpu.pc = dest;

                    }else {
                        
                        int dest = cpu.stack[cpu.pc++];

                        //if not zero, pc keeps moving forward
                        if(cpu.last_value == 0) {
                            cpu.pc = dest;
                        }
                    }

                    break;
                
                }

                //CMP RO, R1 | This is later used in any jump statement to evaluate last result
                case Instruction::CMP: {
                    int reg_one = cpu.stack[cpu.pc++];
                    int reg_two = cpu.stack[cpu.pc++];

                    cpu.last_value = cpu.reg[reg_one] - cpu.reg[reg_two];
                    
                    break;
                }
                
                case Instruction::CALL: {
                    
                    int return_address = cpu.stack[cpu.pc++];
                 
                    //push frame pointer (start of stack frame)
                    push(cpu.pc);
                    push(cpu.reg[5]);

                    cpu.reg[5] = cpu.sp;

                    //jump to label
                    cpu.pc = return_address;

                    break;                
                }
                
                case Instruction::RET: {

                    cpu.sp = cpu.reg[5];

                    cpu.reg[5] = pop(opcode);

                    int return_address = pop(opcode);
                    cpu.pc = return_address;
                

                    break;
                }
                
                //moves the RIGHT reg into LEFT reg e.g. MOV R0, R1
                case Instruction::MOV: {

                    int reg_one = cpu.stack[cpu.pc++];
                    int reg_two = cpu.stack[cpu.pc++];

                    cpu.reg[reg_one] = cpu.reg[reg_two];

                    break;

                }

                case Instruction::STORE_REG_OFF: {
                    int base_reg = cpu.stack[cpu.pc++];
                    int8_t offset = (int8_t)cpu.stack[cpu.pc++];
                    int src_reg = cpu.stack[cpu.pc++];

                    cpu.stack[cpu.reg[base_reg] - offset] = cpu.reg[src_reg];
                    break;
                }

                case Instruction::LOAD_REG_OFF: {
                    int target_reg = cpu.stack[cpu.pc++];
                    int base_reg = cpu.stack[cpu.pc++];
                    int8_t offset = (int8_t)cpu.stack[cpu.pc++];

                    cpu.reg[target_reg] = cpu.stack[cpu.reg[base_reg] - offset];
                    break;
                }

                case Instruction::PRINT_REG:
                case Instruction::PRINT:
        
                    if(opcode == Instruction::PRINT) {
                        printStack();

                    }else {
                        int reg_index = cpu.stack[cpu.pc++];

                        int reg_value = cpu.reg[reg_index];

                        std::cout << reg_value << "\n";
                    }

                    break;
                
                default: throw std::runtime_error("NO OPCODE FOUND" + std::to_string(opcode));
            };
        }

    }catch(const std::runtime_error& e) {
        std::cout << "Runtime Error: " << e.what();
    }


    return 1;

}

int main(void) {

    //soon .asm file wont be needed and it will be produced by the compiler
    // const std::string file_name = "data.asm";
    // Assembler asmblr{file_name};
    // 
    // asmblr.lexer();
    //

    std::ifstream code_file("code.sam");
    Lexer lexer{code_file};

    auto compile_start = std::chrono::high_resolution_clock::now();
    
    lexer.lex();
    const std::vector<Token>& tt = lexer.get_tokens();
    Parser p{tt};
    p.construct_node();

    CodeGenerator gen;
    if (p.get_root()) {
        p.get_root()->generate(gen);
    }

    Assembler asmblr{"", gen.get_bytecode()};
    asmblr.lexer();
    auto compile_end = std::chrono::high_resolution_clock::now();

    Virtual vm{asmblr.get_tokens(), asmblr.get_tokens().size()};

    auto run_start = std::chrono::high_resolution_clock::now();
    vm.init_vm();
    auto run_end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> compile_time = compile_end - compile_start;
    std::chrono::duration<double, std::milli> run_time = run_end - run_start;

    std::cout << "\n------------------------------\n";
    std::cout << "Compile Time: " << compile_time.count() << " ms\n";
    std::cout << "Run Time:     " << run_time.count() << " ms\n";
    std::cout << "------------------------------\n";
    



    // std::vector<uint8_t> tokens = asmblr.get_tokens();


    // Virtual vm{tokens, tokens.size()};

    // vm.init_vm();

    return 0;

}
