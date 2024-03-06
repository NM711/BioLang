CC=g++
FE=./src/frontend

main.out: ./main.cpp ./src/repl.cpp
		$(CC) -o main.out ./main.cpp ./src/repl.cpp ./compiler.o ./vm.o ./parser.o ./lexer.o ./debug.o

depen: $(FE)/lexer/lexer.cpp $(FE)/parser/parser.cpp $(FE)/bytecode/compiler.cpp ./src/runtime/vm.cpp ./src/utils/debug.cpp
		$(CC) -c $(FE)/lexer/lexer.cpp $(FE)/parser/parser.cpp $(FE)/bytecode/compiler.cpp ./src/runtime/vm.cpp ./src/utils/debug.cpp

test: ./tests/lexer.test.cpp
		$(CC) -o test.out ./tests/lexer.test.cpp ./build/dependencies/debug.o ./build/dependencies/lexer.o

run:
		./build/main.out

clean:
		rm -rf ./build ./test.out
