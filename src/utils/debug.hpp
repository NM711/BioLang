#ifndef SRC_DEBUG_HPP
#define SRC_DEBUG_HPP

#include "../frontend/parser/parser.hpp"
#include "../frontend/bytecode/bytecode.hpp"
#include <variant>
#include <string>

void dissasembleChunk(list<Instruction> &chunk);
void dissasembleInstruction(Instruction &instruction);
void logTree(TreeNodes::Program tree);
void logWarning(string str);
#endif
