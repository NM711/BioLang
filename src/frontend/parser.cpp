#include "./parser.hpp"
#include "../utils/debug.hpp"
#include "./lexer.hpp"
#include <cmath>
#include <cstdlib>
#include <math.h>
#include <new>
#include <variant>

void BioParser::setTokens(list<Token> tokens) {
  this->tokens = tokens;
};

void BioParser::eat() {
  cout << "SIZE: " << this->tokens.size() << endl;
  if (this->tokens.size() != 1) {
    return this->tokens.pop_front();
  };
};

 Token BioParser::look() {
   cout << "LOOK: " << this->tokens.front().lexeme << endl;
   return this->tokens.front();
};

Token BioParser::peek() {

  if (this->tokens.size() != 1) {
    auto next = std::next(this->tokens.begin());

    return Token{
      next->id,
      next->lexeme,
      next->info
    };
  };
  return this->look();
};

bool BioParser::prefixSymbolExists(TokenIdentifiers id) {
  const set<TokenIdentifiers> symbolsSet = {Exclamation, Increment, Decrement};
  return symbolsSet.find(id) != symbolsSet.end();
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

TreeNodes::Node BioParser::parsePostfix() {
  TreeNodes::Node lhs;

  if (this->prefixSymbolExists(this->look().id) == false) {
    lhs = this->parsePrimary();
    this->eat();

    if (this->prefixSymbolExists(this->look().id) && this->look().id != Exclamation) {
      string op = this->look().lexeme;
      this->eat();

      if (!holds_alternative<TreeNodes::IdentifierNode>(lhs)) {
        throw SyntaxError("Expected a valid identifier before the postfix operator!", this->look().info);
      };

      TreeNodes::UpdateExpressionNode updater = TreeNodes::UpdateExpressionNode(); 

      updater.isPrefix = false;
      updater.op = op;
      updater.argument = new TreeNodes::Node(lhs);

      lhs = updater;
    };
  };

  return lhs;
};

TreeNodes::Node BioParser::parsePrefix() {
  TreeNodes::Node lhs = this->parsePostfix();

  if (this->prefixSymbolExists(this->look().id)) {
      std::string op = this->look().lexeme;
      this->eat();
      TreeNodes::Node ident = this->parsePrimary();
      this->eat();

      if (!holds_alternative<TreeNodes::IdentifierNode>(ident)) {
        throw SyntaxError("Expected a valid identifier after the prefix operator!", this->look().info);
      };

      TreeNodes::UpdateExpressionNode updater;

      updater.isPrefix = true;
      updater.op = op;
      updater.argument = new TreeNodes::Node(ident);

      lhs = updater;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseMultiplicatives() {

  TreeNodes::Node lhs = this->parsePrefix();

  // focus on this eat below

  this->eat();

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

TreeNodes::Node BioParser::parseRelationalOps() {
  TreeNodes::Node lhs = this->parseAdditives();
  
  if (this->look().id == GreaterThan || this->look().id == GreaterThanOrEqual || this->look().id == LesserThan || this->look().id == LesserThanOrEqual) {
    string op = this->look().lexeme;

    this->eat();

    TreeNodes::Node rhs = this->parsePrimary();

    TreeNodes::ExpressionNode expr;

    expr.lhs = new TreeNodes::Node(lhs);
    expr.op = op;
    expr.rhs = new TreeNodes::Node(rhs);

    lhs = expr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseEqualityOps() {
  TreeNodes::Node lhs = this->parseRelationalOps();

  if (this->look().id == Equality || this->look().id == NotEquality) {
    string op = this->look().lexeme;

    this->eat();

    TreeNodes::Node rhs = this->parsePrimary();

    TreeNodes::ExpressionNode expr;

    expr.lhs = new TreeNodes::Node(lhs);
    expr.op = op;
    expr.rhs = new TreeNodes::Node(rhs);

    lhs = expr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseLogicalOps() {
  TreeNodes::Node lhs = this->parseEqualityOps();
  // parses && and || operators
  
  if (this->look().id == Or || this->look().id == And) {
    string op = this->look().lexeme;
    this->eat();
    TreeNodes::Node rhs = this->parsePrimary();

    TreeNodes::ExpressionNode expr;

    expr.lhs = new TreeNodes::Node(lhs);
    expr.op = op;
    expr.rhs = new TreeNodes::Node(rhs);

    lhs = expr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseExpression() {
  return this->parseLogicalOps();
};

TreeNodes::Node BioParser::parse() {
  try {
    return this->parseExpression();
  } catch (SyntaxError error) {
    error.errorMssg();
    exit(1);
  };
};

TreeNodes::Program BioParser::generateAST() {
  TreeNodes::Program program;

  program.body = {};

  program.body.push_back(this->parse());

  cout << "END OF PARSER\n";

  return program;
};
