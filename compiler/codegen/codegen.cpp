#include "code_generator.h"
#include "../ast/node.h"
#include "../ast/conditions.h"

void MainNode::generate(CodeGenerator& gen) {
    int jump_instr_pos = gen.get_pos();
    gen.emit(Instruction::JUMP, (uint8_t)0);

    for (auto& global : globals) {
        if (dynamic_cast<FunctionNode*>(global.get())) {
            global->generate(gen);
        }
    }

    int main_start = gen.get_pos();
    gen.write_at(jump_instr_pos + 1, (uint8_t)main_start);

    for (auto& global : globals) {
        if (!dynamic_cast<FunctionNode*>(global.get())) {
            global->generate(gen);
        }
    }
    
    gen.emit(Instruction::HALT);
}

void BodyNode::generate(CodeGenerator& gen) {
    gen.get_symbols().push_scope();
    for (auto& statement : statements) {
        statement->generate(gen);
    }
    gen.get_symbols().pop_scope();
}

void FunctionNode::generate(CodeGenerator& gen) {
    gen.get_function_labels()[func_name] = gen.get_pos();
    SymbolTable& table = gen.get_symbols();
    table.push_scope();
    
    // parameters are at positive offsets from FP
    // Stack[FP+1] is old FP, Stack[FP+2] is RetAddr, Stack[FP+3] is Param 1
    for(size_t i = 0; i < parameters.size(); ++i) {
        // we use a special negative offset indicator or just handle it in lookup
        Symbol sym = {parameters[i].name, parameters[i].type, (int)(3 + i)};
        // manually bypass add_symbol logic to set positive offset
        gen.get_symbols().add_symbol(parameters[i].name, parameters[i].type);
        Symbol* s = gen.get_symbols().lookup(parameters[i].name);
        s->offset = (3 + i) * -1; // hack: use negative of negative in codegen
    }

    if (body) body->generate(gen);
    
    table.pop_scope();

    if (gen.get_bytecode().empty() || gen.get_bytecode().back() != Instruction::RET) {
        gen.emit(Instruction::RET);
    }
}

void DeclareVariable::generate(CodeGenerator& gen) {
    gen.get_symbols().add_symbol(name, type);
    if (init) {
        init->generate(gen);
        Symbol* sym = gen.get_symbols().lookup(name);
        if (sym) {
            // local variables are at negative offsets from FP (FP - offset)
            gen.emit(Instruction::STORE_REG_OFF, (uint8_t)5, (uint8_t)sym->offset);
            gen.emit((uint8_t)0);
        }
    }
}

void InitVariable::generate(CodeGenerator& gen) {
    if (init) {
        init->generate(gen);
        Symbol* sym = gen.get_symbols().lookup(name);
        if (sym) {
            gen.emit(Instruction::STORE_REG_OFF, (uint8_t)5, (uint8_t)sym->offset);
            gen.emit((uint8_t)0);
        }
    }
}

void NumberNode::generate(CodeGenerator& gen) {
    gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)value);
}

void PrintNode::generate(CodeGenerator& gen) {
    if (expression) {
        expression->generate(gen);
        gen.emit(Instruction::PRINT_REG, (uint8_t)0);
    }
}

void ReturnNode::generate(CodeGenerator& gen) {
    if (expression) {
        expression->generate(gen);
    }
    gen.emit(Instruction::RET);
}

void BinaryExpression::generate(CodeGenerator& gen) {
    if (left) left->generate(gen);
    
    if (right) {
        gen.emit(Instruction::PUSH, (uint8_t)0);
        right->generate(gen);
        // After this: R0 = right, R1 = left (popped from stack)
        gen.emit(Instruction::POP_REG, (uint8_t)1);
    }
    
    if (op == "+") {
        // R0 = R0 + R1 = right + left (commutative, fine)
        gen.emit(Instruction::ADD_REG, (uint8_t)0, (uint8_t)1);
    } else if (op == "-") {
        // Want left - right. R1=left, R0=right.
        // SUB_REG(1,0): R1 = R1 - R0 = left - right. Then MOV R0, R1.
        gen.emit(Instruction::SUB_REG, (uint8_t)1, (uint8_t)0);
        gen.emit(Instruction::MOV, (uint8_t)0, (uint8_t)1);
    } else if (op == "*") {
        gen.emit(Instruction::MUL_REG, (uint8_t)0, (uint8_t)1);
    } else if (op == "/") {
        // Same fix as subtraction: DIV_REG(1,0) then MOV R0,R1
        gen.emit(Instruction::DIV_REG, (uint8_t)1, (uint8_t)0);
        gen.emit(Instruction::MOV, (uint8_t)0, (uint8_t)1);
    } else if (op == "<") {
        // CMP(0,1): last_value = R0-R1. non-zero (neg) when R0<R1. JZ skips body when >=.
        gen.emit(Instruction::CMP, (uint8_t)0, (uint8_t)1);
    } else if (op == ">") {
        // CMP(1,0): last_value = R1-R0. non-zero (pos) when R1>R0.
        gen.emit(Instruction::CMP, (uint8_t)1, (uint8_t)0);
    } else if (op == "==") {
        // CMP(1,0): last_value = R1 - R0 = left - right. 0 when equal.
        // IfNode must use JNZ (not JZ) so it skips the body when NOT equal.
        gen.emit(Instruction::CMP, (uint8_t)1, (uint8_t)0);
        gen.set_equality_condition(true);
    } else if (op == "++") {
        gen.emit(Instruction::LOAD, (uint8_t)1, 1);
        gen.emit(Instruction::ADD_REG, (uint8_t)0, (uint8_t)1);
    }
}

