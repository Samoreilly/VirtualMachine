#pragma once

#include <vector>
#include <iostream>
#include <optional>
#include <unordered_map>

enum class DataType { STRING, INT, FLOAT, BOOL, VOID};

enum TokenType {
    FUNCTION,
    LOOP,
    KEYWORD,
    NUMBER,
    SYMBOL,
    OPERATOR,
    IDENTIFIER,
};

struct Token {
    TokenType type;
    std::string value;

    int line;
    int col;
};


class Lexer {
    
    static inline const std::unordered_map<std::string, DataType> stringToType = {
        {"String", DataType::STRING},
        {"Int",    DataType::INT},
        {"Float",  DataType::FLOAT},
        {"Bool",   DataType::BOOL},
        {"Void",   DataType::VOID}        
    };

    int start_index = 0, end_index = 0, line = 1, col = 1;

    std::vector<Token> tokens;
    std::string content;
    
    void parse_function();


    void skip_whitespace();
    bool is_symbol(char c);
    bool is_operator(char c);
    
    bool onep_in_bounds();//end_index + 1 < content.length()
    std::string peek_next();
    std::string trim(const std::string& s);
    std::string tokenTypeToString(TokenType type);

public:

    Lexer(std::ifstream& file_name) {
        load(file_name);
        std::cout << "Content: \n" << content;
    }

    void load(std::ifstream& file_name);
    void lex();
    void print_tokens();


    std::vector<Token> get_tokens() { return tokens; } 

};
