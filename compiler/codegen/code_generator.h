#pragma once

#include "symbol_table.h"
#include "../../virtual/virtual_main.h"
#include <vector>
#include <string>
#include <memory>
#include <cstdint>

class Node;
class Condition;

class CodeGenerator {

    std::vector<uint8_t> bytecode;
    SymbolTable symbols;
    std::unordered_map<std::string, int> function_labels;
    int current_label = 0;

public:

    void emit(uint8_t opcode) {
        bytecode.push_back(opcode);
    }

    void emit(uint8_t opcode, uint8_t val) {
        bytecode.push_back(opcode);
        bytecode.push_back(val);
    }

    void emit(uint8_t opcode, uint8_t val1, uint8_t val2) {
        bytecode.push_back(opcode);
        bytecode.push_back(val1);
        bytecode.push_back(val2);
    }

    std::vector<uint8_t>& get_bytecode() {
        return bytecode;
    }

    SymbolTable& get_symbols() {
        return symbols;
    }

    std::unordered_map<std::string, int>& get_function_labels() {
        return function_labels;
    }

    int create_label() {
        return current_label++;
    }

    void write_at(int pos, uint8_t val) {
        bytecode[pos] = val;
    }

    int get_pos() const {
        return bytecode.size();
    }

};
