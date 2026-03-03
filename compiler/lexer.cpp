
#include "lexer.h"
#include <iostream>
#include <fstream>
#include <string>

void Lexer::load(std::ifstream& file_name) {

    content.assign(std::istreambuf_iterator<char>(file_name),
                   std::istreambuf_iterator<char>());
}

void Lexer::lex() {

    int start_index = 0, end_index = 0;

    while(end_index < content.length()) {

    }
}
