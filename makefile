CC=g++
FE=./src/frontend

main.out: ./main.cpp ./src/repl.cpp $(FE)/parser/parser.cpp
		$(CC) -o main.out ./main.cpp ./src/repl.cpp ./lexer.o ./debug.o $(FE)/parser/parser.cpp

depen: $(FE)/lexer/lexer.cpp ./src/utils/debug.cpp
		$(CC) -c $(FE)/lexer/lexer.cpp ./src/utils/debug.cpp

test: ./tests/lexer.test.cpp
		$(CC) -o test.out ./tests/lexer.test.cpp ./build/dependencies/debug.o ./build/dependencies/lexer.o

run:
		./build/main.out

clean:
		rm -rf ./build ./test.out
