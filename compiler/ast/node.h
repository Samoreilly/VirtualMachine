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
    PROGRAM
};

struct Parameter {
    std::string type;
    std::string name;
};

class Node {
public:

    virtual ~Node() {}
    virtual void print() const = 0;

};


//will the base node
class MainNode : public Node {
public:
    //holds functions, loop and global variables
    std::vector<std::unique_ptr<Node>> globals;

    void print() const override {
        
    }
};

//block of code e.g. in a function
class BodyNode : public Node {
public:

    std::vector<std::unique_ptr<Node>> statements;
};

class FunctionNode : public Node {
public:
    
    std::string func_name;
    std::string return_type;
    
    std::unique_ptr<BodyNode> body;//function body
    std::vector<Parameter> parameters;

    void print() const override {
        std::cout << "\n" << "Function name: " << func_name
                          << "Return type: "   << return_type
                          << "\n";
    }

};

class ForNode : public Node {
public:

    std::optional<std::string> init;
    std::optional<std::unique_ptr<Condition>> condition;
    std::optional<std::unique_ptr<BinaryExpression>> incr;

    std::unique_ptr<BodyNode> body;
    
    void print() const override {

    }

};

class WhileNode : public Node {
public:

    std::unique_ptr<Condition> condition;
    std::unique_ptr<BodyNode> body;//while loop

    void print() const override {

    }

};

class DeclareVariable : public Node {

    std::string name;
    std::string type;

    std::unique_ptr<Condition> init;

    void print() const override {

    }
};

class InitVariable : public Node {

    std::string name;
    std::unique_ptr<Condition> init;

    void print() const override {

    }
};


class NumberNode : public Node {
public:
    
    int value;
    
    NumberNode(int v) : value(v) {}

    void print() const override {

    }

};

class StringNode : public Node {

    std::string value;

    StringNode(std::string v) : value(v) {}

    void print() const override {

    }
};




