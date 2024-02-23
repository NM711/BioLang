#include "./frontend/lexer.hpp"
#include "./frontend/parser.hpp"

class Repl {

  public:
    void execute();

  private:
    BioLexer lexer = BioLexer();
    BioParser parser = BioParser();
};