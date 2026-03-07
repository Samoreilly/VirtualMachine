#pragma once

#include "conditions.h"

#include <string>
#include <vector>
#include <memory>
#include <iostream>
#include <optional>

enum class NodeType { 
    FUNC_CALL,
    FUNC_BODY,
    LOOP,
    IDENTIFIER,
    GLOBAL,
    OPERAND,
    ASSIGNMENT,
    VAR_DECL,
    NUMBER,
    PRINT,
    PROGRAM
};

struct Parameter {
    std::string type;
    std::string name;
};

class MainNode : public Node {
public:
    std::vector<std::unique_ptr<Node>> globals;

    void print() const override {
        for (const auto& node : globals) {
            node->print();
        }
    }

    void generate(CodeGenerator& gen) override;
};

class BodyNode : public Node {
public:

    std::vector<std::unique_ptr<Node>> statements;
    
    void print() const override {
        for (const auto& statement : statements) {
            statement->print();
        }
    }

    void generate(CodeGenerator& gen) override;
};

class FunctionNode : public Node {
public:
    
    std::string func_name;
    std::string return_type;
    
    std::unique_ptr<BodyNode> body;
    std::vector<Parameter> parameters;

    void print() const override {
        std::cout << "func " << return_type << " " << func_name << "(";
        for (size_t i = 0; i < parameters.size(); ++i) {
            std::cout << parameters[i].type << " " << parameters[i].name;
            if (i < parameters.size() - 1) std::cout << ", ";
        }
        std::cout << ") { \n";
        if (body) body->print();
        std::cout << "}\n";
    }

    void generate(CodeGenerator& gen) override;

};

class FunctionCallNode : public Condition {
public:

    std::string function_name;
    std::vector<std::unique_ptr<Condition>> arguments;

    void print() const override {
        std::cout << function_name << "(";
        for (size_t i = 0; i < arguments.size(); ++i) {
            arguments[i]->print();
            if (i < arguments.size() - 1) std::cout << ", ";
        }
        std::cout << ")";
    }

    void generate(CodeGenerator& gen) override;
};

class IfNode : public Node {
public:

    std::unique_ptr<Condition> condition;
    std::unique_ptr<BodyNode> body;

    void print() const override {
        std::cout << "if (";
        if (condition) condition->print();
        std::cout << ") { \n";
        if (body) body->print();
        std::cout << "}\n";
    }

    void generate(CodeGenerator& gen) override;
};


class ForNode : public Node {
public:

    std::unique_ptr<Node> initialization;
    std::unique_ptr<Condition> condition;
    std::unique_ptr<Node> increment;

    std::unique_ptr<BodyNode> body;
    
    void print() const override {
        std::cout << "for (";
        if (initialization) initialization->print();
        std::cout << "; ";
        if (condition) condition->print();
        std::cout << "; ";
        if (increment) increment->print();
        std::cout << ") { \n";
        if (body) body->print();
        std::cout << "}\n";
    }

    void generate(CodeGenerator& gen) override;

};

class WhileNode : public Node {
public:

    std::unique_ptr<Condition> condition;
    std::unique_ptr<BodyNode> body;

    void print() const override {
        std::cout << "while (";
        if (condition) condition->print();
        std::cout << ") { \n";
        if (body) body->print();
        std::cout << "}\n";
    }

    void generate(CodeGenerator& gen) override;

};

class DeclareVariable : public Node {
public:

    std::string name;
    std::string type;

    std::unique_ptr<Condition> init;

    void print() const override {
        std::cout << type << " " << name;
        if (init) {
            std::cout << " = ";
            init->print();
        }
        std::cout << ";\n";
    }

    void generate(CodeGenerator& gen) override;
};

class InitVariable : public Node {
public:

    std::string name;
    std::unique_ptr<Condition> init;

    void print() const override {
        std::cout << name << " = ";
        if (init) init->print();
        std::cout << ";\n";
    }

    void generate(CodeGenerator& gen) override;
};

class PrintNode : public Node {
public:

    std::unique_ptr<Condition> expression;

    void print() const override {
        std::cout << "print(";
        if (expression) expression->print();
        std::cout << ");\n";
    }

    void generate(CodeGenerator& gen) override;
};

class NumberNode : public Node {
public:
    
    int value;
    
    NumberNode(int v) : value(v) {}

    void print() const override {
        std::cout << value;
    }

    void generate(CodeGenerator& gen) override;

};

class ReturnNode : public Node {
public:

    std::unique_ptr<Condition> expression;

    void print() const override {
        std::cout << "return ";
        if (expression) expression->print();
        std::cout << ";\n";
    }

    void generate(CodeGenerator& gen) override;
};
