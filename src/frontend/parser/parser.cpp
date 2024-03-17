#include "./parser.hpp"
#include "../../errors.hpp"
#include <cstddef>
#include <variant>

void BioParser::setTokens(std::list<Token> tokens) {
  this->tokens = tokens;
};

void BioParser::eat() {
  if (this->tokens.size() > 1) {
    this->tokens.pop_front();
  };
};

 Token BioParser::look() {
   return this->tokens.front();
};

std::string BioParser::expectedMssg(std::string exp) {
  return std::string("Expected ") + "\"" + exp + "\"" + std::string(", instead received ") + "\"" + this->look().lexeme + "\"!";
};

void BioParser::expected(TokenIdentifiers token, std::string symb, bool eat) {
  if (this->look().id != token) {
    throw FrontendException::SyntaxError(this->expectedMssg(symb), this->look().info);
  };

  if (eat) {
    this->eat();
  };
};

void BioParser::customExpected(bool condition, std::string symb) {
  if (condition) {
    throw FrontendException::SyntaxError(this->expectedMssg(symb), this->look().info);
  };
};

bool BioParser::prefixSymbolExists(TokenIdentifiers id) {
  const std::set<TokenIdentifiers> symbolsSet = {Exclamation, Increment, Decrement};
  return symbolsSet.find(id) != symbolsSet.end();
};

void BioParser::createExprNode(TreeNodes::Node &lhs) {
  std::string op = this->look().lexeme;
  this->eat();

  TreeNodes::Node rhs = this->parseExpression();

  // so from what im understanding, assigning expr.lhs to &lhs and expr.rhs to &rhs only works within the scope
  // of the function. This is due to c++ automatically deallocating memory once we are out of this function,
  // so the addresses we assigned are then automatically given undefined behaviour.
  // So it seems like we may need a level of dynamic memory.

  TreeNodes::ExpressionNode expr;
  expr.lhs = new TreeNodes::Node(lhs);
  expr.op = op;
  expr.rhs = new TreeNodes::Node(rhs);

  lhs = expr;
};

std::list<TreeNodes::ObjectMemberNode> BioParser::parseObjectFields() {
  std::list<TreeNodes::ObjectMemberNode> fields = {};
  while (true) {

    bool isPrivate = false;

    std::string propFieldState = this->look().lexeme;

    this->expected(FieldState, "field state");

    if (propFieldState == "private") {
      isPrivate = true;
    };

    if (this->look().id == Function) {
      TreeNodes::ObjectMethodNode method;
      method.position = this->look().info;
      method.isPrivate = isPrivate;
      method.method = new TreeNodes::Node(this->parseFunction());
      fields.push_back(method);
    } else if (this->look().id == Const || this->look().id == Var) {
      TreeNodes::ObjectPropertyNode property;
      TreeNodes::Node data = this->parseVariable();

      property.isPrivate = isPrivate;
      property.data = new TreeNodes::Node(data);

      fields.push_back(property);
    } else {
      std::cout << "Unexpected field type!\n";
      exit(1);
    };

    if (this->look().id == RightCurly) {
        break;
    };
  };

  return fields;
};

TreeNodes::Node BioParser::parseObject() {
  this->eat();

  TreeNodes::Node ident = this->parsePrimary();

  this->expected(LeftCurly, "{");
  std::list<TreeNodes::ObjectMemberNode> fields = this->parseObjectFields();
  this->expected(RightCurly, "}");
  this->expected(Semicolon, ";");


  TreeNodes::ObjectNode object;

  object.ident = new TreeNodes::Node(ident);
  object.fields = fields;
  return object;
};

TreeNodes::Node BioParser::parseForStmnt() {
  this->eat();

  this->expected(LeftParenthesis, "(");

  // note that it doesnt necessarily need to be an initalizer, you can drop in a previously declared index and what not;

  TreeNodes::Node *init = nullptr;

  if (this->look().id != Semicolon) {
    init = new TreeNodes::Node(this->parse());
  };

  TreeNodes::Node expr = this->parseExpression();

  this->expected(Semicolon, ";");

  TreeNodes::Node upd = this->parseExpression();
  this->expected(RightParenthesis, ")");

  this->expected(LeftCurly, "{");

  TreeNodes::BlockStatementNode block = this->parseBlockStmnt();

  this->expected(RightCurly, "}");
  this->expected(Semicolon, ";");

  TreeNodes::ForLoopNode forStmntNode;
  
  forStmntNode.initializer = init;
  forStmntNode.condition = new TreeNodes::Node(expr);
  forStmntNode.updater = new TreeNodes::Node(upd);
  forStmntNode.block = block;

  return forStmntNode;
};

