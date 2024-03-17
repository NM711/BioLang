#include "./semantic.hpp"
#include "../../errors.hpp"
#include <variant>

SemanticAnalyzer::SemanticAnalyzer() {
  this->table = SymbolTable<SemanticNode>();
};

void SemanticAnalyzer::setSource(TreeNodes::Program source) {
  this->source = source;
};

void SemanticAnalyzer::analyzeAssignment(const TreeNodes::ExpressionNode &expr) {
  TreeNodes::IdentifierNode ident = std::get<TreeNodes::IdentifierNode>(*expr.lhs);
  SemanticNode *retrieved = this->table.resolve(ident.name);

  std::string message;

  // exits should be replaced for exceptions.

  if (retrieved == nullptr) {
    message = std::string("Variable with identifier of ") + "\" " + ident.name + std::string("\" does not exist!\n");
    throw FrontendException::StaticError(message, ident.position);
  };

  // example: fn hi(){# stuff here};
  // hi = 10 -> invalid

  if (std::holds_alternative<SemanticVariable>(*retrieved) == false) {
    message = std::string("Identifier \"") + ident.name + std::string("\" is not binded to a variable, therefore assignment is invalid!");
    throw FrontendException::StaticError(message, ident.position);
  };

  SemanticVariable retrievedVar = std::get<SemanticVariable>(*retrieved);

  if (retrievedVar.isConst) {
    message = std::string("Cannot reassign const at \"") + ident.name + std::string("\"\n");
    throw FrontendException::StaticError(message, ident.position); 
  };
  
};

void SemanticAnalyzer::analyzeExpression(const TreeNodes::ExpressionNode &expr) {
  if (expr.kind == "AssignmentExpressionNode") {
    
  } else if (expr.kind == "ExpressionNode") {
    
  };
};

void SemanticAnalyzer::analyzeNode(const TreeNodes::Node &node) {
  if (std::holds_alternative<TreeNodes::VariableDeclarationNode>(node)) {
    TreeNodes::VariableDeclarationNode variable = std::get<TreeNodes::VariableDeclarationNode>(node);
    TreeNodes::IdentifierNode ident = std::get<TreeNodes::IdentifierNode>(*variable.ident);

    SemanticVariable body = { .isConst = variable.isConstant, .type = variable.type.typeValue,  .typeName = variable.type.ident };

    this->table.declare(ident.name, body);
  } else if (std::holds_alternative<TreeNodes::FunctionDeclarationNode>(node)) {
      
  } else if (std::holds_alternative<TreeNodes::ExpressionNode>(node)) {
    TreeNodes::ExpressionNode expr = std::get<TreeNodes::ExpressionNode>(node);
    this->analyzeAssignment(expr);
  };
};

void SemanticAnalyzer::analyze() {
  try {
    
    for (const auto &node : this->source.body) {
      this->analyzeNode(node);
    };
     
  } catch (FrontendException::StaticError error) {
      std::cout << error.what() << std::endl;
      exit(1);
  };
};

