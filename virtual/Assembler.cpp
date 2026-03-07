#include "Assembler.h"
#include "virtual_main.h"

#include <boost/algorithm/string/trim.hpp>
#include <cctype>
#include <fstream>
#include <stdexcept>
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
    

    
    file.close();

}

//this function is used to find the index for every function
//so there is no need to manually count the index for every change made
void Assembler::labels() {
    int index {0};
    int counter {0};

    while (index < content.length()) {
        char c = content[index];

        if (std::isspace(c) || c == ',') {
            index++;
            continue;
        }

       if (c == '[') {

            index++;
            int s_index = index;

            while (index < content.length() && content[index] != ']') {
                index++;
            }

            if (index < content.length()) {
                std::string_view func(content.data() + s_index, index - s_index);

                label_counter[std::string(func)] = counter;

                
                index++;
            }

            continue;
        }

        if (c == 'R' && index + 1 < content.length() && std::isdigit(content[index + 1])) {
            counter++;

            index += 2; 
            while (index < content.length() && std::isdigit(content[index])) index++;
            continue;
        }

        bool found_content = false;
        while (index < content.length() && !std::isspace(content[index]) && 
               content[index] != '[' && content[index] != ',') {
            index++;
            found_content = true;
        }

       if (found_content) {
            counter++; 
        }
    }
}


void Assembler::lexer() {



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
            
            if(c == '[') {

                while(end_index < content.length() && content[end_index] != ']') {
                    end_index++;
                } 
            }

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

            if(label_counter.find(std::string(curr)) != label_counter.end()) {
                tokens.push_back(label_counter[std::string(curr)]);

                continue;
            }


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
