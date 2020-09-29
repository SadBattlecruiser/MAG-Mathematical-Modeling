#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <windows.h>

//#include "euler.cpp"
//#define TESTFUNC std::function<vector<pair<double, double>>&(double (*der_func)(double), double, double, double, unsigned)>

using namespace std;

double der_func(double T_curr);

double gam = 0.1;
double T_s = 25;
double T_beg = 150;
double time_beg = 0;
double time_end = 100;
unsigned n_steps = 1000;

////////////////////////////////////
// В качестве аргументов принимаем имена библиотек, из которых берем решатели, без .dll
int main(int argc, char *argv[]) {
  // Формируем список имен файлов решателей
  unsigned n_solv = argc - 1;
  string* solv_names = new string[n_solv];
  for (int i = 0; i < n_solv; i++) {
    solv_names[i] = argv[i + 1];
    //cout << solv_names[i] + ".dll" << endl;
  }
  // Вектор с результатами для каждого решателя
  vector<vector<pair<double, double> > > res_arr;
  // Вызываем все решатели, доставая поочередно из dll'ок
  HMODULE dll;
  //typedef int FT(vector<pair<double, double>>& (*)(double (*)(double), double, double, double, unsigned));
  //FT* solver = nullptr;
  vector<pair<double, double> >& (*solver)(double (*)(double), double, double, double, unsigned);
  for (int i = 0; i < n_solv; i++) {
    dll = LoadLibrary(argv[i + 1]);
    solver = (vector<pair<double, double> >& (*)(double (*)(double), double, double, double, unsigned)) GetProcAddress(dll, "solver");
    res_arr.push_back(solver(der_func, T_beg, time_beg, time_end, n_steps));
  }

  for (int i = 0; i < res_arr[0].size(); i++) {
    cout << res_arr[0][i].first << ' ' << res_arr[0][i].second << endl;
  }

  return 0;
};
////////////////////////////////////

double der_func(double T_curr) {
  return -gam * (T_curr - T_s);
};
