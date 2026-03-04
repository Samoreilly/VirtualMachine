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
The assembler converts a text file into byte code. 
1. It reads the instructions from an asm file.
2. it converts the text to bytes
3. later will be deassembled and pass into vm

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



# Compiler for a generic language - java-esque
## loops
## variables
## function


### example snippet of what it might look like

```

function int mul_by_two(int a) {
    return a * 2;
} 

int i = 0;

while(i < 10) {
    print(mul_by_two(i));
    i++;
}

```


# Compiler will produce an Abstract Syntax Tree
## Walk ast and translate it to .asm
## Pass to virtual machine

# Compiler will double pass, first pass filling out its type


# Lexer
1. Building a state machine lexical analyser




### State machine
#### When a keyword is detected such as 'function'/'while', state is changed
#### and we will lex that path. As much as I want to keep limited errors in
#### lexing, this will allow for finer control and clearer syntax errors

#### e.g. function is scanned, state = TokenType::FUNCTION.
#### Then goes down FUNCTION case in switch statement.
