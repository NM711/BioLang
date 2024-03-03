#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>
#include <stack>
#include "../parser/parser.hpp"

typedef double Value;

struct Instruction {
  unsigned short code;
  Value *constant;
};

// Data representation for bytecode sequence

typedef vector<Instruction> BioChunk;

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
  OPINCREMENT,
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
    stack<BioChunk> chunks;
    TreeNodes::Program source;

  public:
    void setSource(TreeNodes::Program program);
    stack<BioChunk> compile;
};

#endif
