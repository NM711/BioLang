#include "./vm.hpp"

BioVirtualMachine::BioVirtualMachine() {
  this->vmStack = {};
  this->vmQueue = {};
};

void BioVirtualMachine::setChunk(list<Instruction> &chunk) {
  this->chunk = chunk;
};

// Returns front byte

Instruction BioVirtualMachine::look() {
  return this->chunk.front();
};

// Advances byte in vector

Instruction BioVirtualMachine::advance() {
  if (this->chunk.size() > 0) {
    this->chunk.pop_front();
  };

  return this->look();
};

RuntimeResultState BioVirtualMachine::dispatch() {
  switch (this->look().code) {

    case OP_VALUE:
      this->vmStack.push(*this->look().data);
    return STATE_OK;

    case OP_ADD: {
      Data v1 = this->vmStack.top();
      this->vmStack.pop();
      Data v2 = this->vmStack.top();
      this->vmStack.pop();

      Data data = Data();

      if (v1.isInt() && v2.isInt()) {
        data.value = get<int>(v1.value) + get<int>(v2.value);
      } else if (v1.isFloat() && v2.isFloat()) {
        data.value = get<float>(v1.value) + get<float>(v2.value);
      } else if (v1.isString() && v2.isString()) {
        data.value = get<string>(v1.value) + get<string>(v2.value);
      } else {
        return STATE_RUNTIME_ERROR;
      };

      this->vmStack.push(data);

      if (this->vmStack.top().isInt()) {
        cout << "RESULT: " << get<int>(this->vmStack.top().value) << endl;
      };

       if (this->vmStack.top().isString()) {
        cout << "RESULT: " << get<string>(this->vmStack.top().value) << endl;
      };

      return STATE_OK;
    };

    case OP_EXIT:
      return STATE_OK;

    default:
      cout << "Invalid <OP CODE>!\n";
      exit(1);
  };
};

void BioVirtualMachine::execute() {
  while (this->chunk.size() > 0) {
    RuntimeResultState runtimeState = this->dispatch();
    
    if (runtimeState == STATE_RUNTIME_ERROR) {
      cout << "Runtime Error!\n";
      exit(1);
    };

    this->advance();
  };
};