TreeNodes::Node BioParser::parseWhileStmnt() {
  this->eat();

  this->expected(LeftParenthesis, "(");
  TreeNodes::Node expr = this->parseExpression();
  this->expected(RightParenthesis, ")");

  this->expected(LeftCurly, "{");

  TreeNodes::BlockStatementNode block = this->parseBlockStmnt();

  this->expected(RightCurly, "}");

  this->expected(Semicolon, ";");

  TreeNodes::WhileLoopNode whileStmnNode;

  whileStmnNode.condition = new TreeNodes::Node(expr);
  whileStmnNode.block = block;

  return whileStmnNode;
};

TreeNodes::Node BioParser::parseIfStmnt() {

  this->eat();

  this->expected(LeftParenthesis, "(");

  // parse expression seems to be eating more than what it is supposed to, resolve issue tomorrow;

  TreeNodes::Node expr = this->parseExpression();

  this->expected(RightParenthesis, ")");

  this->expected(LeftCurly, "{");

  TreeNodes::BlockStatementNode block = this->parseBlockStmnt();

  this->expected(RightCurly, "}");

  this->expected(Semicolon, ";");

  TreeNodes::IfConditionNode ifStmntNode;
  ifStmntNode.condition = new TreeNodes::Node(expr);
  ifStmntNode.block = block;

  return ifStmntNode;
};

TreeNodes::BlockStatementNode BioParser::parseBlockStmnt() {
  TreeNodes::BlockStatementNode block;
  
  while (this->look().id != RightCurly) {
    block.stmnts.push_back(this->parse());
  };

  return block;
};

std::list<TreeNodes::ParamNode> BioParser::parseFunctionParams() {
  std::list<TreeNodes::ParamNode> params;
 
  if (this->look().id != RightParenthesis) {
    while (true) {
      TreeNodes::ParamNode param;
      param.position = this->look().info;
      this->expected(Ident, "param identifier", false);
      TreeNodes::Node ident = this->parsePrimary();
      this->expected(Colon, ":");

      this->customExpected(this->look().id == Void, "Params cannot have a type of void!");
      TreeNodes::DataTypeNode paramType = this->parseType();
      
      param.ident = new TreeNodes::Node(ident);
      param.type = paramType;

      params.push_back(param);

      if (this->look().id == RightParenthesis) {
          break;
      };

      this->expected(Seperator, ",");
    };
  };

  return params;
};

TreeNodes::Node BioParser::parseFunction() {
  TreeNodes::FunctionDeclarationNode fnNode;
  fnNode.position = this->look().info;
  this->eat();

  this->expected(Ident, "identifier", false);

  TreeNodes::Node ident = this->parsePrimary();

  this->expected(LeftParenthesis, "(");

  std::list<TreeNodes::ParamNode> params = this->parseFunctionParams();

  this->expected(RightParenthesis, ")");

  this->expected(Colon, ":");

  TreeNodes::DataTypeNode returnType = this->parseType(); 

  this->expected(LeftCurly, "{");

  TreeNodes::BlockStatementNode block = this->parseBlockStmnt();

  this->expected(RightCurly, "}");

  this->expected(Semicolon, ";");

  fnNode.block = block;
  fnNode.ident = new TreeNodes::Node(ident);
  fnNode.functionReturnType = returnType;
  fnNode.params = params;

  return fnNode;
};

TreeNodes::Node BioParser::parseVariable() {
  
  TreeNodes::VariableDeclarationNode variableNode;
  variableNode.position = this->look().info;
  bool isConstant = false;
  
  if (this->look().id == Const) {
    isConstant = true;
  };

  this->eat();
  
  this->expected(Ident, "identifier", false);
  
  TreeNodes::Node ident = this->parsePrimary();

  this->expected(Colon, ":");
  
  this->customExpected(this->look().id == Void, "Variables cannot be of type void, this type is only reserved for return types.");
  this->customExpected(this->look().id == Null && isConstant, "Constant must be non-null");

  TreeNodes::DataTypeNode varType = this->parseType();  


  // check dec end before assignment, and make sure its not const if so.

  this->customExpected(isConstant && this->look().id == Semicolon, "Initializer");

  if (this->look().id == Equal) {
      this->eat();
      TreeNodes::Node value = this->parse();

      if (!std::holds_alternative<TreeNodes::LiteralNode>(value) && !std::holds_alternative<TreeNodes::IdentifierNode>(value)) {
        throw FrontendException::SyntaxError(std::string("Invalid value being used as initializer in variable ") + "\"" + this->look().lexeme + "\"", this->look().info);
      };

      variableNode.value = new TreeNodes::Node(value);
  };

  this->expected(Semicolon, ";");
  
  variableNode.ident = new TreeNodes::Node(ident);
  variableNode.isConstant = isConstant;
  variableNode.type = varType;

  return variableNode;
};

