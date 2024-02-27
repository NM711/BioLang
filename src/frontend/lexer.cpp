#include "./lexer.hpp"
#include "../utils/debug.hpp"
#include <cstddef>
#include <cstdio>
#include <iostream>
#include <list>
#include <regex>
#include <variant>


bool isDigit(char c) {
  return regex_match(string(1, c), regex("[0-9]"));
};

bool isAlphabet(char c) {
  return regex_match(string(1, c), regex("[a-zA-Z]"));
};

bool isSpecial(char c) {
  return regex_match(string(1, c), regex("[\\+\\-*\\/\\|#%&;.:=!(){}><\"]"));
};

BioLexer::BioLexer() {
  this->lookup = {};
  this->tokens = {};
  this->errorState = false;
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

char BioLexer::peek(int step) {
  return this->input[this->index + step];
};

char BioLexer::eat() {
  this->updateLineInfo();
  return this->input[++this->index];
};

void BioLexer::checkExistsPush(string &key) {
  if (this->exists(key)) {
    this->matchAndPush(key);
    key.clear();
  } else {
    string at = string("Unexpected in ") + "\"" + key + "\"" + ", when attempting to push token!"; 
    throw SyntaxError(at, this->info);
  };
};

void BioLexer::initLookupKeywords() {
  this->lookup["import"] = Import;
  this->lookup["export"] = Export;
  this->lookup["const"] = Const;
  this->lookup["let"] = Var;
  this->lookup["if"] = If;
  this->lookup["else"] = Else;
  this->lookup["fn"] = Function;
  this->lookup["for"] = For;
  this->lookup["while"] = While;
  this->lookup["break"] = Break;
  this->lookup["continue"] = Continue;
  this->lookup["int"] = Integer;
  this->lookup["float"] = Float;
  this->lookup["string"] = String;
  this->lookup["boolean"] = Boolean;
  this->lookup["void"] = Void;
  this->lookup["true"] = BooleanLiteral;
  this->lookup["false"] = BooleanLiteral;
  this->lookup["object"] = Object;
  this->lookup["private"] = FieldState;
  this->lookup["public"] = FieldState;
};

void BioLexer::updateLineInfo() {
  ++this->info.row;
  if (this->look() == '\n') {
    this->info.row = 1;
    ++this->info.col;
  };
};

bool BioLexer::exists(const variant<string, char> &key) {
  string k;

  if (holds_alternative<string>(key)) {
    k = get<string>(key);
  } else if (holds_alternative<char>(key)) {
    k = string(1, get<char>(key));
  };

  if (this->lookup.find(k) == this->lookup.end()) {
    return false;
  } else {
    return true;
  };
};

void BioLexer::matchAndPush(string key) {
  TokenIdentifiers matched = this->lookup[key];
  this->pushToken(key, matched);
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

void BioLexer::handleComment() {
  while (this->look() != '\n' && this->look() != '\0') {
    this->updateLineInfo();
    this->eat();
  };
};

void BioLexer::handleString() {
  string word = "";
  // eat first "
  this->eat();
  while (this->look() != '\"') {
    this->updateLineInfo();
    word += this->look();
    this->eat();
  };
  // eat double quote
  this->eat();

  this->pushToken(word, StringLiteral);
};

void BioLexer::handleLiteralsOrKW() {
  string word = "";
  // you can have alpha num chars in ident.

  while (isDigit(this->look()) || isAlphabet(this->look())) {
    this->updateLineInfo();
    word += this->look();
    this->eat();
  };

  // check if the word is a map keyword

  if (this->exists(word)) {
    this->matchAndPush(word);
  } else if (regex_match(word, regex("^[a-zA-Z]+[a-zA-Z0-9]*$"))) {
    this->pushToken(word, Ident);
  } else if (regex_match(word, regex("[0-9]+"))) {
    this->pushToken(word, IntegerLiteral); 
  } else if (regex_match(word, regex("[0-9]+.[0-9]+"))) {
    this->pushToken(word, FloatLiteral);
  } else {
    string at = string("Unexpected in ") + "\"" + word + "\"" + ", when attempting to push literal or ident token!"; 
    throw SyntaxError(at, this->info);
  };
};

void BioLexer::handleSpecial() {
  string op;

  switch (this->look()) {
 
    case '#':
      this->handleComment();
    break;

    case '\"':
      this->handleString();
    break;

    case ':':
      op += this->look();
      this->eat();
      this->pushToken(op, Colon);
    break;

    case ';':
      op += this->look();
      this->eat();
      this->pushToken(op, Semicolon);
    break;

    case '(':
      op += this->look();
      this->eat();

      this->pushToken(op, LeftParenthesis);
    break;

    case ')':
      op += this->look();
      this->eat();

      this->pushToken(op, RightParenthesis);
    break;

    case '{':
      op += this->look();
      this->eat();

      this->pushToken(op, LeftCurly);
    break;

    case '}':
      op += this->look();
      this->eat();

      this->pushToken(op, RightCurly);
    break;

    case '.':
      op += this->look();
      this->eat();

      this->pushToken(op, Period);
    break;

    case ',':
      op += this->look();
      this->eat();

      this->pushToken(op, Seperator);
    break;

    case '=':
      op += this->look();

      if (this->peek() != '=') {
        this->pushToken(op, Equal);
        this->eat();
        break;
      };

      // if above doesnt run we know we are aiming for the equality op

     if (this->peek(2) == '=') {
       op += this->peek();
       op += this->peek(2);

       this->pushToken(op, Equality);

       this->eat();
       this->eat();
       this->eat();
     } else {

       logWarning("Did you mean \"===\"?\n");
       this->errorState = true;
     };
    break;

    case '!':
      op += this->look();  

      if (this->peek() != '=') {
        this->pushToken(op, Exclamation);
        this->eat();
        break;
      };

      if (this->peek(2) == '=') {
       op += this->peek();
       op += this->peek(2);

       this->pushToken(op, NotEquality);
       this->eat();
       this->eat();
       this->eat();
      } else {
       logWarning("Did you mean \"!==\"?\n");
       this->errorState = true;
      };

    break;

    case '+':
      op += this->look();

      if (this->peek() == '+') {
        op += this->peek();
        this->pushToken(op, Increment);
        this->eat();
        this->eat();
      } else {
        this->pushToken(op, Addition);
        this->eat();
      };

    break;

    case '-':
      op += this->look();

      if (this->peek() == '-') {
        op += this->peek();
        this->pushToken(op, Decrement);
        this->eat();
        this->eat();
      } else {
        this->pushToken(op, Subtraction);
        this->eat();
      };

    break;

    case '*':
      op += this->look();
      this->pushToken(op, Multiplication);
      this->eat();
    break;

    case '/':
      op += this->look();
      this->pushToken(op, Division);
      this->eat();
    break;

    case '%':
      op += this->look();
      this->pushToken(op, Modulus);
      this->eat();
    break;

    case '>':
      op += this->look();

      if (this->peek() == '=') {
        op += this->peek();
        this->pushToken(op, GreaterThanOrEqual);
        this->eat();
        this->eat();
      } else {
       this->pushToken(op, GreaterThan);
       this->eat();
      };

    break;

    case '<':
      op += this->look();

      if (this->peek() == '=') {
        op += this->peek();
        this->pushToken(op, LesserThanOrEqual);
        this->eat();
        this->eat();
      } else {
       this->pushToken(op, LesserThan);
       this->eat();
      };
    break;

    case '&':
      op += this->look();

      if (this->peek() == '&') {
        op += this->peek();
        this->pushToken(op, And);
        this->eat();
        this->eat();
      } else {
        logWarning("\"&\" is an invalid operator, did you mean \"&&\"?");
        this->errorState = true;
      };
    break;

    case '|':
      op += this->look();

      if (this->peek() == '|') {
        op += this->peek();
        this->pushToken(op, Or);
        this->eat();
        this->eat();
      } else {
        logWarning("\"|\" is an invalid operator, did you mean \"||\"?");
        this->errorState = true;
      };
    break;

    default:
      this->errorState = true;
    break;
  };

  op.clear();
};

list<Token> BioLexer::tokenize() {

  if (this->input.empty()) {
    cout << "Input cannot be empty!\n";
    exit(1);
  };

  string word;

  while (this->look() != '\0') {
    try {
      this->updateLineInfo();

      if (isSpecial(this->look())) {
        this->handleSpecial();
      } else if (isAlphabet(this->look()) || isDigit(this->look())) {
        this->handleLiteralsOrKW();
      } else if (this->look() == '\n' || this->look() == ' ') {
        this->eat();
      } else {
        this->errorState = true;
      };

      if (this->errorState) {
          string at = string("Invalid at ") + "\"" + this->look() + "\"";
          throw SyntaxError(at, this->info);
      };

    } catch (SyntaxError error) {
       error.errorMssg();
       exit(1);
    };
  };

  this->pushToken("EOF", EndOfFile);
  list<Token> tempTokens = this->tokens;
  this->cleanup();

  return tempTokens;
};
