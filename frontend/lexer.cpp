#include "lexer.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <regex>

void printToken(Token token) {
  cout << "\n{\n" << "  id: " << token.id << ",\n";
  cout << "  lexeme: " << token.lexeme << ",\n";
  cout << "  info: {\n" << "   col: " << token.info.col << ",\n" << "   row: " << token.info.row << "\n  }\n" << "}\n";
};

BioLexer::BioLexer() {
  this->lookup = {};
  this->tokens = {};
  this->initLookupKeywords();
  this->index = 0;
  this->info = LineInfo({ 1, 1 });
};

void BioLexer::setInput(string input) {
  this->input = input;
};

void BioLexer::cleanup() {
  this->tokens.clear();
  this->index = 0;
  this->info = LineInfo({ 1, 1 });
  this->input.clear();
};

char BioLexer::look() {
  return this->input[this->index];
};

char BioLexer::peek() {
  return this->input[this->index + 1];
};

char BioLexer::eat() {
  this->updateLineInfo();
  return this->input[++this->index];
};

void BioLexer::checkExistsPush(string &key, LexerState &state) {
  if (this->exists(key)) {
    this->matchAndPush(key);
    key.clear();
  } else {
    string at = string("Unexpected in ") + "\"" + key + "\"" + ", when attempting to push token!"; 
    throw SyntaxError(at, this->info);
  };
};

void BioLexer::initLookupKeywords() {
  this->lookup[""] = MapValue{ WS, Whitespace };
  this->lookup[" "] = MapValue{ WS, Whitespace };
  this->lookup["\""] = MapValue{ DoubleQuote, Quote };
  this->lookup["#"] = MapValue{ Hashtag, Comment };
  this->lookup["import"] = MapValue{ Import, Start };
  this->lookup["const"] = MapValue{ Const, Start };
  this->lookup["let"] = MapValue{ Var, Start };
  this->lookup["if"] = MapValue{ If, Start };
  this->lookup["else"] = MapValue{ Else, Start };
  this->lookup["fn"] = MapValue{ Function, Start };
  this->lookup["for"] = MapValue{ For, Start };
  this->lookup["while"] = MapValue{ While, Start };
  this->lookup["break"] = MapValue{ Break, Start };
  this->lookup["continue"] = MapValue{ Continue, Start };
  this->lookup["int"] = MapValue{ Integer, Start };
  this->lookup["float"] = MapValue{ Float, Start };
  this->lookup["string"] = MapValue{ String, Start };
  this->lookup["boolean"] = MapValue{ Boolean, Start };
  this->lookup["true"] = MapValue{ BooleanLiteral, Start };
  this->lookup["false"] = MapValue{ BooleanLiteral, Start };
  this->lookup["!"] = MapValue{ Exclamation, SpecialSymbol };
  this->lookup["<"] = MapValue{ LesserThan, SpecialSymbol };
  this->lookup["<="] = MapValue{ LesserThanOrEqual, SpecialSymbol };
  this->lookup[">"] = MapValue{ GreaterThan, SpecialSymbol };
  this->lookup[">="] = MapValue{ GreaterThanOrEqual, SpecialSymbol };
  this->lookup["==="] = MapValue{ Equality, SpecialSymbol };
  this->lookup["!=="] = MapValue{ NotEquality, SpecialSymbol };
  this->lookup["="] = MapValue{ Equal, SpecialSymbol };
  this->lookup["+"] = MapValue{ Addition, SpecialSymbol };
  this->lookup["++"] = MapValue{ PostOrPrefixOperator, SpecialSymbol };
  this->lookup["--"] = MapValue{ PostOrPrefixOperator, SpecialSymbol };
  this->lookup["-"] = MapValue{ Subtraction, SpecialSymbol };
  this->lookup["*"] = MapValue{ Multiplication, SpecialSymbol };
  this->lookup["/"] = MapValue{ Division, SpecialSymbol };
  this->lookup["%"] = MapValue{ Modulus, SpecialSymbol };
  this->lookup["("] = MapValue{ LeftParenthesis, SpecialSymbol };
  this->lookup[")"] = MapValue{ RightParenthesis, SpecialSymbol };
  this->lookup["{"] = MapValue{ LeftCurly, SpecialSymbol };
  this->lookup["}"] = MapValue{ RightCurly, SpecialSymbol };
  this->lookup[":"] = MapValue{ Colon, SpecialSymbol };
  this->lookup[";"] = MapValue{ Semicolon, SpecialSymbol };
  this->lookup[","] = MapValue{ Seperator, SpecialSymbol };
  this->lookup["."] = MapValue{ Period, SpecialSymbol };
  this->lookup["\n"] = MapValue{ LineBreak, Newline };


  string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string numbers = "0123456789";

  for (int i = 0; i < alphabet.length(); ++i) {
    string letter = alphabet.substr(i, 1);
    this->lookup[letter] = MapValue{ Ident, Alpha };
  };

  for (int i = 0; i < numbers.length(); ++i) {
    string number = numbers.substr(i, 1);
    this->lookup[number] = MapValue{ Number, Num };
  };

};

void BioLexer::updateLineInfo() {
  ++this->info.row;
  if (this->look() == '\n') {
    this->info.row = 1;
    ++this->info.col;
  };
};

