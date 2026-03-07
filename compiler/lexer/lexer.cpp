
#include "lexer.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <cctype>

void Lexer::load(std::ifstream& file_name) {

    content.assign(std::istreambuf_iterator<char>(file_name),
                   std::istreambuf_iterator<char>());

    content = trim(content);
}

void Lexer::lex() {

    while(end_index < content.length()) {
    
        skip_whitespace();

        char c = content[end_index];

        if(std::isalpha(c)) {
            start_index = end_index;

            while(end_index < content.length() && (std::isalpha(content[end_index]) || content[end_index] == '_')) {
                end_index++;
                col++;
            }

            std::string_view curr(content.data() + start_index, end_index - start_index);
            std::string data(curr);

            if(data == "function") {
                tokens.push_back({TokenType::FUNCTION, data, line, col});
            }else if(data == "for" || data == "while") {
                tokens.push_back({TokenType::LOOP, data, line, col});
            }else if(data == "return" || data == "if") {
                tokens.push_back({TokenType::KEYWORD, data, line, col});
            }else {
                tokens.push_back({TokenType::IDENTIFIER, data, line, col});
            }
            
            start_index = end_index;
            continue;
        }

        if(is_operator(c)) {
            start_index = end_index;

            std::string next = peek_next();
 
            if(c == '+' && next == "+") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;                
            }else if(c == '+' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '+') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;

            }else if(c == '-' && next == "-") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;                
            }else if(c == '-' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '-') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;
            }else if(c == '*' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '*') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;
            }else if(c == '/' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '/') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;

            }else if(c == '=' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '=') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;
            }else if(c == '>' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '>') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;
            }else if(c == '<' && next == "=") {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c) + next, line, col});
                end_index += 2;
            }else if(c == '<') {
                tokens.push_back({TokenType::OPERATOR, std::string(1, c), line, col});
                end_index++;
            }
            
            start_index = end_index;
            continue;
        }

        if(std::isdigit(c)) {
            start_index = end_index;

            while(end_index < content.length() && std::isdigit(content[end_index])) {
                end_index++;
                col++;
            }

            std::string_view value(content.data() + start_index, end_index - start_index);

            std::string val(value);

            tokens.push_back({TokenType::NUMBER, val, line, col});
            
            continue;
        }

        if(is_symbol(c)) {
            tokens.push_back({TokenType::SYMBOL, std::string(1, c), line, col});
            end_index++;
            
            start_index = end_index;
            continue;
        }
        
        if(end_index > start_index) {

            
            std::string_view dat(content.data() + start_index, end_index - start_index);
            std::string data(dat);
            
            tokens.push_back({IDENTIFIER, data, line, col});
            end_index++;
            
            start_index = end_index;
            continue;
        }



    }
}












/*
UTILITY FUNCTIONS
*/

bool Lexer::onep_in_bounds() {
    return end_index + 1 < content.length(); 
}

std::string Lexer::peek_next() {
    if(onep_in_bounds()) {
        return std::string(1, content[end_index + 1]);
    }
    return "";
}

void Lexer::skip_whitespace() {
    while(end_index < content.length() && std::isspace(content[end_index])) {
        if(content[end_index] == '\n') {
            line++;
            col = 1;
        } else {
            col++;
        }
        end_index++;
    }
}

bool Lexer::is_symbol(char c) {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == ';';
}

bool Lexer::is_operator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '<' || c == '>';
}

void Lexer::print_tokens() {
    
    std::cout << "\n";

    for(auto t : tokens) {
        std::cout << "Token type: " << tokenTypeToString(t.type)
            << "\nToken value: " << t.value
            << "\nLine: " << t.line
            << "\nColumn: " << t.col
            << "\n\n";
    }
}

std::string Lexer::trim(const std::string& s) {
    auto start = std::find_if_not(s.begin(), s.end(), ::isspace);
    auto end = std::find_if_not(s.rbegin(), s.rend(), ::isspace).base();
    if (start >= end) return "";
    return std::string(start, end);
}

std::string Lexer::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::FUNCTION:   return "FUNCTION";
        case TokenType::LOOP:       return "LOOP";
        case TokenType::KEYWORD:    return "KEYWORD";
        case TokenType::NUMBER:     return "NUMBER";
        case TokenType::SYMBOL:     return "SYMBOL";
        case TokenType::OPERATOR:   return "OPERATOR";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        default:                    return "INVALID_TYPE";
    }
}




