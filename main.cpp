#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include "./frontend/lexer.hpp"
using namespace std;

int main() {

  ifstream input;
  input.open("./examples/test#1.bio");

  if (input.is_open() == false) {
    cout << "File failed to open!\n";
    exit(1);
  };

  string data;
  string line;

  while (getline(input, line)) {
    data += line + "\n";
  };

  BioLexer lexer = BioLexer();

  lexer.setInput(data);

  lexer.tokenize();
  return 0;
};
