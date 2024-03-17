#include "./frontend/lexer/lexer.hpp"
#include "./frontend/parser/parser.hpp"
// #include "./frontend/bytecode/bytecode.hpp"
// #include "./runtime/vm.hpp"

class Repl {

  public:
    void execute();

  private:
    BioLexer lexer = BioLexer();
    BioParser parser = BioParser();
    // BytecodeCompiler compiler = BytecodeCompiler();
    // BioVirtualMachine vm = BioVirtualMachine();
};
