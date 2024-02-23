#ifndef RECURSIVE_DESCENT_HPP
#define RECURSIVE_DESCENT_HPP

#include "./lexer.hpp"
#include <string>
#include <variant>
#include <list>

namespace TreeNodes {

  struct LiteralNode;
  struct ExpressionNode;
  struct VariableNode;
  struct UpdateExpressionNode;
  struct IdentifierNode;

  typedef variant<LiteralNode, ExpressionNode, VariableNode, UpdateExpressionNode, IdentifierNode> Node; 

  struct LiteralNode {
    std::string kind = "PrimaryNode";
    std::string type;
    std::string value;
  };

  struct ExpressionNode {
    std::string kind = "ExpressionNode";
    Node *lhs;
    std::string op;
    Node *rhs;
  };

  struct VariableNode {
    std::string kind = "VariableNode";
    bool isConstant;
    std::string type;
    LiteralNode value;
  };

  struct UpdateExpressionNode {
    std::string kind = "UpdateExpressionNode";
    bool isPrefix;
    std::string op;
    Node *argument;
  };

  struct IdentifierNode {
    std::string kind = "IdentifierNode";
    std::string name;
  };

  struct Program {
    std::string kind = "Program";
    std::list<Node> body;
  };
};

class BioParser {
  public:
    void setTokens(list<Token> tokens);
    TreeNodes::Program generateAST();
  private:
    list<Token> tokens;
    TreeNodes::Node parse();
    TreeNodes::Node parseUpdater();
    TreeNodes::Node parseExpression();
    TreeNodes::Node parseAdditives();
    TreeNodes::Node parseMultiplicatives();
    TreeNodes::Node parsePrimary();
    void eat();
    Token peek();
    Token look();
};

#endif
