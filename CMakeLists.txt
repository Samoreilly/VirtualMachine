cmake_minimum_required(VERSION 4.1)
project(VirtualMachine)

set(CMAKE_CXX_STANDARD 20)

add_executable(
  VirtualMachine
  virtual_main.cpp
  Assembler.cpp 
  compiler/lexer/lexer.cpp
  compiler/ast/node.cpp
  compiler/ast/parser.cpp
  compiler/codegen/codegen.cpp
)
