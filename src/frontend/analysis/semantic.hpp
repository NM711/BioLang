#ifndef SEMANTIC_ANALYSIS_HPP
#define SEMANTIC_ANALYSIS_HPP

#include "../../common.hpp"
#include "../parser/parser.hpp"
#include "../../symb.table.hpp"
#include "../parser/parser.hpp"

/*
  We only keep track of type data for this pass, we dont keep track of values or anything of the like.
*/

struct SemanticVariable {
  SymbolIdentifier id = SYMB_VARIABLE;
  bool isConst;
  DataType type;
  std::string *typeName = nullptr;
};

struct SemanticObject {
   SymbolIdentifier id = SYMB_OBJECT;
};

struct SemanticParam {
  std::string ident;
  DataType type;
  std::string *typeName = nullptr;
};

// still viewing how to structure this.

struct SemanticFunction {
  SymbolIdentifier id = SYMB_FUNCTION;  
  unsigned paramLength;
};


typedef std::variant<SemanticFunction, SemanticObject, SemanticVariable> SemanticNode;

class SemanticAnalyzer {

  public:
    void analyze();

  private:
    SemanticAnalyzer();
    TreeNodes::Program source;
    SymbolTable<SemanticNode> table;
    void setSource(TreeNodes::Program source);
    void analyzeNode(const TreeNodes::Node &node);
    void analyzeExpression(const TreeNodes::ExpressionNode &expr);
    void analyzeAssignment(const TreeNodes::ExpressionNode &expr);
};

#endif
