#include "./symb.table.hpp"
#include <iostream>

template<typename T>

SymbolTable<T>::SymbolTable() {
  this->index = 0;
  this->entries.push_back({});
  this->current = this->entries[this->index];
};

template<typename T>

void SymbolTable<T>::pushEntry() {
  ++this->index;
  this->entries.push_back(current);
};

template<typename T>

void SymbolTable<T>::popEntry() {
  --this->index;
  this->entries.pop_back();
};

template<typename T>

void SymbolTable<T>::declare(std::string ident, T data) {
  
  if (this->current[ident] != nullptr) {
    // throw a proper exception later.
    std::cout << "SymbolResolutionError: Symbol already defined!\n";
    exit(1);
  };
  
  this->current[ident] = data;
};

template<typename T>

T *SymbolTable<T>::resolve(std::string ident) {
  for (SymbolLookup<T> entry : this->entries) {
    return entry[ident];    
  };
};
