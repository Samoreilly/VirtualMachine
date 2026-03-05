
#include "parser.h"
#include "node.h"
#include "../lexer/lexer.h"

#include <memory>
#include <stdexcept>

void Parser::construct_node() {

    std::unique_ptr<MainNode> node = std::make_unique<MainNode>();
    
    while(in_bounds()) {

        Token t = peek();

        switch (t.type) {

            case FUNCTION: {

                auto fn = parse_function();
            
                node->globals.push_back(std::move(fn));

                break;
            } 

            case LOOP: {
                break;
            } 
            
            case KEYWORD: {
                break;
            }

            case NUMBER: {
                break;
            }
            
            case SYMBOL: {
                break;
            }

            case OPERATOR: {
                break;
            }
            
            case IDENTIFIER: {
                break;
            }

            default: {
                advance();
                break;
            }
        };
    }
}

std::unique_ptr<ForNode> Parser::parse_for() {
    auto f = std::make_unique<ForNode>();

    advance();

    expect(SYMBOL, "(");
    
    do {
        

    }while(match(SYMBOL, ";"));

    return f;
}

std::unique_ptr<FunctionNode> Parser::parse_function() {
    
    advance();//move past function

    auto function_node = std::make_unique<FunctionNode>();
    
    // get return type
    if(peek().type == IDENTIFIER && is_return_type(peek().value)) {
        function_node->return_type = advance().value;
    }else {
        throw std::runtime_error("Return-type invalid or not present at line " + std::to_string(peek().line));
    } 
    
    // get function name
    if(peek().type == IDENTIFIER) {
        function_node->func_name = advance().value;
    }else {
        throw std::runtime_error("Function name invalid or not present at line " + std::to_string(peek().line));
    }

    expect(SYMBOL, "(");

    if(match(SYMBOL, ")")) {
        // no parameters
    }else {

        do {
            Parameter p;

            Token type = expect(IDENTIFIER, "", "Expected parameter type");

            if(!is_return_type(type.value)) {
                throw std::runtime_error("Invalid parameter type: " + type.value);
            }

            Token name = expect(IDENTIFIER, "", "Expected parameter name");

            p.name = name.value;
            p.type = type.value;

            function_node->parameters.push_back(p);

        }while(match(SYMBOL, ","));

        expect(SYMBOL, ")");
    }

    return function_node;

}

