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
  if (this->tokens.size() > 1) {
    this->tokens.pop_front();
  };
};

 Token BioParser::look() {
   return this->tokens.front();
};

Token BioParser::peek() {

  if (this->tokens.size() > 1) {
    auto next = std::next(this->tokens.begin());

    return Token{
      next->id,
      next->lexeme,
      next->info
    };
  };
  return this->look();
};

string BioParser::expectedMssg(string exp) {
  return string("Expected a") + "\" " + exp + "\"" + string(", instead received a ") + "\"" + this->look().lexeme + "\"!";
};

bool BioParser::prefixSymbolExists(TokenIdentifiers id) {
  const set<TokenIdentifiers> symbolsSet = {Exclamation, Increment, Decrement};
  return symbolsSet.find(id) != symbolsSet.end();
};

void BioParser::checkValidType() {
  if (this->look().id != Void && this->look().id != String && this->look().id != Integer && this->look().id != Float && this->look().id != Object) {
    throw SyntaxError(this->expectedMssg("VALID TYPE"), this->look().info);
  };
};

void BioParser::createExprNode(TreeNodes::Node &lhs) {
  std::string op = this->look().lexeme;
  this->eat();

  TreeNodes::Node rhs = this->parseExpression();
  this->eat();

  // so from what im understanding, assigning expr.lhs to &lhs and expr.rhs to &rhs only works within the scope
  // of the function. This is due to c++ automatically deallocating memory once we are out of this function,
  // so the addresses we assigned are then automatically given undefined behaviour.
  // So it seems like we may need a level  of dynamic memory.

  TreeNodes::ExpressionNode expr;
  expr.lhs = new TreeNodes::Node(lhs);
  expr.op = op;
  expr.rhs = new TreeNodes::Node(rhs);

  lhs = expr;
};

TreeNodes::Node BioParser::parseVariable() {
  bool isConstant = false;

  if (this->look().id == Const) {
    isConstant = true;
  };

  this->eat();

  TreeNodes::Node ident = this->parsePrimary();

  if (!holds_alternative<TreeNodes::IdentifierNode>(ident)) {
    throw SyntaxError(this->expectedMssg("identifier"), this->look().info);
  };

  this->eat();

  if (this->look().id != Colon) {
    throw SyntaxError(this->expectedMssg(":"), this->look().info);
  };

  this->eat();

  this->checkValidType();

  string varType = this->look().lexeme;

  if (this->look().id == Void && isConstant) {
    throw SyntaxError(this->expectedMssg("VALID TYPE"), this->look().info);
  };

  this->eat();

  // check dec end before assignment, and make sure its not const if so.
 
  if (this->look().id != Equal) {
  
     if (this->look().id == Semicolon && isConstant) {
       cout << "Constant must be initialized!\n";
       exit(1);
     } else {
       TreeNodes::VariableNode varNode;

       varNode.isConstant = isConstant;
       varNode.type = varType;

       return varNode;
     };

  };

  if (this->look().id == Semicolon && isConstant) {
    cout << "Constant must be initialized!\n";
    exit(1);
  } else {
    TreeNodes::VariableNode varNode;

    varNode.isConstant = isConstant;
    varNode.type = varType;

    return varNode;
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

      TreeNodes::PostfixExpressionNode postfix = TreeNodes::PostfixExpressionNode();

      postfix.op = op;
      postfix.argument = new TreeNodes::Node(lhs);

      lhs = postfix;
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

      TreeNodes::PrefixExpressionNode prefix;

      prefix.op = op;
      prefix.argument = new TreeNodes::Node(ident);

      lhs = prefix;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseMultiplicatives() {

  TreeNodes::Node lhs = this->parsePrefix();

  while (this->look().id == Multiplication || this->look().id == Division || this->look().id == Modulus) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseAdditives() {

  TreeNodes::Node lhs = this->parseMultiplicatives();
 
  while (this->look().id == Addition || this->look().id == Subtraction) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseRelationalOps() {
  TreeNodes::Node lhs = this->parseAdditives();
  
  while (this->look().id == GreaterThan || this->look().id == GreaterThanOrEqual || this->look().id == LesserThan || this->look().id == LesserThanOrEqual) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseEqualityOps() {
  TreeNodes::Node lhs = this->parseRelationalOps();

  while (this->look().id == Equality || this->look().id == NotEquality) {
    this->createExprNode(lhs); 
  };

  return lhs;
};

TreeNodes::Node BioParser::parseLogicalOps() {
  TreeNodes::Node lhs = this->parseEqualityOps();
 
  while (this->look().id == Or || this->look().id == And) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseAssignment() {
  TreeNodes::Node lhs = this->parseLogicalOps();

  if (holds_alternative<TreeNodes::IdentifierNode>(lhs) && this->look().id == Equal) {

    string op = this->look().lexeme;
    this->eat();
    TreeNodes::Node rhs = this->parseExpression();
    TreeNodes::ExpressionNode assignmentExpr;

    const TreeNodes::IdentifierNode arg = get<TreeNodes::IdentifierNode>(lhs);

    assignmentExpr.kind = "AssignmentExpressionNode";
    assignmentExpr.lhs = new TreeNodes::Node(lhs);
    assignmentExpr.op = op;
    assignmentExpr.rhs = new TreeNodes::Node(rhs);

    lhs = assignmentExpr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseExpression() {
  return this->parseAssignment();
};

TreeNodes::Node BioParser::parse() {
  try {

    switch (this->look().id) {
      /* case Const: */
      /* case Var: */
      /* break; */

      default:
        return this->parseExpression();
    }

  } catch (SyntaxError error) {
    error.errorMssg();
    exit(1);
  };
};

TreeNodes::Program BioParser::generateAST() {
  TreeNodes::Program program;

  program.body = {};

  while (this->tokens.size() > 1) {
    program.body.push_back(this->parse());
  };

  return program;
};
