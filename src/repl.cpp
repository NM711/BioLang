#include "./repl.hpp"
#include "./debug/debug.h"
#include "utils/debug.hpp"
#include <string>

void Repl::execute() {
  std::string input;
  
  cout << "BioRepl v1.0.0\n";

  while (true) {

    cout << "\n>>> ";
    getline(cin, input);

    lexer.setInput(input);

    auto const tokens = this->lexer.tokenize();

    for (auto &token : tokens) {
      cout << "TOKEN: " << token.lexeme << endl;
    };

    cout << input << endl;

    this->parser.setTokens(tokens);   
  
    auto tree = this->parser.generateAST();

    logTree(tree);
  };
};