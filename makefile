CC=g++
FE=./src/frontend

main.out: ./main.cpp ./src/repl.cpp $(FE)/parser.cpp
		$(CC) -o main.out ./main.cpp ./src/repl.cpp ./lexer.o ./debug.o $(FE)/parser.cpp


depen: $(FE)/lexer.cpp ./src/utils/debug.cpp
		$(CC) -c $(FE)/lexer.cpp ./src/utils/debug.cpp

test: ./tests/lexer.test.cpp ./lexer.o
		$(CC) -o test.out ./tests/lexer.test.cpp ./lexer.o

clean:
	rm ./main.out ./lexer.o ./test.out ./debug.o ./lexer.test.o
