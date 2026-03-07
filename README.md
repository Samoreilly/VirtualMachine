# Virtual Machine and Compiler

This project features a custom virtual machine and a dedicated compiler for a structured programming language. The software translates high level source code into bytecode that executes on a simulated processor.

# Architecture

The system supports two independent execution paths that both produce the same bytecode format for the virtual machine.

### Execution Paths

**Path 1 — High-Level Compiler (`.sam` source files)**

Source code written in the custom language is processed through a multi stage compilation pipeline. The Lexer tokenizes the source text, the Parser constructs an abstract syntax tree, and the Code Generator emits bytecode directly. This path handles functions, loops, variables, and expressions automatically.

```
code.sam → Lexer → Parser → Code Generator → Bytecode → VM
```

**Path 2 — Assembler (`.asm` assembly files)**

Assembly files written using the instruction set mnemonics are processed by the Assembler. The Assembler resolves labels to calculate jump offsets and translates each mnemonic into its corresponding opcode. This path provides direct low-level control over the virtual machine.

```
data.asm → Assembler (load → labels → lexer) → Bytecode → VM
```

Both paths output a `vector<uint8_t>` of bytecode that is loaded into the virtual machine's memory for execution. The virtual machine is agnostic to how the bytecode was produced, enforcing a clean separation between the frontend and the execution engine.

### Memory and Registers

The virtual machine utilizes 65536 bytes of unified memory for both instruction storage and the stack. Bytecode instructions are loaded starting from address 0 and the stack begins at the highest memory address, growing downward. A program counter (PC) tracks the current instruction and a stack pointer (SP) tracks the top of the stack.

There are six general-purpose registers. R0 serves as the accumulator for arithmetic results and return values. R1 acts as an auxiliary register for the second operand in binary operations. R2 through R4 are reserved for future use. R5 is designated as the frame pointer (FP) to manage function call frames and local variable access.

Each bytecode instruction is stored as a single byte for compact program representation.

<img width="1063" height="586" alt="image" src="https://github.com/user-attachments/assets/522b5e58-a9ff-4517-a31a-e797dd64231d" />



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
