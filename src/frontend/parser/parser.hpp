#ifndef RECURSIVE_DESCENT_HPP
#define RECURSIVE_DESCENT_HPP

#include "../lexer/lexer.hpp"
#include "../../common.hpp"
#include <string>
#include <variant>
#include <set>
#include <list>

namespace TreeNodes {
  struct ForLoopNode;
  struct FunctionCallNode;
  struct DataTypeNode;
  struct WhileLoopNode;
  struct IfConditionNode;
  struct ObjectPropertyNode;
  struct ObjectMethodNode;
  struct ObjectNode;
  struct LiteralNode;
  struct ExpressionNode;
  struct VariableDeclarationNode;
  struct BlockStatementNode;
  struct FunctionDeclarationNode;
  struct ParamNode;
  struct IdentifierNode;
  struct PrefixExpressionNode;
  struct PostfixExpressionNode;

  typedef std::variant<LiteralNode, IfConditionNode, ForLoopNode, WhileLoopNode, ObjectPropertyNode, ObjectMethodNode, ObjectNode, ParamNode, BlockStatementNode, VariableDeclarationNode, ExpressionNode, FunctionDeclarationNode, IdentifierNode, PostfixExpressionNode, PrefixExpressionNode, FunctionCallNode> Node; 

  struct DataTypeNode {
    std::string kind = "DataTypeNode"; 
    DataType typeValue;
    // if the data type is of interface, then we need to refrence that interface via ident.
    std::string *ident = nullptr;
    LineInfo position;
  };

  struct ObjectMethodNode {
    std::string kind = "ObjectMethodNode";
    bool isPrivate;
    Node *method;
    LineInfo position;
  };

  struct ObjectPropertyNode {
    std::string kind = "ObjectPropertyNode";
    bool isPrivate;
    Node *data;
    LineInfo position;
  };

  typedef std::variant<ObjectPropertyNode, ObjectMethodNode> ObjectMemberNode;

  struct ObjectNode {
    std::string kind = "ObjectNode";
    Node *ident;
    std::list<ObjectMemberNode> fields;
    LineInfo position;
  };

  struct ParamNode {
    std::string kind = "ParameterNode";
    DataTypeNode type;
    Node *ident;
    LineInfo position;
  };

  struct LiteralNode {
    std::string kind = "PrimaryNode";
    DataTypeNode type;
    std::string value;
    LineInfo position;
  };
  
  // expression wont contain line number, but its elements will.

  struct ExpressionNode {
    std::string kind = "ExpressionNode";
    Node *lhs;
    std::string op;
    Node *rhs;
  };

  struct BlockStatementNode {
    std::string kind = "BlockStatementNode";
    std::list<Node> stmnts;
  };

  struct FunctionDeclarationNode {
    std::string kind = "FunctionDeclarationNode";
    Node *ident;
    DataTypeNode functionReturnType;
    std::list<ParamNode> params;
    BlockStatementNode block;
    LineInfo position;
  };

  struct VariableDeclarationNode {
    std::string kind = "VariableDeclarationNode";
    bool isConstant;
    DataTypeNode type;
    Node *ident;
    Node *value;
    LineInfo position;
  };

  struct FunctionCallNode {
    std::string kind = "FunctionCallNode";
    Node *called;
    // only stores idents or literals
    std::list<Node> arguments;
    LineInfo position;
  };

  struct ForLoopNode {
    std::string kind = "ForLoopNode";
    Node *initializer;
    Node *condition;
    Node *updater;
    BlockStatementNode block;
    LineInfo position;
  };

  struct WhileLoopNode {
    std::string kind = "WhileLoopNode";
    Node *condition;
    BlockStatementNode block;
    LineInfo position;
  };

  struct IfConditionNode {
    std::string kind = "IfConditionNode";
    Node *condition;
    BlockStatementNode block;
    LineInfo position;
  };

  struct PrefixExpressionNode {
    std::string kind = "PrefixExpressionNode";
    std::string op;
    Node *argument;
    LineInfo position;
  };

  struct PostfixExpressionNode {
    std::string kind = "PostfixExpressionNode";
    std::string op;
    Node *argument;
    LineInfo position;
  };

  struct IdentifierNode {
    std::string kind = "IdentifierNode";
    std::string name;
    LineInfo position;
  };

  struct Program {
    std::string kind = "Program";
    std::list<Node> body;
  };
};

// TODO: add parse return + parse fn/method call.

class BioParser {
  public:
    void setTokens(std::list<Token> tokens);
    TreeNodes::Program generateAST();
  private:
    std::list<Token> tokens;
    bool prefixSymbolExists(TokenIdentifiers id);
    void createExprNode(TreeNodes::Node &lhs);
    std::string expectedMssg(std::string exp);
    void expected(TokenIdentifiers token, std::string symb, bool eat = true);
    void customExpected(bool condition, std::string symb);
    void checkValidType();
    TreeNodes::Node parse();
    TreeNodes::Node parseVariable();
    TreeNodes::Node parseObject();
    std::list<TreeNodes::ObjectMemberNode> parseObjectFields();
    TreeNodes::Node parseIfStmnt();
    TreeNodes::Node parseForStmnt();
    TreeNodes::Node parseWhileStmnt();
    TreeNodes::BlockStatementNode parseBlockStmnt();
    std::list<TreeNodes::ParamNode> parseFunctionParams();
    TreeNodes::Node parseFunction();
    TreeNodes::Node parseAssignment();
    TreeNodes::Node parseMember();
    TreeNodes::Node parsePostfix();
    TreeNodes::Node parsePrefix();
    TreeNodes::Node parseExpression();
    TreeNodes::Node parseAdditives();
    TreeNodes::Node parseCall();
    TreeNodes::Node parseRelationalOps();
    TreeNodes::Node parseLogicalOps();
    TreeNodes::Node parseEqualityOps();
    TreeNodes::Node parseMultiplicatives();
    TreeNodes::Node parsePrimary();
    TreeNodes::DataTypeNode parseType();
    void eat();
    Token look();
};

#endif
