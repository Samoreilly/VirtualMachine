# Virtual Machine and Compiler

This project features a custom virtual machine and a dedicated compiler for a structured programming language. The software translates high level source code into bytecode that executes on a simulated processor.

# Architecture

The system architecture is designed for simplicity and efficiency. The virtual machine utilizes 65536 bytes of memory for storage and execution. There are six primary registers used for data manipulation and local storage. Register 5 is specifically designated as the frame pointer to manage local variable scopes and function calls. 

The memory model uses a stack that begins at the highest memory address and grows downward toward lower addresses. A program counter tracks the location of the current instruction to maintain the execution flow. Each bytecode instruction is stored as a single byte for compact program representation.

# Instruction Set Architecture

The virtual machine runs on a diverse set of opcodes that handle different computational tasks.

1. Arithmetic and Logic
The machine supports basic math through instructions like ADD, SUB, MUL, and DIV. These operations can be performed using either registers or values stored on the stack. The CMP instruction compares two register values to assist with logical decisions.

2. Memory and Data Transfer
Data movement is handled by several commands. The LOAD instruction moves immediate values into registers while MOV copies data between registers. The PUSH and POP instructions facilitate interaction with the stack. Direct memory access is supported through STORE and offset based loading via LOAD_REG_OFF.

3. Control Flow
The execution path can be redirected using JUMP instructions. Conditional branching is achieved through JZ and JNZ which jump based on the result of the previous operation. Subroutine management is handled by CALL and RET which allow the program to jump to functions and return to the correct location afterward.

4. System Operations
The PRINT and PRINT_REG instructions provide a way to output data directly to the console for debugging or user interaction. The HALT instruction identifies the end of the program and stops the virtual machine.

# Supported Language Features

The compiler recognizes a specific subset of language constructs that enable complex program logic.

1. Function Definitions
Programs can define functions with specific return types and parameter lists. This allows for modular code and reusable logic across the application.

2. Variable Management
The language supports variable declarations and assignments for integer data. Variables can be managed within local scopes during function execution.

3. Control Structures
Iterative logic is implemented through while loops and for loops. These structures allow programs to execute blocks of code multiple times based on specific conditions.

4. Expressions and Operations
The compiler processes arithmetic expressions and relational comparisons. It also supports unary operations like incrementing values for loop counters.

5. Built In Output
The system includes a print function that can output the results of expressions and variable values to the user interface.

# Implementation Process

The project follows a modular compilation pipeline. The lexer converts source text into a series of tokens. The parser then organizes these tokens into an abstract syntax tree to represent the program structure. The code generator traverses this tree to produce executable bytecode while managing memory offsets through a symbol table. Finally the virtual machine executes the resulting instructions within its managed memory environment.
