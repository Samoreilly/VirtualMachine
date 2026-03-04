
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


