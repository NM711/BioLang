#ifndef LEXER_HPP
#define LEXER_HPP

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
  Ignore,
  Whitespace,
  Newline,
  SpecialSymbol,
  Alpha,
  Num,
  Quote,
  QuoteEnd,
  Comment,
  CommentEnd
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
  Left_Curly,
  Right_Curly,
  Left_Parenthesis,
  Right_Parenthesis,
  DoubleQuote,
  Colon,
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

class BioLexer {
  private:
    list<Token> tokens;
    LineInfo *info;
    string input;
    private: int index;

  public: BioLexer(string input);

  private:
    unordered_map<string, MapValue> lookup;
    void updateLineInfo();
    void initLookupKeywords();
    void pushToken(string kw, TokenIdentifiers id);
    void changeState(char inputChar, LexerState &state);
    bool exists(string key);
    void matchAndPush(string key);

    // looks at current index
    char look();
    // advances index
    char eat();

  public: list<Token> tokenize();
};

#endif
