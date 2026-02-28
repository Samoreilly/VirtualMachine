#include "Assembler.h"

#include <fstream>
#include <string>
#include <iostream>

void Assembler::load() {

    std::ifstream file(file_name);

    if(!file.is_open()) {
        std::cerr << "Failed to open file:" << file_name << "\n";
    }
    std::string line, content;

    while(std::getline(file, line)) {
        content += (line + "\n");
    }
    
    std::cout << "File Content: \n" << content << "\n";
    
    file.close();

}
