#pragma once

#include <vector>
#include <iostream>

enum TokenType {
    FUNCTION,
    RETURN,
    
    FOR,
    WHILE,

    F_NAME, F_TYPE,
    V_NAME, V_TYPE,
    
    STRING,
    NUMBER,
    BOOLEAN,

    ASSIGN,
    PLUH,
    PLUHPLUH,
    STAR,
    LESS,
    GREATER,
    EQUALS,

    LPAREN, RPAREN,
    LBRACE, RBRACE,
    COMMA,
    SEMICOLON,

    UNKNOWN

};

struct Token {
    TokenType type;
};


class Lexer {

    std::vector<TokenType> tokens;
    std::string content;

public:

    Lexer(std::ifstream& file_name) {
        load(file_name);
    }

    void load(std::ifstream& file_name);
    void lex();

    std::vector<TokenType> get_tokens() { return tokens; } 

};
