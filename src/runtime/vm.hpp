#include "../frontend/bytecode/bytecode.hpp"
#include "../utils/debug.hpp"
#include <queue>

enum RuntimeResultState {
  STATE_OK,
  STATE_RUNTIME_ERROR,
  STATE_COMPILE_ERROR,
};

class BioVirtualMachine {
  private:
    list<Instruction> chunk;
    stack<Data> vmStack;
    queue<Data> vmQueue;
    RuntimeResultState dispatch(); 
    Instruction look();
    Instruction advance();
  public:
    BioVirtualMachine();
    void setChunk(list<Instruction> &chunk);
    void execute();
};
