CC=g++
FLAGS= -c -std=c++11

all: main eulerdll

main: maino
	$(CC) out\main.o -o out\main.out
maino:
	$(CC) $(FLAGS) src\main.cpp -o out\main.o

eulerdll: eulero
	$(CC) -shared out\euler.o -o out\euler.dll
eulero:
	$(CC) $(FLAGS) -DBUILDING_EXAMPLE_DLL src\euler.cpp -o out\euler.o

clean:
	rm -rf out\*.o all
