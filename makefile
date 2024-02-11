CC=g++

main.out: ./main.cpp ./lexer.o
		$(CC) -o main.out ./main.cpp ./lexer.o

lexer.o: ./frontend/lexer.cpp
		$(CC) -c ./frontend/lexer.cpp

test: ./tests/lexer.test.cpp ./lexer.o
		$(CC) -o test.out ./tests/lexer.test.cpp ./lexer.o


clean:
	rm ./main.out ./lexer.o ./test.out ./lexer.test.o
