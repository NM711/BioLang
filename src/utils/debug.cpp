#include "./debug.hpp"
#include <typeinfo>
#include <variant>
#include "./colors.hpp"

void logWarning(string str) {
  cout << YELLOW << "WARNING: " << str << DEFAULT << endl;
};

void logNode(TreeNodes::Node node);

void logSide(TreeNodes::Node *node, string side) {

  if (node == nullptr) {
    cout << "NullBranch\n";
  };

  // Pretty printing requires a bit of work but eh, fuck it.
  // It is good enough for my use case

  cout << side << YELLOW << ": (\n\t";
  logNode(*node);
  cout << "), \n\t" << DEFAULT;
};

void logNode(TreeNodes::Node node) {
  if (holds_alternative<TreeNodes::LiteralNode>(node)) {

    TreeNodes::LiteralNode literal = get<TreeNodes::LiteralNode>(node);

      cout << "Kind: " << literal.kind << ", ";
      cout << "Type: " << literal.type << ", ";
      cout << "Value: " << literal.value << " ";

    } else if (holds_alternative<TreeNodes::IdentifierNode>(node)) {
      TreeNodes::IdentifierNode ident = get<TreeNodes::IdentifierNode>(node);

      cout << "Kind: " << ident.kind << ", ";
      cout << "Name: " << ident.name << " ";

    } else if (holds_alternative<TreeNodes::ExpressionNode>(node)) {
      TreeNodes::ExpressionNode expr = get<TreeNodes::ExpressionNode>(node);
      cout << "Kind: " << expr.kind << ", "; 
      logSide(expr.lhs, "Lhs");
      cout << "Operator: " << expr.op << ", ";
      logSide(expr.rhs, "Rhs");
      cout << endl;
    } else if (holds_alternative<TreeNodes::PrefixExpressionNode>(node)) {
      TreeNodes::PrefixExpressionNode prefix = get<TreeNodes::PrefixExpressionNode>(node);
      cout << "Kind: " << prefix.kind << endl;
    
      cout << "Operator: " << prefix.op << endl;

      TreeNodes::IdentifierNode arg = get<TreeNodes::IdentifierNode>(*prefix.argument);
      cout << "Argument: " << arg.name << endl;

    } else if (holds_alternative<TreeNodes::PostfixExpressionNode>(node)) {
      TreeNodes::PostfixExpressionNode postfix = get<TreeNodes::PostfixExpressionNode>(node);
      cout << "Kind: " << postfix.kind << endl;
    
      cout << "Operator: " << postfix.op << endl;

      TreeNodes::IdentifierNode arg = get<TreeNodes::IdentifierNode>(*postfix.argument);
      cout << "Argument: " << arg.name << endl;
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
