CC = g++
CFLAGS = --std=c++17 -Wall -Werror -pedantic
LIB = -lsfml-graphics -lsfml-audio -lsfml-window -lsfml-system

all: Sokoban Sokoban.a

Sokoban: main.o Sokoban.o
	$(CC) $(CFLAGS) -o Sokoban main.o Sokoban.o $(LIB)

Sokoban.a: main.o Sokoban.o
	ar rcs Sokoban.a Sokoban.o main.o

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp -o main.o

Sokoban.o: Sokoban.cpp Sokoban.hpp
	$(CC) $(CFLAGS) -c Sokoban.cpp

lint:
	cpplint *.cpp *.hpp

clean:
	rm *.o Sokoban Sokoban.a
