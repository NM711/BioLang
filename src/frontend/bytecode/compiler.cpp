#include "./bytecode.hpp"
#include <variant>

BytecodeCompiler::BytecodeCompiler() {
  this->chunk = {};
};

void BytecodeCompiler::setSource(TreeNodes::Program source) {
  this->source = source;
};

list<Instruction> BytecodeCompiler::getInstructions() {
  return this->chunk;
};

Instruction BytecodeCompiler::generateInstruction(unsigned short code, Data *data) {
  return Instruction{
     code,
     data
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

  Data *data = new Data();

  if (literal.type == "boolean" || literal.type == "int") {
    data->value = stoi(literal.value); 
  } else if (literal.type == "float") {
    data->value = stof(literal.value);
  } else if (literal.type == "string") {
    data->value = literal.value;
  } else {
    data = nullptr;
  };

  this->chunk.push_back(this->generateInstruction(OP_VALUE, data));
};

// TODO: Start clearing the nodes during compilation into bytecode

void BytecodeCompiler::compileNode(TreeNodes::Node &node) {
  if (holds_alternative<TreeNodes::ExpressionNode>(node)) {
    const auto expr = get<TreeNodes::ExpressionNode>(node);
    // 10 + 23 + 2 + 1
    // change OP_VALUE to OP_PUSH later
    // OP_VALUE, OP_VALUE, OP_ADD, OP_VALUE, OP_VALUE, OP_ADD

    this->compileNode(*expr.lhs);
    this->compileNode(*expr.rhs);
    this->chunk.push_back(this->generateInstruction(this->compileOperator(expr.op), nullptr));

    delete expr.lhs;
    delete expr.rhs;

  } else if (holds_alternative<TreeNodes::LiteralNode>(node)) {

    auto literal = get<TreeNodes::LiteralNode>(node);
    this->compileLiteral(literal);

  } else if (holds_alternative<TreeNodes::IdentifierNode>(node)) {

    auto ident = get<TreeNodes::IdentifierNode>(node);
    Value name = ident.name;

    Data *data = new Data();

    data->value = name;

    this->chunk.push_back(this->generateInstruction(OP_ID, data));

  } else if (holds_alternative<TreeNodes::FunctionNode>(node)) {
    auto fn = get<TreeNodes::FunctionNode>(node);
  };
};

void BytecodeCompiler::compile() {
  for (auto &node : this->source.body) {
    this->compileNode(node);
  };
};
