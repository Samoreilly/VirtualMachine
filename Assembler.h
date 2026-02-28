#pragma once

#include <string>
#include <vector>

class Assembler {

    std::string file_name;
    std::vector<int> tokens;

public:

    Assembler(const std::string& f) : file_name(f) {}

    void load();

};

