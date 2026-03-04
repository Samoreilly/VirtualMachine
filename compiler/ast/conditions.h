
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
