#pragma once

#include <cstdint>
#include <type_traits>

constexpr int MEM_SIZE = 4096;

typedef uint16_t Reg;


//corresponds to registers
enum Instruction : int {
       
    //operands
    ADD_STACK = 0x00,
    ADD_REG = 0x01,

    SUB_STACK = 0x02,
    SUB_REG = 0x03,
    
    MUL_STACK = 0x04,
    MUL_REG = 0x05,

    DIV_STACK = 0x06,
    DIV_REG = 0x07,

    //instructions
    LOAD = 0xB0,// load into reg
    PUSH = 0xB1, // push onto stack
    POP_NOREG = 0xB2,
    POP_REG = 0xB3,

    PRINT = 0xFA,
    HALT = 0xFF
};

struct CPU {
    int reg[6] = {0};
    int stack[MEM_SIZE] = {0};

    int sp = MEM_SIZE - 1;
    int pc = 0;

    bool is_running {true};
};


class Virtual {
   
    CPU cpu;

public:

    int init_vm();
    
    void add(Instruction opcode);
    void mul(Instruction opcode);
    void div(Instruction opcode);
    void sub(Instruction opcode);

    void push(int reg, int value);
    void load(int reg, int value);
   
    void printStack();

    //initialize vm with some instructions
    Virtual(const int init_program[], std::size_t size) {

        for(int i = 0; i < size; i++) {
            cpu.stack[i] = init_program[i];
        }
    }
};