void ValueCondition::generate(CodeGenerator& gen) {
    Symbol* sym = gen.get_symbols().lookup(token.value);
    if (sym) {
        // cpu.stack[cpu.reg[base_reg] - offset]
        // if offset is negative, it becomes cpu.reg[base_reg] + abs(offset)
        gen.emit(Instruction::LOAD_REG_OFF, (uint8_t)0, (uint8_t)5);
        gen.emit((uint8_t)sym->offset);
    } else {
        try {
            int val = std::stoi(token.value);
            gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)val);
        } catch (...) {
            gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)0); 
        }
    }
}

void BooleanCondition::generate(CodeGenerator& gen) {
    if (token.value == "true") {
        gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)1);
    } else if (token.value == "false") {
        gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)0);
    } else {
        try {
            int val = std::stoi(token.value);
            gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)val);
        } catch (...) {
            gen.emit(Instruction::LOAD, (uint8_t)0, (uint8_t)0);
        }
    }
}

void IfNode::generate(CodeGenerator& gen) {
    if (condition) condition->generate(gen);
    
    // For == conditions, CMP gives 0 when equal (condition is TRUE).
    // We must skip the body when NOT equal, so use JNZ.
    // For all other conditions, 0 means false, so use JZ.
    bool is_eq = gen.consume_equality_condition();
    int jump_pos = gen.get_pos();
    gen.emit(is_eq ? Instruction::JNZ : Instruction::JZ, (uint8_t)0);
    
    if (body) body->generate(gen);
    
    int end_label = gen.get_pos();
    gen.write_at(jump_pos + 1, (uint8_t)end_label);
}

void WhileNode::generate(CodeGenerator& gen) {
    int start_label = gen.get_pos();
    if (condition) condition->generate(gen);
    
    int jump_pos = gen.get_pos();
    gen.emit(Instruction::JZ, (uint8_t)0);
    
    if (body) body->generate(gen);
    gen.emit(Instruction::JUMP, (uint8_t)start_label);
    
    int end_label = gen.get_pos();
    gen.write_at(jump_pos + 1, (uint8_t)end_label);
}

void ForNode::generate(CodeGenerator& gen) {
    if (initialization) initialization->generate(gen);

    int start_pos = gen.get_pos();

    int jump_to_end_pos = -1;
    if (condition) {
        condition->generate(gen);
        jump_to_end_pos = gen.get_pos();
        gen.emit(Instruction::JZ, (uint8_t)0);
    }

    if (body) body->generate(gen);

    if (increment) increment->generate(gen);

    gen.emit(Instruction::JUMP, (uint8_t)start_pos);

    if (jump_to_end_pos != -1) {
        int end_pos = gen.get_pos();
        gen.write_at(jump_to_end_pos + 1, (uint8_t)end_pos);
    }
}

void FunctionCallNode::generate(CodeGenerator& gen) {
    for (auto& arg : arguments) {
        arg->generate(gen);
        gen.emit(Instruction::PUSH, (uint8_t)0);
    }
    
    int addr = 0;
    if (gen.get_function_labels().count(function_name)) {
        addr = gen.get_function_labels()[function_name];
    }
    gen.emit(Instruction::CALL, (uint8_t)addr);

    // Caller-cleanup: RET restores SP/FP but leaves the pushed arguments
    // orphaned on the stack. Pop them off here so the stack is balanced.
    // Without this, a saved intermediate (e.g. fib(n-1) result pushed by
    // BinaryExpression) would be shadowed by stale argument words.
    for (size_t i = 0; i < arguments.size(); ++i) {
        gen.emit(Instruction::POP_NOREG);
    }
}
