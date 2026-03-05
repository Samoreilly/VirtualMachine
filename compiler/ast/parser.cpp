
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

std::unique_ptr<BodyNode> Parser::parse_body() {
    consume(SYMBOL, "{");

    auto body = std::make_unique<BodyNode>();



    consume(SYMBOL, "}");
}

std::unique_ptr<WhileNode> Parser::parse_while() {
    
    consume(LOOP, "while");

    auto w = std::make_unique<WhileNode>();

    consume(SYMBOL, "(");

    if(!check(SYMBOL, ")")) {
        auto b = std::make_unique<BinaryExpression>();
 
    }


}

std::unique_ptr<ForNode> Parser::parse_for() {
    
    consume(LOOP, "for");
   
    auto f = std::make_unique<ForNode>();

    consume(SYMBOL, "(");

    if (!check(SYMBOL, ";")) {
    
        if (is_return_type(peek().value)) {
            std::string type = advance().value;
            f->init = consume(IDENTIFIER, "", "Expected loop variable name").value;
        }
    }
    
    consume(SYMBOL, ";");

    if (!check(SYMBOL, ";")) {
        f->condition = parse_condition();
    }
    consume(SYMBOL, ";");

    if (!check(SYMBOL, ")")) {
        auto b = std::make_unique<BinaryExpression>();
        
        Token t = consume(IDENTIFIER, "", "Expected increment variable");

        b->left = std::make_unique<ValueCondition>(t);
        
        if(match(OPERATOR)) {
            b->op = peek().value;
            
            Token token = consume(IDENTIFIER);

            b->right = std::make_unique<ValueCondition>(token);
            
            
        }else {
            throw std::runtime_error("You dont how know how to for loop");
        }

        f->incr = std::move(b); 
    }

    consume(SYMBOL, ")");

    return f;
}

std::unique_ptr<FunctionNode> Parser::parse_function() {
    consume(FUNCTION);

    auto function_node = std::make_unique<FunctionNode>();
    
    // get return type
    Token return_type = consume(IDENTIFIER, "", "Expected return type");
    if (is_return_type(return_type.value)) {
        function_node->return_type = return_type.value;
    } else {
        throw std::runtime_error("Invalid return type: " + return_type.value);
    } 
    
    // get function name
    function_node->func_name = consume(IDENTIFIER, "", "Expected function name").value;

    consume(SYMBOL, "(");

    if (!match(SYMBOL, ")")) {

        do {
            Parameter p;
            p.type = consume(IDENTIFIER, "", "Expected parameter type").value;
       
            if (!is_return_type(p.type)) {
                throw std::runtime_error("Invalid parameter type: " + p.type);
            }

            p.name = consume(IDENTIFIER, "", "Expected parameter name").value;
            function_node->parameters.push_back(p);

        } while (match(SYMBOL, ","));

        consume(SYMBOL, ")");
    }

    return function_node;
}

std::unique_ptr<BinaryExpression> Parser::parse_condition() {
    // This is a placeholder for your expression parsing logic
    auto expr = std::make_unique<BinaryExpression>();
    
    // Move past something for now so it's not an infinite loop
    advance(); 
    
    return expr;
}
