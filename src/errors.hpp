#ifndef INTERPRETER_ERRORS_HPP
#define INTERPRETER_ERRORS_HPP

#include <iostream>
#include <string>

#include "./frontend/lexer/lexer.hpp"

class InterpreterErrorBuilder : public std::exception {
  private:
    std::string name;  
    std::string mssg;
    LineInfo lineInfo;
   

  public:
    InterpreterErrorBuilder(const std::string name, const std::string message, const LineInfo info) {
      this->mssg = message;
      this->lineInfo = info;
      this->name = name;
    };
   
    const std::string &what() {
      const std::string full = this->name + " >>>>>> " + this->mssg + " " + "(col: " + std::to_string(this->lineInfo.col) + ", row: " + std::to_string(this->lineInfo.row) + ")\n";
      return full;
    };
  };

namespace FrontendException {

  class SyntaxError : public InterpreterErrorBuilder {
    public:
      SyntaxError(const std::string message, const LineInfo info) : InterpreterErrorBuilder("SyntaxErro", message, info) {};
   };

  class StaticError : public InterpreterErrorBuilder {
    public:
      StaticError(const std::string message, const LineInfo info) : InterpreterErrorBuilder("StaticError", message, info) {};
  };

  class TypeError : public InterpreterErrorBuilder {
    public:
      TypeError(const std::string message, const LineInfo info) : InterpreterErrorBuilder("TypeError", message, info) {};
  };
};

namespace RuntimeExceptions {
  
};

#endif
