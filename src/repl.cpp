#include "./repl.hpp"
#include "./debug/debug.h"
#include "frontend/bytecode/bytecode.hpp"
#include "utils/debug.hpp"
#include <string>

void Repl::execute() {
  string input;

  cout << "BioRepl v1.0.0\n";


  BioChunk c = { 
    Instruction{
        .code = OP_VAR,
        .constant = "HELLO"
    },

    Instruction{
        .code = OP_EXIT,
        .constant = &"1"
    }
  };

  dissasembleChunk(c);

/*
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
  };*/
};
