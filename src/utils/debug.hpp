#ifndef SRC_DEBUG_H
#define SRC_DEBUG_H

#include "../frontend/parser.hpp"
#include <variant>
#include <string>

void logTree(TreeNodes::Program tree);
void logWarning(string str);
#endif
