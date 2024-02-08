CC=g++

main.out: main.cpp
		$(CC) -o main.out main.cpp ./frontend/lexer.cpp

clean:
	rm ./main.out
