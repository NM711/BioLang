#ifndef SYMBOL_TABLE_HPP
#define SYMBOL_TABLE_HPP

#include <unordered_map>
#include <string>
#include <vector>

template<typename B>

using SymbolLookup = std::unordered_map<std::string, B>;

template<typename T>

/*
 The symbol table is a critical component that is used, during the stages of syntax analysis and even runtime for checking
 bindings. At its most simplest form it is a datastructure that is used for managing different scopes and looking into the symbols
 associated with each of the given scopes.
*/

class SymbolTable {

  private:
    // using vector instead of stack so that we can look through indexes.
    std::vector<SymbolLookup<T>> entries;
    unsigned index;
    SymbolLookup<T> current;
  public:
    SymbolTable();
    void popEntry();
    void pushEntry();
    void declare(std::string ident, T data);
    T *resolve(std::string ident);
};


#endif
