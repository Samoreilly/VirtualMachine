#pragma once

#include "../lexer/lexer.h"
#include <string>
#include <iostream>
#include <memory>

class CodeGenerator;

class Node {
public:

    virtual ~Node() = default;
    virtual void print() const = 0;
    virtual void generate(CodeGenerator& gen) = 0;

};

class Condition : public Node {
public:

    virtual ~Condition() = default;
    virtual void print() const = 0;
    virtual void generate(CodeGenerator& gen) = 0;
};

class BinaryExpression : public Condition {
public:

    std::unique_ptr<Condition> left;
    std::string op;
    std::unique_ptr<Condition> right;

    void print() const override {
        if (left) left->print();
        std::cout << " " << op << " ";
        if (right) right->print();
    }

    void generate(CodeGenerator& gen) override;

};

class ValueCondition : public Condition {
public:

    Token token;
    ValueCondition(Token t) : token(t) {}

    void print() const override {
        std::cout << token.value;
    }
    
    void generate(CodeGenerator& gen) override;

    ~ValueCondition() override = default;
};

class BooleanCondition : public Condition {
public:

    Token token;
    BooleanCondition(Token t) : token(t) {}

    void print() const override {
        std::cout << token.value;
    }
    
    void generate(CodeGenerator& gen) override;

    ~BooleanCondition() override = default;
};
