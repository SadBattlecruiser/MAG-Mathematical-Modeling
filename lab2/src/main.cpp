#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <windows.h>
#include <solver_plugin.hpp>

using namespace std;

double der_func(double T_curr); // Ф-я производной
double analytic(double time);   // аналитического решения

unsigned gam_type = 1;          // Тип задания gam. 1 - значение, 2 - интервал, 3 - M(gam) и D(gam)
double gam = 0.1;
double T_s = 25;
double T_beg = 150;
double time_beg = 0;
double time_end = 100;
unsigned n_steps = 1000;

unsigned n_solv = 0;            // Количество численных решателей
vector<string> solv_names;      // Имена численных решателей
char out_file_name[100] = "out.csv";
char csv_dlm = ',';

////////////////////////////////////
int main(int argc, char *argv[]) {
  // Контейнеры с плагинами
  vector<ifc_SolverPlugin*> solvers;

  // Контейнер с результатами для каждого решателя
  map<string, vector<pair<double, double> > > res_map;
  HMODULE dll;
  vector<pair<double, double> >& (*solver)(double (*)(double), double, double, double, unsigned);
  solver = (vector<pair<double, double> >& (*)(double (*)(double), double, double, double, unsigned)) GetProcAddress(dll, "solver");


  cout << "Done." << endl;
  cout << "End of program." << endl;
  return 0;
};
////////////////////////////////////

double der_func(double T_curr) {
  return -gam * (T_curr - T_s);
};

double analytic(double time) {
  return T_s + (T_beg - T_s) * exp(-gam*time);
};
