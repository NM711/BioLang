#ifndef RECURSIVE_DESCENT_HPP
#define RECURSIVE_DESCENT_HPP

#include "lexer.hpp"
#include <list>
#include <unordered_map>

class Node {
  public:
    Node(std::string kind);

  private:
    std::string kind;
};

class BioParser {
  public:
    void setTokens(list<Token> tokens);

  private:
    list<Token> tokens;
    void parseExpression();
    void parseAdditives();
    void parseMultiplicatives();
    void parsePrimary();
    Token eat();
    Token look();
};

#endif
