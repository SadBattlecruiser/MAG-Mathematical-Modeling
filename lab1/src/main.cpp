#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <utility>
#include <cmath>
#include <windows.h>
#include <config.hpp>

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
// В качестве аргументов принимаем имена библиотек, из которых берем решатели, без .dll
int main(int argc, char *argv[]) {
  ifstream config_file("config.txt");
  if (!config_file) {
    cout << "A default config.txt was created." << endl;
    create_default_config();
    config_file.open("config.txt");
  }
  // Устанавливаем все глобальные переменные
  read_config_from_file(config_file);
  config_file.close();

  // Контейнер с результатами для каждого решателя
  map<string, vector<pair<double, double> > > res_map;

  // Аналитическое решение
  cout << "Analytical calculation..." << endl;
  vector<pair<double, double> > analytic_res;
  for (double time_curr = time_beg; time_curr <= time_end*1.00001; time_curr += (time_end-time_beg)/n_steps) {
    analytic_res.push_back(pair<double, double>(time_curr, analytic(time_curr)));
    //cout << "a_time: " << time_curr << endl;
  }
  res_map["analytic"] = analytic_res;
  cout << "Done." << endl;

  // Вызываем все численые решатели, доставая поочередно из dll'ок
  HMODULE dll;
  vector<pair<double, double> >& (*solver)(double (*)(double), double, double, double, unsigned);
  for (int i = 0; i < n_solv; i++) {
    cout << "Numerical calculation " << solv_names[i] << "..." << endl;
    dll = LoadLibrary(solv_names[i].c_str());
    solver = (vector<pair<double, double> >& (*)(double (*)(double), double, double, double, unsigned)) GetProcAddress(dll, "solver");
    res_map[solv_names[i]] = solver(der_func, T_beg, time_beg, time_end, n_steps + 1);
    cout << "Done." << endl;
  }

  // Записываем в .csv
  cout << "Writing results..." << endl;
  ofstream out_file(out_file_name);
  // Названия столбцов
  out_file << "time" << csv_dlm << "analytic";
  for (int i = 0; i < n_solv; i++) {
    out_file << csv_dlm << solv_names[i];
  }
  out_file << endl;
  // Для каждого шага сначала отдельно аналитическое, потом все численные
  for (int i = 0; i <= n_steps; i++) {
    out_file << res_map["analytic"][i].first << csv_dlm << res_map["analytic"][i].second;
    for (int j = 0; j < n_solv; j++) {
      out_file << csv_dlm << res_map[solv_names[j]][i].second;
    }
    out_file << endl;
  }
  out_file.close();
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
