#include "./frontend/lexer/lexer.hpp"
#include "./frontend/parser/parser.hpp"

class Repl {

  public:
    void execute();

  private:
    BioLexer lexer = BioLexer();
    BioParser parser = BioParser();
};
