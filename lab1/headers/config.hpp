#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>

using namespace std;

extern unsigned gam_type;
extern double gam;
extern double T_s;
extern double T_beg;
extern double time_beg;
extern double time_end;
extern unsigned n_steps;

extern unsigned n_solv;             // Количество численных решателей
extern vector<string> solv_names;   // Имена численных решателей
extern char out_file_name[];
extern char csv_dlm;

void create_default_config();
void read_config_from_file(ifstream& inp);

#endif
