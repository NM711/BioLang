#include "./debug.hpp"
#include <typeinfo>
#include <variant>


void logWarning(string str) {
  cout << "\033[33m" << "WARNING: " << str << "\033[0m" << endl;
};


void logNode(TreeNodes::Node node);

void logSide(TreeNodes::Node *node, string side) {

  if (node == nullptr) {
    cout << "NullBranch\n";
  };

  cout << side << ": {\n   ";
  logNode(*node);
  cout << "\n  },\n"; 

};

void logNode(TreeNodes::Node node) {
  if (holds_alternative<TreeNodes::LiteralNode>(node)) {

    TreeNodes::LiteralNode primary = std::get<TreeNodes::LiteralNode>(node);

      cout << "Kind: " << primary.kind << ", ";
      cout << "Type: " << primary.type << ", ";
      cout << "Value: " << primary.value << " ";

    } else if (holds_alternative<TreeNodes::ExpressionNode>(node)) {
      TreeNodes::ExpressionNode expr = std::get<TreeNodes::ExpressionNode>(node);

      cout << "Kind: " << expr.kind << ", ";

      logSide(expr.lhs, "Lhs");

      cout << "Operator: " << expr.op << ", ";
      
      logSide(expr.rhs, "Rhs"); 

      cout << endl;
    };
};

void logTree(TreeNodes::Program tree) {

  cout << "AST SIZE: " << tree.body.size() << endl;

  if (tree.body.size() > 0) {
  
    for (auto const &node : tree.body) {
      logNode(node);
    };

  } else {
    cout << "Tree Is Empty!\n";
  };
};
