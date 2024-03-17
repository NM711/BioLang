#ifndef COMMON_HPP
#define COMMON_HPP

#include <variant>
#include <string>

typedef std::variant<std::string, int, float> Value;

enum DataType {
  T_STRING,
  T_INTEGER,
  T_FLOAT,
  T_NULL,
  T_BOOLEAN,
  T_VOID,
  T_OBJECT_REF,
};

enum SymbolIdentifier {
  SYMB_VARIABLE,
  SYMB_OBJECT,
  SYMB_FUNCTION
};

#endif
