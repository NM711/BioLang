#include "../frontend/lexer.hpp"
#include <list>
#include <vector>

enum ReturnState {
  NoLengthMatch,
  Failure,
  Success,
};

class LexerTest {
  public:
    ReturnState test(list<TokenIdentifiers> expected, list<Token> tokens) {
    
      if (expected.size() != tokens.size()) {
        cout << expected.size() << " : " << tokens.size() << endl;
        return NoLengthMatch;
      };

      list<TokenIdentifiers>::iterator expectedIt = expected.begin();
      list<Token>::iterator tokenIt = tokens.begin();
      int i = 0;
      while (expectedIt != expected.end() && tokenIt != tokens.end()) {


        if (*expectedIt != tokenIt->id) {

            cout << "EXPECTED " << *expectedIt <<  " TOKEN " << tokenIt->id << " INDEX = " << i << endl;
            return Failure;
        };
        ++i;
        ++expectedIt;
        ++tokenIt;
      };

      return Success;
    };

   void evaluate(vector<ReturnState> returned) {
  
    for (int i = 0; i < returned.size(); ++i) {
        ReturnState state = returned[i];

        if (state == NoLengthMatch) {
          cout << "The length of both expected list and provided token list do not match with one another!\n";
          cout << "TEST #" << i << " FAIL\n";
        } else if (state == Failure) {
            cout << "TEST #" << i << " FAIL\n";
        } else if (state == Success) {
          cout << "TEST #" << i << " SUCCESS\n";
        };
    };
   };
};

const string input = 
"# This is justa  comment hopefully this is ignored! \n"
"fn main() {"
"  if (5 >= 3) {"
"    const letString    :       string =        \"Hello Mundoooo\"; "
"  };"
"};";

const string input2 = "#######################hekaisdjasndkj#\n"
"myFunction() {"
"  for (let i: int = 0; i < 5; ++i) {"
"   #COMMENT IN MIDDLE OF CODE TO TEST\n"
"    if (i === 5) {"
"     #break;\n"
    "};"
"  };"
"if (myFunction() === 3) {"
    "const hello: string = \"hello world\n how do you do !\";"
    "break;"
"#another comment\n"
  "};"

  "while(true){"
  "  print(123);"
  "  continue;"
  "};"

"};";

const string input3 = 
"# Comment with special symbols !@#$%^&*()_+-=[]{}\\|;:'\",./?\n"
"import const letVar: string = \"Hello World!\";\n"
"if (5 >= 3) {\n"
"    for (let i:int === 0; i < 10; --i) {\n"
"        while (i > 5) {\n"
"            if (i !== 7) {\n"
"                const hello: string = \"Hi there!\";\n"
"                break;\n"
"            };\n"
"            continue;\n"
"        };\n"
"    };\n"
"};";


int main() {
  
  LexerTest tester = LexerTest();
  BioLexer lexer = BioLexer();

  lexer.setInput(input);

  ReturnState test1 = tester.test(
  {
    Function, // fn
    Ident, // main
    LeftParenthesis, // (
    RightParenthesis, // )
    LeftCurly, // {
    If, // if
    LeftParenthesis, // (
    Number, // 5
    GreaterThanOrEqual, // >=
    Number, // 3
    RightParenthesis, // )
    LeftCurly, // {
    Const, // const
    Ident, // letString
    Colon, // :
    String, // string
    Equal, // =
    StringLiteral, // "Hello Mundooo"
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    EndOfFile // EOF
  },
   
    lexer.tokenize()
  );

 lexer.setInput(input2);

  ReturnState test2 = tester.test(
  {
    Ident, // myFunction
    LeftParenthesis, // (
    RightParenthesis, // )
    LeftCurly, // {
    For, // for
    LeftParenthesis, // (
    Var, // let
    Ident, // i
    Colon, // :
    Integer, // int
    Equal, // =
    Number, // 0
    Semicolon, // ;
    Ident, // i
    LesserThan, // <
    Number, // 5
    Semicolon, // ;
    PostOrPrefixOperator, //++
    Ident, // i
    RightParenthesis, // )
    LeftCurly, // {
    If, // if
    LeftParenthesis, // (
    Ident, // i
    Equality, // ===
    Number, // 5
    RightParenthesis, // )
    LeftCurly, // {
    RightCurly, // }
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    If, // if
    LeftParenthesis, // (
    Ident, // myFunction
    LeftParenthesis, // (
    RightParenthesis, // )
    Equality, // ===
    Number, // 3
    RightParenthesis, // )
    LeftCurly, // {
    Const, // const
    Ident, // hello
    Colon, // :
    String, // string
    Equal, // =
    StringLiteral, // "hello world\n how do you do!\n"
    Semicolon, // ;
    Break, // break
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    While, // while
    LeftParenthesis, // (
    BooleanLiteral, // true
    RightParenthesis, // )
    LeftCurly, // {
    Ident, // print
    LeftParenthesis, // (
    Number, // 123
    RightParenthesis, // )
    Semicolon, // ;
    Continue, // continue
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    EndOfFile // EOF
  },

    lexer.tokenize()
  );

  lexer.setInput(input3);

  ReturnState test3 = tester.test(
  {
    Import, // import
    Const, // const
    Ident, // letVar
    Colon, // :
    String, // string
    Equal, // =
    StringLiteral, // Hello World!
    Semicolon, // ;
    If, // if
    LeftParenthesis, // (
    Number, // 5
    GreaterThanOrEqual, // >=
    Number, // 3
    RightParenthesis, // )
    LeftCurly, // {
    For, // for
    LeftParenthesis, // (
    Var, // let
    Ident, // i
    Colon, // :
    Integer, // int
    Equal, // =
    Number, // 0
    Semicolon, // ;
    Ident, // i
    LesserThan, // <
    Number, // 10
    Semicolon, // ;
    PostOrPrefixOperator, // --
    Ident, // i
    RightParenthesis, // )
    LeftCurly, // {
    While, // while
    LeftParenthesis, // (
    Ident, // i
    GreaterThan, // >
    Number, // 5
    RightParenthesis, // )
    LeftCurly, // {
    If, // if
    LeftParenthesis, // (
    Ident, // i
    NotEquality, // !==
    Number, // 7
    RightParenthesis, // )
    LeftCurly, // {
    Const, // const
    Ident, // hello
    Colon, // :
    String, // string
    Equal, // =
    StringLiteral, // "Hi there!"
    Semicolon, // ;
    Break, // break
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    Continue, // continue
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    RightCurly, // }
    Semicolon, // ;
    EndOfFile // EOF
  },
    lexer.tokenize()
  );

   tester.evaluate({test1, test2, test3});

  return 0;
};
