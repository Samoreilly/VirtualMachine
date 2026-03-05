#pragma once

#include "../lexer/lexer.h"
#include "node.h"

#include <optional>
#include <unordered_set>
#include <stdexcept>
#include <memory>

class Parser {

    std::vector<Token> tokens;
    int t_index;

public:

    static inline const std::unordered_set<std::string> RETURN_TYPES = {
        "void", "int", "string", "bool"
    };

    Parser(std::vector<Token>& t) : tokens(t), t_index(0) {}

    void construct_node();
    
    std::unique_ptr<FunctionNode> parse_function();
    std::unique_ptr<ForNode> parse_for();


    bool in_bounds() const {
        return t_index < tokens.size();
    }

    Token peek() const {
        if (!in_bounds()) return tokens.back();
        return tokens[t_index];
    }

    Token advance() {
        if (in_bounds()) return tokens[t_index++];
        return tokens.back();
    }

    bool match(TokenType type, const std::string& expected = "") {
        if (!in_bounds()) return false;
        if (tokens[t_index].type == type) {
            if (expected.empty() || tokens[t_index].value == expected) {
                t_index++;
                return true;
            }
        }
        return false;
    }

    Token expect(TokenType type, const std::string& expected = "", const std::string& errMsg = "") {
        if (match(type, expected)) {
            return tokens[t_index - 1];
        }

        std::string msg = errMsg.empty() ? "Unexpected token: " + peek().value : errMsg;
        throw std::runtime_error(msg + " at line " + std::to_string(peek().line));
    }

    bool is_return_type(const std::string& type) {
        return RETURN_TYPES.find(type) != RETURN_TYPES.end();
    }

};

