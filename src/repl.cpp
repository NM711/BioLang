#include "./repl.hpp"
// #include "utils/debug.hpp"
#include <string>

void Repl::execute() {
  string input;

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

    // this->compiler.setSource(tree);
    // this->compiler.compile();

    // auto chunk = this->compiler.getInstructions();
    // dissasembleChunk(chunk);

    // vm.setChunk(chunk);
    // vm.execute();

    /* logTree(tree); */
  };
};
