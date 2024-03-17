#include "./debug.hpp"
#include "./colors.hpp"
#include <variant>
#include <iomanip>

void logInstruction(Instruction &instruction, std::string opname) {
  std::cout << instruction.code << " " << " " << opname << " " << " ";
  std::cout << std::endl;
};

void dissasembleInstruction(Instruction instruction) {

  switch (instruction.code) {
    case OP_ADD:
      logInstruction(instruction, "OP_ADD");
    break;

    case OP_VALUE:
      logInstruction(instruction, "OP_VALUE");
    break;

    case OP_ID:
      logInstruction(instruction, "OP_ID");
    break;

    case OP_BREAK:
      logInstruction(instruction, "OP_BREAK");
    break;

    case OP_CONTINUE:
      logInstruction(instruction, "OP_CONTINUE");
    break;

    case OP_VAR:
      logInstruction(instruction, "OP_VAR");
    break;

    case OP_CONST:
      logInstruction(instruction, "OP_CONST");
    break;

    case OP_RETURN:
      logInstruction(instruction, "OP_BREAK");
    break; 

    case OP_EXIT:
      logInstruction(instruction, "OP_EXIT");
    break;

    default:
      std::cout << "Unknown opcode!\n";
      exit(1);
  };
};

void dissasembleChunk(std::list<Instruction> &chunk) {
  std::cout << "==> CHUNK <==\n";

  // we will esentially iterate over the vector of codes within the chunk.
  // remember a chunk is simply a sequence of opcodes.

  for (const auto &instruction : chunk) {
    dissasembleInstruction(instruction);
  };

};

void logWarning(std::string str) {
  std::cout << YELLOW << "WARNING: " << str << DEFAULT << std::endl;
};

void logNode(TreeNodes::Node node);

void logSide(TreeNodes::Node *node, std::string side) {

  if (node == nullptr) {
    std::cout << "NullBranch\n";
  };

  // Pretty printing requires a bit of work but eh, fuck it.
  // It is good enough for my use case

  std::cout << side << YELLOW << ": (\n\t";
  logNode(*node);
  std::cout << "), \n\t" << DEFAULT;
};

void logNode(TreeNodes::Node node) {
  if (std::holds_alternative<TreeNodes::LiteralNode>(node)) {

    TreeNodes::LiteralNode literal = std::get<TreeNodes::LiteralNode>(node);

      std::cout << "Kind: " << literal.kind << ", ";
      std::cout << "Type: " << literal.type.typeValue << ", ";
      std::cout << "Value: " << literal.value << " ";

    } else if (std::holds_alternative<TreeNodes::IdentifierNode>(node)) {
      TreeNodes::IdentifierNode ident = std::get<TreeNodes::IdentifierNode>(node);

      std::cout << "Kind: " << ident.kind << ", ";
      std::cout << "Name: " << ident.name << " ";

    } else if (std::holds_alternative<TreeNodes::ExpressionNode>(node)) {
      TreeNodes::ExpressionNode expr = std::get<TreeNodes::ExpressionNode>(node);
      std::cout << "Kind: " << expr.kind << ", "; 
      logSide(expr.lhs, "Lhs");
      std::cout << "Operator: " << expr.op << ", ";
      logSide(expr.rhs, "Rhs");
      std::cout << std::endl;
    } else if (std::holds_alternative<TreeNodes::PrefixExpressionNode>(node)) {
      TreeNodes::PrefixExpressionNode prefix = std::get<TreeNodes::PrefixExpressionNode>(node);
      std::cout << "Kind: " << prefix.kind << std::endl;
    
      std::cout << "Operator: " << prefix.op << std::endl;

      TreeNodes::IdentifierNode arg = std::get<TreeNodes::IdentifierNode>(*prefix.argument);
      std::cout << "Argument: " << arg.name << std::endl;

    } else if (std::holds_alternative<TreeNodes::PostfixExpressionNode>(node)) {
      TreeNodes::PostfixExpressionNode postfix = get<TreeNodes::PostfixExpressionNode>(node);
      std::cout << "Kind: " << postfix.kind << std::endl;
    
      std::cout << "Operator: " << postfix.op << std::endl;

      TreeNodes::IdentifierNode arg = std::get<TreeNodes::IdentifierNode>(*postfix.argument);
      std::cout << "Argument: " << arg.name << std::endl;
    } else if (std::holds_alternative<TreeNodes::VariableDeclarationNode>(node)) {
      TreeNodes::VariableDeclarationNode variable = std::get<TreeNodes::VariableDeclarationNode>(node);
      std::cout << "Kind: " << variable.kind << std::endl;

      std::cout << "Variable_Type: " << variable.type.typeValue << std::endl;
      
      std::cout << "IsConstant: ";

      (variable.isConstant) ? std::cout << "true\n" : std::cout << "false\n";

      std::cout << "Value: " << YELLOW;

      logNode(*variable.value);
    } else if (std::holds_alternative<TreeNodes::ParamNode>(node)) {
      TreeNodes::ParamNode param = std::get<TreeNodes::ParamNode>(node);
      std::cout << "Kind: " << param.kind << std::endl;
      std::cout << "Param_Type: " << param.type.typeValue << std::endl;
      std::cout << "Ident: ";
      logNode(*param.ident);
    } else if (std::holds_alternative<TreeNodes::FunctionDeclarationNode>(node)) {
      TreeNodes::FunctionDeclarationNode fn = std::get<TreeNodes::FunctionDeclarationNode>(node);
      std::cout << "Kind: " << fn.kind << std::endl;
      std::cout << "Ident: (";
      logNode(*fn.ident);
      std::cout << ")\n";

      std::cout << "Return_Type: " <<  fn.functionReturnType.typeValue << std::endl;
      std::cout << "Params: " << YELLOW << " (";

      for (auto const param : fn.params) {
        std::cout << "PARAM ===>\n";
        logNode(param);
      };
      std::cout << ")" << DEFAULT << std::endl;

      std::cout << "BLOCK: " << GREEN  << " (";
      logNode(fn.block);
      std::cout << GREEN << ") " << DEFAULT << std::endl; 
    } else if (std::holds_alternative<TreeNodes::BlockStatementNode>(node)) {
      TreeNodes::BlockStatementNode block = std::get<TreeNodes::BlockStatementNode>(node);

      std::cout << "Kind: " << block.kind << std::endl;

      std::cout << "Statements: \n";

      for (const auto &stmnt : block.stmnts) {
        std::cout << "\tSTMNT: " << YELLOW << "( ";
        logNode(stmnt);
        std::cout << "\t)" << DEFAULT << std::endl;
      };
    };
};

void logTree(TreeNodes::Program tree) {

  std::cout << "AST SIZE: " << tree.body.size() << std::endl;

  if (tree.body.size() > 0) {
  
    for (auto const &node : tree.body) {
      logNode(node);
    };

  } else {
    std::cout << "Tree Is Empty!\n";
  };
};
