#ifndef LEXER_HPP
#define LEXER_HPP

#include <exception>
#include <iostream>
#include <list>
#include <map>
#include <unordered_map>

using namespace std;

struct LineInfo {
  int row;
  int col;
};

enum LexerState {
  Start,
  Whitespace,
  Newline,
  SpecialSymbol,
  Alpha,
  Num,
  Quote,
  Comment,
  Reject
};

enum TokenIdentifiers {
  WS,
  LineBreak,
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
  Ident,
  Alphabet,
  Number,
  Integer,
  Float,
  String,
  StringLiteral,
  LeftCurly,
  RightCurly,
  LeftParenthesis,
  RightParenthesis,
  DoubleQuote,
  Colon,
  Period,
  Semicolon,
  Seperator,
  Hashtag,
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
  PostOrPrefixOperator,
  ExpressionCall
};

struct Token {
  TokenIdentifiers id;
  string lexeme;
  LineInfo *info;
};

struct MapValue {
  TokenIdentifiers id;
  LexerState state;
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

  public: BioLexer(string input);

  private:
    unordered_map<string, MapValue> lookup;
    void updateLineInfo();
    void initLookupKeywords();
    void pushToken(string kw, TokenIdentifiers id);
    bool exists(string key);
    void matchAndPush(string key);
    // looks at current index
    char look();
    // advances index
    char eat();

  public: list<Token> tokenize();
};

#endif
