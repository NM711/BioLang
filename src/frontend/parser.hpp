#ifndef RECURSIVE_DESCENT_HPP
#define RECURSIVE_DESCENT_HPP

#include "./lexer.hpp"
#include <string>
#include <variant>
#include <set>
#include <list>

namespace TreeNodes {

  struct LiteralNode;
  struct ExpressionNode;
  struct VariableNode;
  struct BlockStatementNode;
  struct FunctionNode;
  struct IdentifierNode;
  struct PrefixExpressionNode;
  struct PostfixExpressionNode;

  typedef variant<LiteralNode, BlockStatementNode, FunctionNode, ExpressionNode, VariableNode, IdentifierNode, PostfixExpressionNode, PrefixExpressionNode> Node; 

  struct LiteralNode {
    string kind = "PrimaryNode";
    string type;
    string value;
  };

  struct ExpressionNode {
    string kind = "ExpressionNode";
    Node *lhs;
    string op;
    Node *rhs;
  };

  struct BlockStatementNode {
    string kind = "BlockStatementNode";
    list<Node> stmnts;
  };

  struct FunctionNode {
    string kind = "FunctionNode";
    IdentifierNode *ident;
    list<Node> arguments;
    BlockStatementNode block;
  };

  struct VariableNode {
    string kind = "VariableNode";
    bool isConstant;
    string type;
    IdentifierNode *ident;
    Node *value;
  };

  struct PrefixExpressionNode {
    string kind = "PrefixExpressionNode";
    string op;
    Node *argument;
  };

  struct PostfixExpressionNode {
    string kind = "PostfixExpressionNode";
    string op;
    Node *argument;
  };

  struct IdentifierNode {
    string kind = "IdentifierNode";
    string name;
  };

  struct Program {
    string kind = "Program";
    list<Node> body;
  };
};

class BioParser {
  public:
    void setTokens(list<Token> tokens);
    TreeNodes::Program generateAST();
  private:
    list<Token> tokens;
    bool prefixSymbolExists(TokenIdentifiers id);
    void createExprNode(TreeNodes::Node &lhs);
    string expectedMssg(string exp);
    void expected(TokenIdentifiers token, string symb);
    void customExpected(bool condition, string symb);
    void checkValidType();
    TreeNodes::Node parse();
    TreeNodes::Node parseVariable();
    TreeNodes::Node parseObject();
    TreeNodes::BlockStatementNode parseBlockStmnt();
    TreeNodes::Node parseFunction();
    TreeNodes::Node parseAssignment();
    TreeNodes::Node parsePostfix();
    TreeNodes::Node parsePrefix();
    TreeNodes::Node parseExpression();
    TreeNodes::Node parseAdditives();
    TreeNodes::Node parseRelationalOps();
    TreeNodes::Node parseLogicalOps();
    TreeNodes::Node parseEqualityOps();
    TreeNodes::Node parseMultiplicatives();
    TreeNodes::Node parsePrimary();
    void eat();
    Token peek();
    Token look();
};

#endif
