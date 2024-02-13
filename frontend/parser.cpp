#include "./parser.hpp"
#include <variant>

Node::Node(std::string kind) {
  this->kind = kind;
};

class ProgramNode : Node {
  public:
    std::list<Node> body;

  ProgramNode() : Node("ProgramNode") {
    this->body = {};
  };

  void setBody(Node node) {
    this->body.push_back(node);
  };
};

class VariableNode : public Node {
    private:
      bool isConstant;
      std::string type;
      std::string value;

    public: VariableNode(std::string type, bool isConstant, string value) : Node("VariableNode") {
      this->isConstant = isConstant;
      this->type = type;
      this->value = value;
    };
};

class AssignmentNode : Node {
  private:
    std::variant<string, Node> value;

  public:
    AssignmentNode(std::variant<string, Node> value) : Node("AssignmentNode") {
      this->value = value;
    };
};

class ExpressionNode : public Node {
    private:
      std::string op;
      std::variant<string, Node> lhs;
      std::variant<string, Node> rhs;

    public: ExpressionNode(std::string op, std::variant<string, Node> lhs, std::variant<string, Node> rhs) : Node("ExpressionNode") {
      this->op = op;
      this->lhs = lhs;
      this->rhs = rhs;
    };
};

class BlockNode : public Node {
    private:
      std::list<Node> body;

    public: 

      BlockNode() : Node("BlockNode") {
        this->body = {};
      };

      void setBody(Node node) {
          this->body.push_back(node);
      };
};

void BioParser::setTokens(list<Token> tokens) {
  this->tokens = tokens;
};



