CC=g++
FLAGS= -c -std=c++11
HEADERS= -I .\src

all: main euler_dll heun_dll

main: main_o config_o
	$(CC) out\main.o out\config.o -o out\main.exe
main_o:
	$(CC) $(FLAGS) $(HEADERS) src\main.cpp -o out\main.o

config_o:
	$(CC) $(FLAGS) $(HEADERS) src\config.cpp -o out\config.o

euler_dll: euler_o
	$(CC) -shared out\euler.o -o out\euler.dll
euler_o:
	$(CC) $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\euler.cpp -o out\euler.o

heun_dll: heun_o
	$(CC) -shared out\heun.o -o out\heun.dll
heun_o:
	$(CC) $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\heun.cpp -o out\heun.o
