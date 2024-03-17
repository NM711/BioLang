#include "../frontend/bytecode/bytecode.hpp"
#include "../utils/debug.hpp"
#include <queue>

enum RuntimeResultState {
  STATE_OK,
  STATE_RUNTIME_ERROR,
  STATE_COMPILE_ERROR,
};

class TypeCheck {
  public:
    bool isBool(Data &d) {
     return d.type == T_BOOLEAN;
    };

    bool isString(Data &d) {
      return d.type == T_STRING;
    };
    bool isInt(Data &d) {
      return d.type == T_INTEGER;
    };

    bool isFloat(Data &d) {
      return d.type == T_FLOAT;
    };
};

class BioVirtualMachine {
  private:
    list<Instruction> chunk;
    stack<Data> vmStack;
    queue<Data> vmQueue;
    TypeCheck checker = TypeCheck();
    RuntimeResultState dispatch(); 
    Instruction look();
    Instruction advance();
  public:
    BioVirtualMachine();
    void setChunk(list<Instruction> &chunk);
    void execute();
};
