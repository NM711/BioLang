#ifndef BYTECODE_H
#define BYTECODE_H

#include <vector>

typedef double Value;

struct BioChunk {
  unsigned short bytecode;
};

struct BioValue : BioChunk {
  Value value;
};

typedef std::vector<BioChunk> ChunkVector; 

enum OperationCode {
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
  OP_NOT,
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
  INST_STMNT_END
};

#endif
