#pragma once

#include <cstdint>

#define MEM_SIZE 4096

typedef uint16_t Reg;



//corresponds to registers
enum Instruction : int {
    HALT = 0xFF,
    PRINT = 0xFA,
    //operands
    ADD = 0x00,
    SUB = 0x01,
    MUL = 0x02,
    DIV = 0x03,
    //instructions
    LOAD = 0x04,// load into reg
    PUSH = 0x05, // push onto stack
    POP = 0x06

};

struct CPU {
    int reg[6] = {0};
    int stack[MEM_SIZE] = {0};

    int sp = MEM_SIZE - 1;
    int pc = 0;

    bool is_running {true};
};


class Virtual {
   
public:

    CPU cpu; 
    int init_vm();
    
    void add();
    void mul();

    void push(int reg, int value);
    void load(int reg, int value);
   
    void printStack();

    //initialize vm with some instructions
    Virtual(int init_program[], std::size_t size) {

        for(int i = 0; i < MEM_SIZE; i++) cpu.stack[i] = 0;

        for(int i = 0; i < size; i++) {
            cpu.stack[i] = init_program[i];
        }
}};
