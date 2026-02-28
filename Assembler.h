#pragma once

#include "virtual_main.h"

#include <cctype>
#include <string>
#include <vector>
#include <unordered_map>

class Assembler {

    static inline const std::unordered_map<std::string, uint8_t> INSTRUCTIONS {

        {"ADD_STACK", ADD_STACK},
        {"ADD_REG",   ADD_REG},
        {"SUB_STACK", SUB_STACK},
        {"SUB_REG",   SUB_REG},
        {"MUL_STACK", MUL_STACK},
        {"MUL_REG",   MUL_REG},
        {"DIV_STACK", DIV_STACK},
        {"DIV_REG",   DIV_REG},
        {"LOAD",      LOAD},
        {"PUSH",      PUSH},
        {"POP_NOREG", POP_NOREG},
        {"POP_REG",   POP_REG},
        {"PRINT",     PRINT},
        {"HALT",      HALT},
        {"JUMP",      JUMP},
        {"JZ",        JZ}       // jumps to a location if last result was zero
    
    };
    
    static inline const bool is_symbol(char c) {
        return c == ',';
    }

    std::string file_name;
    std::vector<uint8_t> tokens;
    std::string content;

    int start_index = 0, end_index = 0;

    void print();

public:

    Assembler(const std::string& f) : file_name(f) {
        load();
    }

    void load();
    void lexer();
        
    std::vector<uint8_t> get_tokens() { return tokens; }
};


