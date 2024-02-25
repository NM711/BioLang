#ifndef SRC_DEBUG_HPP
#define SRC_DEBUG_HPP

#include "../frontend/parser.hpp"
#include <variant>
#include <string>
void logTree(TreeNodes::Program tree);
void logWarning(string str);
#endif
