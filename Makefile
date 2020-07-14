CC = g++
LD = g++
CCFLAGS = -Wall -pedantic -std=c++11 -O3
LDFLAGS =

main: main.o
	$(CC) $(CCFLAGS) main.o -o main -lsfml-graphics -lsfml-window -lsfml-system
main.o: main.cpp ANN_Lib.hpp
	$(CC) $(CCFLAGS) -c -std=c++11 ./main.cpp
snake: snake.cpp ANN_Lib.hpp
	$(CC) $(CCFLAGS) -c -std=c++11 ./snake.cpp
	$(CC) $(CCFLAGS) snake.o -o snake -lsfml-graphics -lsfml-window -lsfml-system
make clean:
	rm main snake snake.o main.o