TreeNodes::DataTypeNode BioParser::parseType() {

  TreeNodes::DataTypeNode dataType;
  dataType.position = this->look().info;
  
  switch (this->look().id) {
    case Boolean:
      this->eat();
      dataType.typeValue = T_BOOLEAN;
    return dataType;

    case String:
      this->eat();
      dataType.typeValue = T_STRING;
    return dataType;

    case Integer:
     this->eat();
     dataType.typeValue = T_INTEGER;
    return dataType;

    case Float:
     this->eat();
     dataType.typeValue = T_FLOAT;
    return dataType;

    case Null:
      this->eat();
      dataType.typeValue = T_NULL;
    return dataType;

    case Void:
      this->eat();
      dataType.typeValue = T_VOID;
    return dataType;

    default:
      this->expected(Ident, "Type Identifier", false);
      Token typeIdent = this->look();
      this->eat();

      dataType.typeValue = T_OBJECT_REF;
      dataType.ident = new std::string(typeIdent.lexeme);
      return dataType;
  };
};

// make it so this eats automatically, the issue is that if i dont then i would need to perform a check
// when I call this specific parsing function via this->parse and what not, every other function eats but this does not.

TreeNodes::Node BioParser::parsePrimary() {

  TreeNodes::LiteralNode literal;
  literal.position = this->look().info;
  TreeNodes::DataTypeNode literalTypeData;

  switch (this->look().id) {
      case NullLiteral: {
        literalTypeData.typeValue = T_NULL;
        literal.value = "null";
        literal.type = literalTypeData;
        return literal;    
      };
    
      case BooleanLiteral: {
        literalTypeData.typeValue = T_BOOLEAN;
        literal.type = literalTypeData;
        
        if (this->look().lexeme == "0") {
          literal.value = "false";
        } else {
          literal.value = "true";
        };

        this->eat();
        return literal;
      };

      case StringLiteral: {
        literalTypeData.typeValue = T_STRING;
        literal.value = this->look().lexeme;
        literal.type = literalTypeData;
        this->eat();
        return literal;
      };

      case IntegerLiteral: {
        literalTypeData.typeValue = T_INTEGER;
        literal.value = this->look().lexeme;
        literal.type = literalTypeData;
        this->eat();
        return literal;
      };

      case FloatLiteral: {
        literalTypeData.typeValue = T_FLOAT;
        literal.value = this->look().lexeme;
        literal.type = literalTypeData;
        this->eat();
        return literal;
      };

      case EndOfFile: {
        literalTypeData.typeValue = T_NULL;
        literal.value = "EOF";
        literal.type = literalTypeData;
        return literal;
      };

      case Ident: {
        const TreeNodes::IdentifierNode ident = {.name = this->look().lexeme};
        this->eat();
        return ident;
      };

      default:
        std::cout << "Unexpected token id found!" << " (\"" << this->look().lexeme << "\") at (col: " << this->look().info.col << ", row: " << this->look().info.row << ")\n";
        exit(1);
  };
};

TreeNodes::Node BioParser::parseMember() {
  TreeNodes::Node lhs;
  
  if (this->prefixSymbolExists(this->look().id) == false) {
    lhs = this->parsePrimary();
    while (this->look().id == Arrow) {
      std::string op = this->look().lexeme;
      this->eat();

      TreeNodes::Node rhs = this->parseExpression();

      if (!std::holds_alternative<TreeNodes::IdentifierNode>(lhs)) {
        throw FrontendException::SyntaxError("Expected a valid identifier before the postfix operator!", this->look().info);
      };

      TreeNodes::ExpressionNode memberExpr;

      memberExpr.kind = "MemberExpressionNode";
      memberExpr.lhs = new TreeNodes::Node(lhs);
      memberExpr.op = op;
      memberExpr.rhs = new TreeNodes::Node(rhs);

      lhs = memberExpr;
    };
  };

  return lhs;
};

TreeNodes::Node BioParser::parseCall() {
  TreeNodes::Node lhs = this->parseMember();

  if (std::holds_alternative<TreeNodes::IdentifierNode>(lhs) && this->look().id == LeftParenthesis) {
    this->eat();

    std::list<TreeNodes::Node> args;
  
    while (this->look().id != RightParenthesis) {
      // parse args
      TreeNodes::Node arg = this->parsePrimary();

      if (std::holds_alternative<TreeNodes::LiteralNode>(arg)) {
        args.push_back(arg);
      } else if (std::holds_alternative<TreeNodes::IdentifierNode>(arg)) {
        args.push_back(arg);
      } else {
        throw FrontendException::SyntaxError("Expected a valid a literal or identifier in the function call argument!", this->look().info);
      };

      if (this->look().id != RightParenthesis) {
        this->expected(Seperator, ",");
      };
    };
    // eat right parenthesis
    this->eat();
    this->expected(Semicolon, ";");
  };
  return lhs;
};

