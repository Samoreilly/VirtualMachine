#pragma once

#include "../lexer/lexer.h"

#include <string>
#include <memory>

class Condition {
public:

    virtual ~Condition() = 0;
    virtual void print() const = 0;
};

class BinaryExpression : public Condition {
public:

    std::unique_ptr<Condition> left;
    std::string op;
    std::unique_ptr<Condition> right;

    void print() const override {

    }
};

class ValueCondition : public Condition {
public:

    Token token;
    ValueCondition(Token t) : token(t) {}

    void print() const override {
        std::cout << token.value;
    }
    
    ~ValueCondition() override = default;
};

class BooleanCondition : public Condition {
public:

    Token token;
    BooleanCondition(Token t) : token(t) {}

    void print() const override {
        std::cout << token.value;
    }
    
    ~BooleanCondition() override = default;
};
