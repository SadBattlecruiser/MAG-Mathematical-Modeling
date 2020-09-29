CC=g++
FLAGS= -c -std=c++11

all: main euler_dll heun_dll

main: main_o
	$(CC) out\main.o -o out\main.out
main_o:
	$(CC) $(FLAGS) src\main.cpp -o out\main.o

euler_dll: euler_o
	$(CC) -shared out\euler.o -o out\euler.dll
euler_o:
	$(CC) $(FLAGS) -DBUILDING_EXAMPLE_DLL src\euler.cpp -o out\euler.o

heun_dll: heun_o
	$(CC) -shared out\heun.o -o out\heun.dll
heun_o:
	$(CC) $(FLAGS) -DBUILDING_EXAMPLE_DLL src\heun.cpp -o out\heun.o

clean:
	rm -rf out/*.o all