TreeNodes::Node BioParser::parsePostfix() {
  TreeNodes::Node lhs = this->parseCall();

  if (std::holds_alternative<TreeNodes::IdentifierNode>(lhs) && this->prefixSymbolExists(this->look().id) && this->look().id != Exclamation) {
    std::string op = this->look().lexeme;
    this->eat();
    if (!std::holds_alternative<TreeNodes::IdentifierNode>(lhs)) {
      throw FrontendException::SyntaxError("Expected a valid identifier before the postfix operator!", this->look().info);
    };

    TreeNodes::PostfixExpressionNode postfix = TreeNodes::PostfixExpressionNode();

    postfix.op = op;
    postfix.argument = new TreeNodes::Node(lhs);

    lhs = postfix;
  };

  return lhs;
};

TreeNodes::Node BioParser::parsePrefix() {
  TreeNodes::Node lhs = this->parsePostfix();

  if (this->prefixSymbolExists(this->look().id)) {
      std::string op = this->look().lexeme;
      this->eat();
      TreeNodes::Node ident = this->parsePrimary();

      if (!std::holds_alternative<TreeNodes::IdentifierNode>(ident)) {
        throw FrontendException::SyntaxError("Expected a valid identifier after the prefix operator!", this->look().info);
      };

      TreeNodes::PrefixExpressionNode prefix;

      prefix.op = op;
      prefix.argument = new TreeNodes::Node(ident);

      lhs = prefix;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseMultiplicatives() {

  TreeNodes::Node lhs = this->parsePrefix();

  while (this->look().id == Multiplication || this->look().id == Division || this->look().id == Modulus) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseAdditives() {

  TreeNodes::Node lhs = this->parseMultiplicatives();
 
  while (this->look().id == Addition || this->look().id == Subtraction) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseRelationalOps() {
  TreeNodes::Node lhs = this->parseAdditives();
  
  while (this->look().id == GreaterThan || this->look().id == GreaterThanOrEqual || this->look().id == LesserThan || this->look().id == LesserThanOrEqual) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseEqualityOps() {
  TreeNodes::Node lhs = this->parseRelationalOps();

  while (this->look().id == Equality || this->look().id == NotEquality) {
    this->createExprNode(lhs); 
  };

  return lhs;
};

TreeNodes::Node BioParser::parseLogicalOps() {
  TreeNodes::Node lhs = this->parseEqualityOps();
 
  while (this->look().id == Or || this->look().id == And) {
    this->createExprNode(lhs);
  };

  return lhs;
};

TreeNodes::Node BioParser::parseAssignment() {
  TreeNodes::Node lhs = this->parseLogicalOps();

  if (std::holds_alternative<TreeNodes::IdentifierNode>(lhs) && this->look().id == Equal) {

    std::string op = this->look().lexeme;
    this->eat();
    TreeNodes::Node rhs = this->parseExpression();
    TreeNodes::ExpressionNode assignmentExpr;

    const TreeNodes::IdentifierNode arg = std::get<TreeNodes::IdentifierNode>(lhs);

    assignmentExpr.kind = "AssignmentExpressionNode";
    assignmentExpr.lhs = new TreeNodes::Node(lhs);
    assignmentExpr.op = op;
    assignmentExpr.rhs = new TreeNodes::Node(rhs);

    lhs = assignmentExpr;
  };

  return lhs;
};

TreeNodes::Node BioParser::parseExpression() {
  return this->parseAssignment();
};

TreeNodes::Node BioParser::parse() {
  try {

    switch (this->look().id) {
      case Const:
      case Var:
        return this->parseVariable();

      case Function:
        return this->parseFunction();

      case If:
        return this->parseIfStmnt();

      case For:
        return this->parseForStmnt();

      case While:
        return this->parseWhileStmnt();

      case Object:
        return this->parseObject();

      default:
        return this->parseExpression();
    }

  } catch (FrontendException::SyntaxError error) {
    std::cout << error.what();
    exit(1);
  };
};

TreeNodes::Program BioParser::generateAST() {
  TreeNodes::Program program;

  program.body = {};

  while (this->tokens.size() > 1) {
    program.body.push_back(this->parse());
  };

  return program;
};
