cmake_minimum_required(VERSION 4.1)
project(VirtualMachine)

set(CMAKE_CXX_STANDARD 20)

add_executable(VirtualMachine virtual_main.cpp Assembler.cpp 
  compiler/lexer.cpp compiler/ast.cpp compiler/generate.cpp
)
