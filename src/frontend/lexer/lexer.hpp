#ifndef LEXER_HPP
#define LEXER_HPP

#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <variant>

struct LineInfo {
  unsigned row;
  unsigned col;
};

enum TokenIdentifiers {
  EndOfFile,
  Function,
  Var,
  Const,
  For,
  While,
  If,
  Else,
  ElseIf,
  Import,
  Export,
  Ident,
  Integer,
  Float,
  String,
  Boolean,
  Object,
  Void,
  Null,
  IntegerLiteral,
  FloatLiteral,
  BooleanLiteral,
  StringLiteral,
  NullLiteral,
  LeftCurly,
  RightCurly,
  LeftParenthesis,
  RightParenthesis,
  Colon,
  Arrow,
  Exclamation,
  Semicolon,
  Seperator,
  Equal,
  Addition,
  Subtraction,
  Modulus,
  Division,
  Multiplication,
  GreaterThan,
  LesserThan,
  GreaterThanOrEqual,
  LesserThanOrEqual,
  Equality,
  NotEquality,
  Or,
  And,
  Return,
  Increment,
  Decrement,
  Break,
  As,
  Continue,
  FieldState,
};

struct Token {
  TokenIdentifiers id;
  std::string lexeme;
  LineInfo info;
};

class BioLexer {
  private:
    std::list<Token> tokens;
    LineInfo info;
    std::string input;
    private: int index;

  public: BioLexer();

  private:
    std::unordered_map<std::string, TokenIdentifiers> lookup;
    bool errorState;
    void updateLineInfo();
    void initLookupKeywords();
    void pushToken(std::string kw, TokenIdentifiers id);
    bool exists(const std::variant<std::string, char> &key);
    void matchAndPush(std::string key);
    void checkExistsPush(std::string &key);
    void handleComment();
    void handleString();
    void handleSpecial();
    void handleLiteralsOrKW();
    void cleanup();
    // looks at current index
    char look();
    char peek(int step = 1);
    // advances index
    char eat();

  public:
    void setInput(std::string input);
    std::list<Token> tokenize();
};

#endif
