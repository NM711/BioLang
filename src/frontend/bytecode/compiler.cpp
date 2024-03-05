#include "./bytecode.hpp"
#include <variant>

BytecodeCompiler::BytecodeCompiler() {
  this->chunk = {};
};

void BytecodeCompiler::setSource(TreeNodes::Program source) {
  this->source = source;
};

vector<Instruction> BytecodeCompiler::getInstructions() {
  return this->chunk;
};

Instruction BytecodeCompiler::generateInstruction(unsigned short code, Value *value) {
  return Instruction{
     code,
     value
  };
};

unsigned short BytecodeCompiler::compileOperator(string op) {
  if (op == "=") {
    return OP_EQ;
  } else if (op == "+") {
    return OP_ADD; 
  } else if (op == "-") {
    return OP_SUB;
  } else if (op == "*") {
    return OP_MULT;
  } else if (op == "/") {
    return OP_DIV;
  } else if (op == "%") {
    return OP_MOD;
  } else if (op == "++") {
    return OP_INCREMENT;
  } else if (op == "--") {
    return OP_DECREMENT;
  } else if (op == "===") {
    return OP_EQUALITY;
  } else if (op == "!==" || op == "!") {
    return OP_NOT_EQUALITY;
  } else if (op == ">") {
    return OP_GT;
  } else if (op == ">=") {
    return OP_GT_EQ;
  } else if (op == "<") {
    return OP_LS;
  } else if (op == "<=") {
    return OP_LS_EQ;
  } else {
    cout << "Invalid Operator\n";
    exit(1);
  };
};

void BytecodeCompiler::compileLiteral(TreeNodes::LiteralNode &literal) {

  Value value;

  if (literal.type == "boolean" || literal.type == "int") {
    value = stof(literal.value);
  } else if (literal.type == "float") {
    value = stof(literal.value);
  } else if (literal.type == "string") {
    value = stof(literal.value);
  } else if (literal.type == "void") {
    value = nullptr;
  };

  this->chunk.push_back(this->generateInstruction(OP_VALUE, &value));
};

// TODO: Start clearing the nodes during compilation into bytecode

void BytecodeCompiler::compileNode(TreeNodes::Node &node) {
  if (holds_alternative<TreeNodes::ExpressionNode>(node)) {
    const auto expr = get<TreeNodes::ExpressionNode>(node);

    this->compileNode(*expr.lhs);

    this->chunk.push_back(this->generateInstruction(this->compileOperator(expr.op), nullptr));

    this->compileNode(*expr.rhs);

  } else if (holds_alternative<TreeNodes::LiteralNode>(node)) {
    auto literal = get<TreeNodes::LiteralNode>(node);
    this->compileLiteral(get<TreeNodes::LiteralNode>(node));
  };
};

void BytecodeCompiler::compile() {
  for (auto &node : this->source.body) {

    if (holds_alternative<TreeNodes::ExpressionNode>(node)) {
      const auto expr = get<TreeNodes::ExpressionNode>(node);

      this->compile();

      /* switch (expr.op) { */
      
      /* } */


    };

  };
};
