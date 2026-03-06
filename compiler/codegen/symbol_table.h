#pragma once

#include <string>
#include <unordered_map>
#include <vector>

struct Symbol {
    std::string name;
    std::string type;
    int offset;
};

class SymbolTable {

    std::vector<std::unordered_map<std::string, Symbol>> scopes;
    int stack_offset = 0;

public:

    SymbolTable() {
        push_scope();
    }

    void push_scope() {
        scopes.push_back({});
    }

    void pop_scope() {
        scopes.pop_back();
    }

    void add_symbol(const std::string& name, const std::string& type) {
        stack_offset += 1;
        scopes.back()[name] = {name, type, stack_offset};
    }

    Symbol* lookup(const std::string& name) {
        for(auto it = scopes.rbegin(); it != scopes.rend(); ++it) {
            if(it->find(name) != it->end()) {
                return &(*it)[name];
            }
        }
        return nullptr;
    }

    int get_offset() const {
        return stack_offset;
    }

};
