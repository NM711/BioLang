#ifndef RECURSIVE_DESCENT_HPP
#define RECURSIVE_DESCENT_HPP

#include "./lexer.hpp"
#include <string>
#include <variant>
#include <set>
#include <list>

namespace TreeNodes {
  struct ForLoopNode;
  struct WhileLoopNode;
  struct IfConditionNode;
  struct ObjectPropertyNode;
  struct ObjectMethodNode;
  struct ObjectNode;
  struct LiteralNode;
  struct ExpressionNode;
  struct VariableNode;
  struct BlockStatementNode;
  struct FunctionNode;
  struct ParamNode;
  struct IdentifierNode;
  struct PrefixExpressionNode;
  struct PostfixExpressionNode;

  typedef variant<LiteralNode, IfConditionNode, ForLoopNode, WhileLoopNode, ObjectPropertyNode, ObjectMethodNode, ObjectNode, ParamNode, BlockStatementNode, FunctionNode, ExpressionNode, VariableNode, IdentifierNode, PostfixExpressionNode, PrefixExpressionNode> Node; 


  struct ObjectMethodNode {
    string kind = "ObjectMethodNode";
    bool isPrivate;
    FunctionNode *method;
  };

  struct ObjectPropertyNode {
    string kind = "ObjectPropertyNode";
    bool isPrivate;
    Node *ident;
    string type;
    Node *value;
  };

  typedef variant<ObjectPropertyNode, ObjectMethodNode> ObjectMemberNode;

  struct ObjectNode {
    string kind = "ObjectNode";
    Node *ident;
    list<ObjectPropertyNode> properties;
  };

  struct ParamNode {
    string kind = "ParameterNode";
    string type;
    Node *ident;
  };

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
    Node *ident;
    string functionReturnType;
    list<ParamNode> params;
    BlockStatementNode block;
  };

  struct VariableNode {
    string kind = "VariableNode";
    bool isConstant;
    string type;
    Node *ident;
    Node *value;
  };

  struct ForLoopNode {
    string kind = "ForLoopNode";
    Node *initializer;
    Node *condition;
    Node *updater;
    BlockStatementNode block;
  };

  struct WhileLoopNode {
    string kind = "WhileLoopNode";
    Node *condition;
    BlockStatementNode block;
  };

  struct IfConditionNode {
    string kind = "IfConditionNode";
    Node *condition;
    BlockStatementNode block;
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
    void expected(TokenIdentifiers token, string symb, bool eat = true);
    void customExpected(bool condition, string symb);
    void checkValidType();
    TreeNodes::Node parse();
    TreeNodes::Node parseVariable();
    TreeNodes::Node parseObject();
    list<TreeNodes::ObjectPropertyNode> parseObjectProperties();
    TreeNodes::Node parseIfStmnt();
    TreeNodes::Node parseForStmnt();
    TreeNodes::Node parseWhileStmnt();
    TreeNodes::BlockStatementNode parseBlockStmnt();
    list<TreeNodes::ParamNode> parseFunctionParams();
    TreeNodes::Node parseFunction();
    TreeNodes::Node parseAssignment();
    TreeNodes::Node parseMember();
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
    Token look();
};

#endif
