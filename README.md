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
4. Perform the action.U
5. Stop when a halt instruction is read.



# Assembler
The assembler converts a text file into machine code. 
1. It reads the instructions from an asm file.
2. it converts the strings into a vector of integers.
3. The virtual machine then evaluates the vector.

# Lexer
The lexer processes the raw text.
1. It identifies the keywords and numbers.
2. It maps the keywords to the instruction set.
3. It ignores whitespace and commas.

4. Then passed onto the Virtual Machine.



# Implementing stack frames (to avoid using a seperate call stack)

## allocate a register to store the frame pointer - the pc value when a CALL is made
## let function run, handle operations, RET  | All values from CALL TO RET must be popped off stack
## when RET is called, fetch frame pointer - pc = frame pointer + 1, for thenext instruction











