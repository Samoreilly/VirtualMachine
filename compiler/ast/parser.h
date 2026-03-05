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
    std::unique_ptr<WhileNode> parse_while();
    std::unique_ptr<Node> parse_global();
    std::unique_ptr<BinaryExpression> parse_condition();
    std::unique_ptr<BodyNode> parse_body();
    std::unique_ptr<Node> parse_statement();
    std::unique_ptr<DeclareVariable> parse_declare();
    std::unique_ptr<InitVariable> parse_assignment();

    bool in_bounds() const { return t_index < tokens.size(); }

    Token peek() const {
        return in_bounds() ? tokens[t_index] : tokens.back();
    }

    std::optional<Token> peek_next() const {
        if(t_index + 1 < tokens.size()) {
            return tokens[t_index + 1];
        }

        return std::nullopt;
    }

    Token advance() {
        if (in_bounds()) return tokens[t_index++];
        return tokens.back();
    }

    bool check(TokenType type, const std::string& val = "") const {
        if (!in_bounds()) return false;
        if (tokens[t_index].type != type) return false;
        if (!val.empty() && tokens[t_index].value != val) return false;
        return true;
    }

    bool match(TokenType type, const std::string& val = "") {
        if (check(type, val)) {
            advance();
            return true;
        }
        return false;
    }

    Token consume(TokenType type, const std::string& val = "", const std::string& msg = "") {
        if (check(type, val)) return advance();
        
        std::string err = msg.empty() ? "Expected " + val : msg;
        throw std::runtime_error(err + " at line " + std::to_string(peek().line));
    }

    bool is_return_type(const std::string& type) {
        return RETURN_TYPES.find(type) != RETURN_TYPES.end();
    }

};

