#include "lexer.hpp"
#include <cstdio>
#include <iostream>
#include <list>
#include <regex>

void printToken(Token token) {
  cout << "\n{\n" << "  id: " << token.id << ",\n";
  cout << "  lexeme: " << token.lexeme << ",\n";
  cout << "  info: {\n" << "   col: " << token.info->col << ",\n" << "   row: " << token.info->row << "\n  }\n" << "}\n";
};

BioLexer::BioLexer(string input) {
  this->lookup = {};
  this->tokens = {};
  this->initLookupKeywords();
  this->input = input;
  this->index = 0;
  this->info = LineInfo({ 1, 1 });
};

char BioLexer::look() {
  return this->input[this->index];
};

char BioLexer::eat() {
  return this->input[++this->index];
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
  this->lookup["int"] = MapValue{ Integer, Start };
  this->lookup["float"] = MapValue{ Float, Start };
  this->lookup["string"] = MapValue{ String, Start };
  this->lookup["<"] = MapValue{ LesserThan, SpecialSymbol };
  this->lookup["<="] = MapValue{ LesserThanOrEqual, SpecialSymbol };
  this->lookup[">"] = MapValue{ GreaterThan, SpecialSymbol };
  this->lookup[">="] = MapValue{ GreaterThanOrEqual, SpecialSymbol };
  this->lookup["==="] = MapValue{ Equality, SpecialSymbol };
  this->lookup["="] = MapValue{ Equal, SpecialSymbol };
  this->lookup["+"] = MapValue{ Addition, SpecialSymbol };
  this->lookup["-"] = MapValue{ Subtraction, SpecialSymbol };
  this->lookup["*"] = MapValue{ Multiplication, SpecialSymbol };
  this->lookup["/"] = MapValue{ Division, SpecialSymbol };
  this->lookup["%"] = MapValue{ Modulus, SpecialSymbol };
  this->lookup["("] = MapValue{ LeftParenthesis, SpecialSymbol };
  this->lookup[")"] = MapValue{ RightParenthesis, SpecialSymbol };
  this->lookup["()"] = MapValue{ ExpressionCall, SpecialSymbol };
  this->lookup["{"] = MapValue{ LeftCurly, SpecialSymbol };
  this->lookup["}"] = MapValue{ RightCurly, SpecialSymbol };
  this->lookup[":"] = MapValue{ Colon, SpecialSymbol };
  this->lookup[";"] = MapValue{ Semicolon, SpecialSymbol };
  this->lookup[","] = MapValue{ Seperator, SpecialSymbol };
  // we might need to make this a num or something or perform a check in num state.
  this->lookup["."] = MapValue{ Period, SpecialSymbol };
  this->lookup["\n"] = MapValue{ LineBreak, Newline };


  string alphabet = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
  string numbers = "0123456789";

  for (int i = 0; i < alphabet.length(); ++i) {
    string letter = alphabet.substr(i, 1);
    this->lookup[letter] = MapValue{ Alphabet, Alpha };
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

bool BioLexer::exists(string key) {
  return this->lookup.find(key) != nullptr;
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
      &this->info
    }
  );
};

list<Token> BioLexer::tokenize() {
  string word;
  TokenIdentifiers id;

  string apply;

  LexerState state = Start;

  while (this->index < this->input.length()) {
    this->updateLineInfo();
    string stringChar = string(1, this->look());
    string symbol;
    
    try {
      MapValue matchedChar = this->lookup[stringChar];
    
      switch (state) {
        case Start: {
            // since we are dealing with input characters, we are not composing different keywords, meaning that the states
            // can NEVER be START, so we do not need to perform checks. Single characters fall under A - Special Chars,
            // or B -> Alphanumeric characters.

            switch (matchedChar.state) {
              case Comment:
              case Alpha:
              case Num:
              case Newline:
              case Quote:
              case SpecialSymbol:
              case Whitespace:
                state = matchedChar.state;
              break;

              default:
                state = Reject;
              break;
            };

          // we want to continue on the assigned state without consuming.
          // if for some reason it gets too messy, ill consume per switch rather than consuming on end of switch,
          // becase if im running multiple continues, shit gets harder to debug.
          continue;
        };

        case Alpha:
        case Num:
            cout << "WORD " << word << " CHAR " << this->look() << " STATE MATCHED: " << matchedChar.state << endl;
            cout << " Special " << SpecialSymbol << endl;

          // this is necessary in case you have special characters or quotations right next to a literal
          // let myInt string:
          // if the check did not exist this would get read as let, myInt, string:, which would on the last keyword would make this
          // case fail the checks on the condition below this one.

          if (matchedChar.state == Quote || matchedChar.state == SpecialSymbol) {
            state = Start;
            continue;
          };

          if (matchedChar.state != Num && matchedChar.state != Alpha && matchedChar.state != Quote && matchedChar.state != SpecialSymbol) {

              if (this->exists(word)) {
                this->matchAndPush(word);
              } else {

                // Now we must check if either:
                // A. Word is composed entirely of Number characters.
                // B. Word is made up of alphanumeric characters.

                if (regex_match(word, regex("^[a-zA-Z]+[a-zA-Z0-9]*$"))) {
                  this->pushToken(word, Ident);
                } else if (state == Num && regex_match(word, regex("\\b[\\d.]+\\b"))) {
                  this->pushToken(word, Number);
                } else {
                  cout << "ERROR PUSHING IN ALPHA OR NUM STATE !\n";
                  cout << word << endl;
                  exit(1);
                };
              };

            word.clear();
            state = Start;
          } else {
            word += this->look(); 
          };
        break;

        case SpecialSymbol:
          if (matchedChar.state != SpecialSymbol) {

            if (this->exists(word)) {
              this->matchAndPush(word);
              word.clear();
              state = Start;
            } else {
              throw SyntaxError(string("Unexpected token found in ") + "\"" + word + "\"", this->info);
            };

          } else {
            word += this->look();
          };

        break;

        case Comment:
          if (matchedChar.state == Newline) {
            state = Start;
          };
        break;

        case Newline:
        case Whitespace:
          state = Start;
        break;

        case Quote:
          cout << "IS QUOTE\n";
          word += this->look();
          this->eat();

          if (matchedChar.state == Quote) {
            this->pushToken(word, StringLiteral);
            word.clear();
            state = Start;
          };

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


  for (auto const it : this->tokens) {
    printToken(it);
  };


  return this->tokens;
};
