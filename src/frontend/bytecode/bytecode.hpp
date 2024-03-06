#ifndef BYTECODE_H
#define BYTECODE_H

#include <stack>
#include <variant>
#include "../parser/parser.hpp"

typedef variant<float, int, string> Value;

class Data {
  public:
    Value value;

    bool isInt() {
      return holds_alternative<int>(value);
    };

    bool isFloat() {
      return holds_alternative<float>(value);
    };

    bool isString() {
      return holds_alternative<string>(value);
    };
};

struct Instruction {
  unsigned short code;
  Data *data;
};

// Operation codes for our bytecode

enum OperationCode {
  OP_EXIT,
  OP_RETURN,
  // Operators
  OP_EQ,
  OP_ADD,
  OP_SUB,
  OP_DIV,
  OP_MULT,
  OP_MOD,
  OP_INCREMENT,
  OP_DECREMENT,
  OP_EQUALITY,
  OP_NOT_EQUALITY,
  OP_GT_EQ,
  OP_GT,
  OP_LS_EQ,
  OP_LS,
  // Keywords
  OP_VAR,
  OP_CONST,
  OP_VALUE,
  OP_BREAK,
  OP_CONTINUE,
  OP_IF,
  OP_FOR,
  OP_WHILE,
  OP_STRUCT,
  OP_TYPE,
  OP_ID,
  OP_EXPR_CALL
};

// we are gonna pass this full stack to the vm, the vm will read the stack check if some chunks include some async keyword
// then these chunks will be passed onto a queue within the vm for execution.

class BytecodeCompiler {
  private:
    list<Instruction> chunk;
    TreeNodes::Program source;
    unsigned short compileOperator(string op);
    void compileNode(TreeNodes::Node &node);
    void compileLiteral(TreeNodes::LiteralNode &literal);
    Instruction generateInstruction(unsigned short code, Data *data);
  public:
    BytecodeCompiler();
    void setSource(TreeNodes::Program program);
    list<Instruction> getInstructions();
    void compile();
};

#endif
