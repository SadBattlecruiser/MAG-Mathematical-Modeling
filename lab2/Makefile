CC=g++
FLAGS= -c -std=c++11
HEADERS= -I .\src

all: main euler_plg heun_plg txt_config_plg to_csv_plg to_console_plg

main: main_o
	$(CC) -o out\\main.exe out\\main.o

main_o:
	$(CC) -o out\\main.o $(FLAGS) $(HEADERS) src\\main.cpp

euler_plg: euler_o
	$(CC) -o out\\euler_plugin.plg -shared out\\euler_plugin.o
euler_o:
	$(CC) -o out\\euler_plugin.o $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\\euler_plugin.cpp

heun_plg: heun_o
	$(CC) -o out\\heun_plugin.plg -shared out\\heun_plugin.o
heun_o:
	$(CC) -o out\\heun_plugin.o $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\\heun_plugin.cpp

txt_config_plg: txt_config_o
	$(CC) -o out\\txt_config_plugin.plg -shared out\\txt_config_plugin.o
txt_config_o:
	$(CC) -o out\\txt_config_plugin.o $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\\txt_config_plugin.cpp

to_csv_plg: to_csv_o
	$(CC) -o out\\to_csv_plugin.plg -shared out\\to_csv_plugin.o

to_csv_o:
	$(CC) -o out\\to_csv_plugin.o $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\\to_csv_plugin.cpp

to_console_plg: to_console_o
	$(CC) -o out\\to_console_plugin.plg -shared out\\to_console_plugin.o

to_console_o:
	$(CC) -o out\\to_console_plugin.o $(FLAGS) $(HEADERS) -DBUILDING_EXAMPLE_DLL src\\to_console_plugin.cpp
