#include "Assembler.h"
#include "virtual_main.h"

#include <boost/algorithm/string/trim.hpp>
#include <cctype>
#include <fstream>
#include <string>
#include <iostream>
#include <boost/algorithm/string.hpp>

void Assembler::load() {

    std::ifstream file(file_name);

    if(!file.is_open()) {
        std::cerr << "Failed to open file:" << file_name << "\n";
    }

    std::string line;
    while(std::getline(file, line)) {
        content += (line + "\n");
    }
    
    std::cout << "File Content: \n" << content << "\n";
    
    file.close();

}

void Assembler::lexer() {

    std::cout << "Debug: Entered lexer";

    while(end_index < content.length()) {

        char c = content[end_index];
        
        if(std::isspace(c)) {
            
            end_index++;
            
            while(end_index < content.length() && std::isspace(content[end_index])) {
                end_index++;
            }

            start_index = end_index;
            continue;
        }

        if(c == 'R' && end_index + 1 < content.length() && std::isdigit(content[end_index + 1])) {
            tokens.push_back(content[end_index + 1] - '0');
            
            end_index += 2;
            start_index = end_index;
            continue;
        }

        if(std::isdigit(c)) {
            start_index = end_index;

            while(end_index < content.length() && std::isdigit(content[end_index])) {
                end_index++;
            }

            std::string_view curr(content.data() + start_index, end_index - start_index);
            
            tokens.push_back(std::stoi(std::string(curr)));
            
            start_index = end_index;
            continue;

        }

        if(is_symbol(c)) {
            end_index++;
            start_index = end_index;
            continue;
        }


        start_index = end_index;
        while(end_index < content.length() && !std::isspace(content[end_index]) && !is_symbol(content[end_index]) && !std::isdigit(content[end_index])) {
            end_index++;
        }

        if(end_index > start_index) {
            std::string_view curr(content.data() + start_index, end_index - start_index);
            
            std::string op(curr);
            boost::trim_left(op);
            boost::trim_right(op);

            std::cout << "Printed string: [" << curr << "]" << std::endl;
            int opcode = INSTRUCTIONS.at(op);
            tokens.push_back(opcode);
                        
        }

        start_index = end_index;

    }

    print();

}


void Assembler::print() {

    // for(int i : tokens) {
    //     std::cout << "0x" << std::hex << i << "\n";
    // }

}
