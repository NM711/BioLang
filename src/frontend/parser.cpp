#include "./parser.hpp"
#include "../utils/debug.hpp"
#include "./lexer.hpp"
#include <cmath>
#include <math.h>
#include <new>
#include <variant>

// convert tokens to tree, do semantic analysis and convert the tree into bytecode.

void BioParser::setTokens(list<Token> tokens) {
  this->tokens = tokens;
};

void BioParser::eat() {
  this->tokens.pop_front();
};

 Token BioParser::look() {
   return this->tokens.front();
};

Token BioParser::peek() {
  auto next = std::next(this->tokens.begin());

  return Token{
    next->id,
    next->lexeme,
    next->info
  };
};

TreeNodes::Node BioParser::parsePrimary() {

  switch (this->look().id) {
      case BooleanLiteral:
        if (this->look().lexeme == "0") {
          return TreeNodes::LiteralNode{.type = "BOOL", .value = "false"};
        } else {
          return TreeNodes::LiteralNode{.type = "BOOL", .value = "true"};
        };
      case StringLiteral:
        return TreeNodes::LiteralNode{.type = "STRING", .value = this->look().lexeme}; 
      case IntegerLiteral:
        return TreeNodes::LiteralNode{.type = "INT", .value = this->look().lexeme};
      case FloatLiteral:
        return TreeNodes::LiteralNode{.type = "FLOAT", .value = this->look().lexeme};
      case EndOfFile:
        return TreeNodes::LiteralNode{.type = "EOF", .value = this->look().lexeme};
      case Ident:
        return TreeNodes::IdentifierNode{.name = this->look().lexeme};
      default:
        cout << "Unexpected token id found!" << " (\"" << this->look().lexeme << "\") at (col: " << this->look().info.col << ", row: " << this->look().info.row << ")\n";
        exit(1);
  };
};

TreeNodes::Node BioParser::parseUpdater() {

  TreeNodes::Node lhs;

  // postfix
  // prefix
  // just parse primary if none

  if (this->look().id != Increment && this->look().id != Decrement) {
    lhs = this->parsePrimary();
    this->eat();

    if (this->look().id == Increment || this->look().id == Decrement) {

      std::string op = this->look().lexeme;
      this->eat();

      if (!holds_alternative<TreeNodes::IdentifierNode>(lhs)) {
        throw "Expected a valid identifier before the post increment/decrement operator!";
      };


      TreeNodes::UpdateExpressionNode updater = TreeNodes::UpdateExpressionNode(); 

      updater.isPrefix = false;
      updater.op = op;
      updater.argument = new TreeNodes::Node(lhs);

      lhs = updater;
    }

  } else if (this->look().id == Increment || this->look().id == Decrement) {
      std::string op = this->look().lexeme;
      this->eat();

      TreeNodes::Node ident = this->parsePrimary();
       
      if (!holds_alternative<TreeNodes::IdentifierNode>(lhs)) {
        throw "Expected a valid identifier after the prefix increment/decrement operator!";
      };

      TreeNodes::UpdateExpressionNode updater = TreeNodes::UpdateExpressionNode(); 

      updater.isPrefix = false;
      updater.op = op;
      updater.argument = new TreeNodes::Node(lhs);

      lhs = updater;
  } else {
    lhs = this->parsePrimary();
  }

  return lhs;
};


TreeNodes::Node BioParser::parseMultiplicatives() {

  TreeNodes::Node lhs = this->parsePrimary();

  // focus on this eat below

  this->eat();


  cout << "LOOK LEXEME: " << endl;

  while (this->look().lexeme == "*" || this->look().lexeme == "/" || this->look().lexeme == "%") {
    std::string op = this->look().lexeme;
    cout << "RETURNING IN MULTI\n";
    this->eat();

    TreeNodes::Node rhs = this->parsePrimary();
    this->eat();

    TreeNodes::ExpressionNode expr;

    expr.lhs = new TreeNodes::Node(lhs);
    expr.op = op;
    expr.rhs = new TreeNodes::Node(rhs);

    lhs = expr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseAdditives() {

  TreeNodes::Node lhs = this->parseMultiplicatives();

  while (this->look().lexeme == "+" || this->look().lexeme == "-") {
    std::string op = this->look().lexeme;
    cout << "RETURNING IN ADDITIVE\n";
    this->eat();

    TreeNodes::Node rhs = this->parsePrimary();
    
    this->eat();

    TreeNodes::ExpressionNode expr;

    // so from what im understanding, assigning expr.lhs to &lhs and expr.rhs to &rhs only works within the scope
    // of the function. This is due to c++ automatically deallocating memory once we are out of this function,
    // so the addresses we assigned are then automatically given undefined behaviour.
    // So it seems like we may need a level  of dynamic memory.

    expr.lhs = new TreeNodes::Node(lhs);
    expr.op = op;
    expr.rhs = new TreeNodes::Node(rhs);

    lhs = expr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseExpression() {
  return this->parseAdditives();
};

TreeNodes::Node BioParser::parse() {
  return this->parseExpression();
};

TreeNodes::Program BioParser::generateAST() {

  // note that it has to be greater than one because EOF is automatically appended to the token list, before the lexer is exited.
  TreeNodes::Program program;

  program.body = {};

  program.body.push_back(this->parse());

  cout << "END OF PARSER\n";

  return program;
};
