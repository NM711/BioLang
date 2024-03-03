#ifndef LEXER_HPP
#define LEXER_HPP

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>
#include <variant>

using namespace std;

struct LineInfo {
  int row;
  int col;
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
  IntegerLiteral,
  FloatLiteral,
  BooleanLiteral,
  StringLiteral,
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
  Continue,
  FieldState,
};

struct Token {
  TokenIdentifiers id;
  string lexeme;
  LineInfo info;
};

class SyntaxError : public exception {

  private:
    string mssg;
    LineInfo lineInfo;

  public:
      SyntaxError(const string message, const LineInfo info) : mssg(message), lineInfo(info) {};

      const char *what() {
        return "SyntaxError";
      };

      void errorMssg() {
        cout << this->what() << " >>>>>> " << this->mssg << " " << "(col: " << this->lineInfo.col << ", row: " << this->lineInfo.row << ")\n";
      };
};

class BioLexer {
  private:
    list<Token> tokens;
    LineInfo info;
    string input;
    private: int index;

  public: BioLexer();

  private:
    unordered_map<string, TokenIdentifiers> lookup;
    bool errorState;
    void updateLineInfo();
    void initLookupKeywords();
    void pushToken(string kw, TokenIdentifiers id);
    bool exists(const std::variant<string, char> &key);
    void matchAndPush(string key);
    void checkExistsPush(string &key);
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
    void setInput(string input);
    list<Token> tokenize();
};

#endif