bool BioLexer::exists(const string &key) {
  if (this->lookup.find(key) == this->lookup.end()) {
    return false;
  } else {
    return true;
  };
};

void BioLexer::matchAndPush(string key) {
  MapValue matched = this->lookup[key];
  this->pushToken(key, matched.id);
};

void BioLexer::pushToken(string kw, TokenIdentifiers id) {

  this->tokens.push_back(
    Token {
      id,
      kw,
      this->info
    }
  );
};

void BioLexer::handleSpecial(string &character, LexerState &state) {
  string special;

  switch (this->look()) {
    case '(':
    case ')':
    case '{':
    case '}':
    case ';':
    case ':':
    case '/':
    case '*':
    case '%':
        this->matchAndPush(character);
    break;

    case '!':
        if (this->peek() == '=') {
          special += this->look();
          special += this->peek();
          this->eat();
          this->eat();
          special += this->look();
          this->checkExistsPush(special, state);
          break;
        };

        this->matchAndPush(character);
    break;

    case '+':

      if (this->peek() == '+') {
        special += this->look();
        special += this->peek();
        this->eat();
        this->matchAndPush(special);
        special.clear();
        break;
      };

      this->matchAndPush(character);
    break;

    case '-':
      if (this->peek() == '-') {
        special += this->look();
        special += this->peek();
        this->eat();
        this->matchAndPush(special);
        special.clear();
        break;
      };

      this->matchAndPush(character);
    break;


    case '=':

      if (this->peek() == '=') {
        special += this->look();
        special += this->peek();
        this->eat();
        this->eat();
        special += this->look();
        this->checkExistsPush(special, state);
        special.clear();
        break;
      };

      this->matchAndPush(character);
    break;

    case '>':

      if (this->peek() == '=') {
        special += this->look();
        special += this->peek();
        this->eat();

        this->matchAndPush(special);
        special.clear();
        break;
      };

      this->matchAndPush(character);
    break;

    case '<':

      if (this->peek() == '=') {
        special += this->look();
        special += this->peek();
        this->eat();

        this->matchAndPush(special);
        special.clear();
        break;
      };

      this->matchAndPush(character);
    break;

    default:
      state = Reject;
    break;
  };
};

list<Token> BioLexer::tokenize() {


  if (this->input.empty()) {
    cout << "Input cannot be empty!\n";
    exit(1);
  };

  string word;
  LexerState state = Start;

  while (this->index < this->input.length()) {
    string stringChar = string(1, this->look());

    try {
      MapValue matchedChar;

      if (this->exists(stringChar)) {
        matchedChar = this->lookup[stringChar];
      } else if (this->exists(stringChar) == false && state != Comment && state != Quote) {
        state = Reject;
      };

      switch (state) {
        case Start:
           if (matchedChar.id == DoubleQuote) {
              this->eat();
            };

            state = matchedChar.state;
        continue;

        case Alpha:
        case Num:
          if (matchedChar.state != Alpha && matchedChar.state != Num && matchedChar.id != Period ) {
            if (this->exists(word)) {
              this->matchAndPush(word);
            } else {

              // Now we must check if either:
              // A. Word is composed entirely of Number characters.
              // B. Word is made up of alphanumeric characters.

              if (regex_match(word, regex("^[a-zA-Z]+[a-zA-Z0-9]*$"))) {
                this->pushToken(word, Ident);
              } else if (state == Num && regex_match(word, regex("[0-9]+.[0-9]+|[0-9]+"))) {
                this->pushToken(word, Number);
              } else {

                string at = string("Unexpected in ") + "\"" + word + "\"" + ", when attempting to push num or alphabet token!"; 
                throw SyntaxError(at, this->info);
              };
            };

            state = Start;

            word.clear();
            continue;
          } else {

            word += this->look();
          };

        break;

        case Comment:
          if (matchedChar.state == Newline) {
            state = Start;
          };

        break;

        case SpecialSymbol: {

            // for SpecialSymbols and operators, Instead of munching everything until the end of the input type,
            // I will handle this by hand that way I reduce the chances of ambigiouty.
            // Take the statements fn myFunction () {}; If I munch till the end of input "()" will be lexed literally as that,
            // rather than "(" and ")", but now take a case like "===" or ">=", I cant lex these piece by piece, id rather do this
            // in a single go. That way I can easily say hey this token is for Equal, whilst this is for Equality, etc.

          if (matchedChar.state != SpecialSymbol) {
            state = Start;
            break;
          };

          this->handleSpecial(stringChar, state);

          if (state != Reject) {
            state = Start;
          };

          break;
        };

        case Quote:
          if (matchedChar.state == Quote) {
            this->pushToken(word, StringLiteral);
            word.clear();
            state = Start;
          } else {
            word += this->look();
          };
        break;

        case Newline:
        case Whitespace:
            state = Start;
        break;

        default:
          string at = string("Invalid at ") + "\"" + stringChar + "\"";
          throw SyntaxError(at, this->info);
      };

      this->eat();

    } catch(SyntaxError error) {
        error.errorMssg();
        exit(1);
    };

  };

  this->pushToken("EOF", EndOfFile);
  list<Token> tempTokens = this->tokens;
  this->cleanup();

  /* for (auto const it : tempTokens) { */
    /* printToken(it); */
  /* }; */


  return tempTokens;
};
