#include "../frontend/bytecode/bytecode.hpp"
#include <queue>

enum RuntimeResultState {
  STATE_OK,
  STATE_RUNTIME_ERROR,
  STATE_COMPILE_ERROR,
};

class BioVirtualMachine {
  private:
    // instruction pointer
    unsigned ip;
    stack<BioChunk> chunks;
    queue<BioChunk> queue;
  public:
    void execute();
};
