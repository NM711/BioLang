#include "lexer.hpp"
#include <iostream>
#include <list>

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
  this->info = new LineInfo({ 0, 0 });
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
  this->lookup["("] = MapValue{ Left_Parenthesis, SpecialSymbol };
  this->lookup[")"] = MapValue{ Right_Parenthesis, SpecialSymbol };
  this->lookup["{"] = MapValue{ Left_Curly, SpecialSymbol };
  this->lookup["}"] = MapValue{ Right_Curly, SpecialSymbol };
  this->lookup[":"] = MapValue{ Colon, SpecialSymbol };
  this->lookup[";"] = MapValue{ Semicolon, SpecialSymbol };
  this->lookup[","] = MapValue{ Seperator, SpecialSymbol };
  this->lookup["\n"] = MapValue{ LineBreak, Newline };


  string alphabet = "abcdefghijklmnopqrstuvABCDEFGHIJKLMNOPQRSTUV";
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
  ++this->info->row;
  if (this->look() == '\n') {
    this->info->row = 1;
    ++this->info->col;
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
      this->info
    }
  );
};

void BioLexer::changeState(char inputChar, LexerState &state) {
  string stringifiedChar = string(1, inputChar);

  if (this->lookup.find(stringifiedChar) != nullptr) {
    MapValue matched = this->lookup[stringifiedChar];

    switch (matched.state) {

      case Newline:
    
        // break out of comment if exists;
        if (state == Comment) {
          state = CommentEnd;
        } else {
          state = Newline;
        };

      break;

      case SpecialSymbol:
        if (state != Comment && state != Quote) {
          state = SpecialSymbol;
        };

      break;

      case Alpha:

        if (state != Comment && state != Quote && state != SpecialSymbol) {
            state = Alpha;
        };

        // checks if the state before this was not a number, we are expecting alphanum chars, not numalpha
      break;

      case Num:
        if (state != Comment && state != Quote && state != SpecialSymbol) {
            state = Num;
        };
      break;

      case Quote:
        
        if (state != Comment && state != Quote) {
          state = Quote;
        } else if (state == Quote) {
          state = QuoteEnd;
        };

      break;

      case Whitespace:
        state = Start; 
      break;

      case Comment:
        state = Comment;
      break;

      default:
        state = Ignore;
    };

  };
};

list<Token> BioLexer::tokenize() {
  string word;
  TokenIdentifiers id;

  string apply;

  LexerState state = Start;
  string stringChar = string(1, this->look());

  while (this->index < this->input.length()) {
    this->updateLineInfo();

    this->changeState(this->look(), state);

    switch (state) {
        case Alpha:
        case Num:
          word += this->look();
        break;

        case Quote:
          if (this->look() != '\"') {
            word += this->look();
          };
        break;

        case SpecialSymbol:

          cout << "S: " << word << endl;

          word += this->look();
        break;

        case Start:

          cout << "WORD LENGTH " << word.length() << " WORD VALUE " << word << endl;

          if (word.length() > 0) {
              if (this->lookup.find(word) != nullptr) {
                MapValue matched = this->lookup[word];
                /* cout << "START STATE: " << matched.id << endl; */
                this->pushToken(word, matched.id);
              } else {
                this->pushToken(word, Ident);
              };
          };

        word.clear();
      break;

        case QuoteEnd:
          this->pushToken(word, StringLiteral);
          word.clear();
        break;

      default: {

        if (state != CommentEnd && state != Comment && state != Newline && state != Whitespace && state != Ignore) {
          cout << "SYNTAX ERROR >>>>>" << " (col: " << this->info->col << ", row: " << this->info->row << ")\n";
        };
      };
    };

    this->eat();
  };

  for (auto const &it : this->tokens) {
    printToken(it);
  };


  return this->tokens;
};
