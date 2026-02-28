# Virtual Machine Architecture

This project is a custom virtual machine written in C++.

## Memory
The machine has one block of memory. It stores 4096 integers. 
The program instructions start at index 0 and grow upwards. 
The stack starts at the end of memory and grows downwards.

## CPU State
The CPU has 6 registers for storing numbers.
It has a program counter to track the current instruction.
It has a stack pointer to track the top of the stack.

## Operations
The machine handles two main ways of doing math.
1. Stack math. Numbers are pushed to the stack. Math operations pop the top two numbers and push the result back.
2. Register math. Numbers are loaded into registers. Math operations happen directly inside the registers.

## Execution Loop
The machine runs a continuous loop to function.
1. Read the instruction at the program counter.
2. Move the program counter forward.
3. Figure out what the instruction means.
4. Perform the action.
5. Stop when a halt instruction is read.



# Assembler 
## (in progress)

1. Take an .asm file that uses my instruction set defined in virtual_main.h
2. Translate the data to a vector<int> that can be used to pass into the program



# Lexer
1. Translate keywords to matching hexadecimal

