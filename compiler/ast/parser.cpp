
#include "parser.h"
#include "node.h"
#include "conditions.h"
#include "../lexer/lexer.h"

#include <cctype>
#include <memory>
#include <stdexcept>

void Parser::construct_node() {
    root = std::make_unique<MainNode>();
    
    while(in_bounds()) {
        root->globals.push_back(parse_statement());
    }
}

void Parser::print_ast() const {
    if (root) {
        root->print();
    }
}

std::unique_ptr<BodyNode> Parser::parse_body() {
    
    consume(SYMBOL, "{");

    auto body = std::make_unique<BodyNode>();


    while(!check(SYMBOL, "}")) {
        body->statements.push_back(parse_statement());
    }
    
    consume(SYMBOL, "}");
    return body;
}

std::unique_ptr<IfNode> Parser::parse_if() {
    consume(KEYWORD, "if");
    auto if_node = std::make_unique<IfNode>();

    consume(SYMBOL, "(");
    if_node->condition = parse_condition();
    consume(SYMBOL, ")");
    if_node->body = parse_body();


    return if_node;
}

std::unique_ptr<Node> Parser::parse_statement() {
   
    if(check(FUNCTION)) return parse_function();
    else if(check(LOOP, "while")) return parse_while();
    else if(check(LOOP, "for")) return parse_for();
    else if(is_return_type(peek().value)) return parse_declare();
    else if(peek().value == "print") return parse_print();
    else if(check(KEYWORD, "return")) return parse_return();
    else if(check(KEYWORD, "if")) return parse_if();
    else if(check(IDENTIFIER) && peek_next() && peek_next()->value == "(") return parse_function_call();

    return parse_assignment();
}

std::unique_ptr<Node> Parser::parse_return() {
    consume(KEYWORD, "return");
    auto r = std::make_unique<ReturnNode>();

    if (!check(SYMBOL, ";")) {
        r->expression = parse_condition();
    }

    consume(SYMBOL, ";");
    return r;
}

std::unique_ptr<DeclareVariable> Parser::parse_declare() {
    
    auto dec = std::make_unique<DeclareVariable>();
    
    std::string type = peek().value;
    advance();
    std::string name = peek().value;

    dec->type = type;
    dec->name = name;
    
    advance();

    if(match(OPERATOR, "=")) {
        Token curr = advance();
        
        std::unique_ptr<Condition> init;
    
        if(curr.value == "true" || curr.value == "false") {
            init = std::make_unique<BooleanCondition>(curr);
            dec->init = std::move(init);
        }else {
            init = std::make_unique<ValueCondition>(curr);
            dec->init = std::move(init);
        }        
    }
    consume(SYMBOL, ";");

    return dec;
}

std::unique_ptr<Condition> Parser::parse_function_call() {
    auto call = std::make_unique<FunctionCallNode>();
    call->function_name = consume(IDENTIFIER).value;
    consume(SYMBOL, "(");
    if (!check(SYMBOL, ")")) {
        do {
            call->arguments.push_back(parse_condition());
        } while (match(SYMBOL, ","));
    }
    consume(SYMBOL, ")");
    if (check(SYMBOL, ";")) {
        consume(SYMBOL, ";");
    }
    return call;
}

std::unique_ptr<InitVariable> Parser::parse_assignment() {
    
    auto ass = std::make_unique<InitVariable>();

    Token name_token = consume(IDENTIFIER, "", "You cannot assign a number to another value");
    ass->name = name_token.value;

    if(check(OPERATOR)) {
        auto bin = std::make_unique<BinaryExpression>();

        bool no_second_value {false};//for e.g. if val++;  there shouldn't be a value after ++
        Token op = peek();


        bin->left = std::make_unique<ValueCondition>(name_token);
        bin->op = op.value;
        
        advance();

        Token right = peek();
        
        if(right.value != ";" && right.value != ")") {
            
            if((op.value == "++" || op.value == "--")) {
                throw std::runtime_error("Increments and Decrements must not be followed by a value");
            }
        
            bin->right = std::move(parse_condition());

        }

        
        if(check(SYMBOL, ";")) consume(SYMBOL, ";");

        ass->init = std::move(bin);
    } 

    return ass;

}

std::unique_ptr<WhileNode> Parser::parse_while() {
    
    consume(LOOP, "while");

    auto w = std::make_unique<WhileNode>();

    consume(SYMBOL, "(");

    w->condition = parse_condition();

    consume(SYMBOL, ")");

    w->body = parse_body();

    return w;
    
}

std::unique_ptr<ForNode> Parser::parse_for() {
    consume(LOOP, "for");
    auto f = std::make_unique<ForNode>();

    consume(SYMBOL, "(");

    if (!check(SYMBOL, ";")) {
        if (is_return_type(peek().value)) {
            f->initialization = parse_declare();
        } else {
            f->initialization = parse_assignment();
        }
    } else {
        consume(SYMBOL, ";");
    }

    if (!check(SYMBOL, ";")) {
        f->condition = parse_condition();
    }
    consume(SYMBOL, ";");

    if (!check(SYMBOL, ")")) {
        f->increment = parse_assignment();
    }

    consume(SYMBOL, ")");
    
    f->body = parse_body();

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

    function_node->body = parse_body();

    return function_node;
}

std::unique_ptr<Node> Parser::parse_print() {
    advance();
    consume(SYMBOL, "(");
    auto p = std::make_unique<PrintNode>();
    p->expression = parse_condition();
    consume(SYMBOL, ")");
    consume(SYMBOL, ";");
    return p;
}

//will handle single condition for now.. for e.g. i < 10
std::unique_ptr<Condition> Parser::parse_condition() {
    
    auto expr = std::make_unique<BinaryExpression>();
    
    Token var = peek();

    if (peek_next() && peek_next()->value == "(") {
        return parse_function_call();
    }
   
    bool string_of_digit = string_of_digits(var.value);
   
    if(peek_next() && peek_next()->value == ")") {
        advance();

        if(string_of_digit) {
            return std::make_unique<BooleanCondition>(var);
        }else {
            return std::make_unique<ValueCondition>(var);
        }
        
    }

    advance();

    auto bin = std::make_unique<BinaryExpression>();

    if(string_of_digit) {
        bin->left = std::make_unique<BooleanCondition>(var);
    }else {
        bin->left = std::make_unique<ValueCondition>(var);  
    }
    
    bin->op = peek().value;
    advance();

    Token right = peek();
    
    if(string_of_digits(right.value)) {
        bin->right = std::make_unique<BooleanCondition>(right);
    }else {
        bin->right = std::make_unique<ValueCondition>(right);  
    }

    advance();
    
    return bin;
}

bool Parser::string_of_digits(std::string_view str) const {
    
    for(char c : str) {
        if(!std::isdigit(c)) {
            return false;
        }
    }
    return true;
}









