#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <string>
#include <map>
#include <utility>
#include <cmath>
#include <windows.h>
#include <config_plugin.hpp>      // Задают параметры модели
#include <solver_plugin.hpp>      // Численные решатели
#include <widget_plugin.hpp>      // Выводят результат

using namespace std;

unsigned gam_type = 1;            // Тип задания gam. 1 - значение, 2 - интервал, 3 - M(gam) и D(gam)
double gam = 0.1;
double T_s = 25;
double T_beg = 150;
double time_beg = 0;
double time_end = 100;
unsigned n_steps = 1000;
string out_file_name = "out.csv"; // Дефолтное имя выходного файла на случай, если есть плагин записи в файл
char csv_dlm = ',';               // Туда же разделитель


// Ф-я производной
double der_func(double T_curr);
// Аналитическое решение
double analytic(double time);
// Найти все .plg в папке
vector<string>& find_plug_files(const char* path);
// Попытаться загрузить файлы и инициировать плагины
void register_plugins(const vector<string>& names, list<ifc_ConfigPlugin*>& configs,
                      list<ifc_SolverPlugin*>& solvers, list<ifc_WidgetPlugin*>& widgets);

// Установить глобальные переменные по структуре, которую вернет конфигуратор
// Костыль, но я не смог грамотно передать глобальные переменные в плагин и установить их там, что неудивительно
void set_global(config_struct cs);
// Распечатать текущие параметры и набор плагинов
void print_state(const list<ifc_ConfigPlugin*>& configs, const list<ifc_SolverPlugin*>& solvers,
                  const list<ifc_WidgetPlugin*>& widgets);

////////////////////////////////////
int main(int argc, char* argv[]) {
  // Имена файлов с плагинами
  vector<string> names;
  if (argc == 2) {  // Если передали путь к плагинами
    names = find_plug_files(argv[1]);
  }
  else {
    names = find_plug_files(".");
  }
  for (size_t i = 0; i < names.size(); i++) {
    cout << names[i] << endl;
  }
  // Инициализируем плагины
  list<ifc_ConfigPlugin*> configs;
  list<ifc_SolverPlugin*> solvers;
  list<ifc_WidgetPlugin*> widgets;
  register_plugins(names, configs, solvers, widgets);


  cout << "TEST SOLVER" << endl; // Падаем
  (*solvers.begin())->solve(der_func, T_beg, time_beg, time_end, n_steps + 1);


  // Нет конфигураторов - дефолтные настройки, больше 1 конф. - берем 1-й.
  if (configs.size() == 0) {
    cout << "WARNING: No ifc_ConfigPlugin have been registered" << endl;
  }
  else {

    // ПОЧЕМУ-ТО НЕЛЬЗЯ ОБРАТИТЬСЯ ПО УКАЗАТЕЛЮ ЗДЕСЬ
    cout << "before set_global" << endl;
    // Выставляем глобальные переменные по возвращенной структуре
    cout << (*configs.begin())->get_name() << endl;
    config_struct temp = (*configs.begin())->config(string());
    cout << "temp" << endl;
    set_global(temp);
    cout << "after set_global" << endl;
  }
  // Контейнер с результатами для каждого решателя
  map<string, vector<pair<double, double> > > res_map;
  // Аналитическое решение
  cout << "Analytical calculation..." << endl;
  res_map["analytic"] = vector<pair<double, double> >();
  for (double time_curr = time_beg; time_curr <= time_end*1.00001; time_curr += (time_end-time_beg)/n_steps) {
    res_map["analytic"].push_back(pair<double, double>(time_curr, analytic(time_curr)));
    //cout << "a_time: " << time_curr << endl;
  }
  // Численные решения
  if (solvers.size() == 0) {
    cout << "WARNING: No ifc_SolverPlugin have been registered" << endl;
  }


  print_state(configs, solvers, widgets);
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

vector<string>& find_plug_files(const char* path) {
  vector<string>& names = *(new vector<string>);
  WIN32_FIND_DATA FindFileData;
  string files(path);
  files.append("\\\\*.plg");
  HANDLE hf = FindFirstFile(files.c_str(), &FindFileData);
  if (hf != INVALID_HANDLE_VALUE)
  {
    do
    {
      string curr_name(FindFileData.cFileName);
      names.push_back(curr_name);
    } while (FindNextFile(hf, &FindFileData) != 0);
    FindClose(hf);
  }
  else {
    cout << "INVALID_HANDLE_VALUE in FindFirstFile" << endl;
    cout << "There may be no .plg files in the " << path << " folder" << endl;
  }
  return names;
}

void register_plugins(const vector<string>& names, list<ifc_ConfigPlugin*>& configs,
                      list<ifc_SolverPlugin*>& solvers, list<ifc_WidgetPlugin*>& widgets) {
  for (size_t i = 0; i < names.size(); i++) {
    HMODULE dll = LoadLibrary(names[i].c_str());
    if (dll) {
      ifc_BasePlugin* (*reg_plg)() = (ifc_BasePlugin* (*)()) GetProcAddress(dll, "register_plugin");
      if (reg_plg) {
        ifc_BasePlugin* curr_plug_p = reg_plg();
        cout << "REG PLUG" << endl;
        cout << curr_plug_p->get_title() << endl;
        if (curr_plug_p->is_instance("ifc_SolverPlugin")) {
          cout << "------" << endl;
          solvers.emplace_back((ifc_SolverPlugin*) curr_plug_p);
          cout << "PUSH IN SOLVERS" << endl;
          cout << "NOW SOLVER.SIZE():" << solvers.size() << endl;
          cout << (*(solvers.end()))->get_title() << endl;
          cout << (*(solvers.begin()))->get_title() << endl;
          cout << "------" << endl;
        }
        else if (curr_plug_p->is_instance("ifc_WidgetPlugin")) {
          widgets.emplace_back((ifc_WidgetPlugin*) curr_plug_p);
        }
        else if (curr_plug_p->is_instance("ifc_ConfigPlugin")) {
          configs.emplace_back((ifc_ConfigPlugin*) curr_plug_p);
        }
        else {
          cout << "WARNING: register_plugin() was found, but type " << curr_plug_p->get_type() << " is unknown" << endl;;
        }
      }
      else {
        cout << "WARNING: register_plugin() not found in " << names[i] << endl;
      }
    }
    else {
      cout << "WARNING: Library load error " << names[i] << endl;
    }
    FreeLibrary(dll);
  }
  return;
}

void set_global(config_struct cs) {
  cout << "in set_global()" << endl;
  gam_type = cs.gam_type;
  gam = cs.gam;
  T_s = cs.T_s;
  T_beg = cs.T_beg;
  time_beg = cs.time_beg;
  time_end = cs.time_end;
  n_steps  = cs.n_steps;
  out_file_name = cs.out_file_name;
  csv_dlm = cs.csv_dlm;
  cout << "end set_global()" << endl;
}

void print_state(const list<ifc_ConfigPlugin*>& configs, const list<ifc_SolverPlugin*>& solvers,
                  const list<ifc_WidgetPlugin*>& widgets) {
  cout << "-------" << endl;
  cout << "Current state." << endl;
  cout << "Registered plugins:" << endl;

  cout << "ifc_ConfigPlugin" << endl;
  if (configs.size() > 0) {
    for (auto it = configs.begin(); it != configs.end(); ++it) {
      cout << '\t' << (*it)->get_name() << " - " << (*it)->get_title() << endl;
    }
    cout << "  used: " << (*(configs.begin()))->get_name() << endl;
  }
  else {
    cout << "\tnone" << endl;
  }

  cout << "ifc_SolverPlugin" << endl;
  if (solvers.size() > 0) {
    for (auto it = solvers.begin(); it != solvers.end(); ++it) {
      cout << '\t' << (*it)->get_name() << " - " << (*it)->get_title() << endl;
    }
  }
  else {
    cout << "\tnone" << endl;
  }

  cout << "ifc_WidgetPlugin" << endl;
  if (widgets.size() > 0) {
    for (auto it = widgets.begin(); it != widgets.end(); ++it) {
      cout << '\t' << (*it)->get_name() << " - " << (*it)->get_title() << endl;
    }
  }
  else {
    cout << "\tnone" << endl;
  }

  cout << "Model parameters:" << endl;
  cout << "gam_type: " << gam_type << endl;
  cout << "gam: " << gam << endl;
  cout << "T_s: " << T_s << endl;
  cout << "T_beg: " << T_beg << endl;
  cout << "time_beg: " << time_beg << endl;
  cout << "time_end: " << time_end << endl;
  cout << "n_steps: " << n_steps << endl;

  cout << "Settings:" << endl;
  // Если есть вывод в файл
  for (auto it = widgets.begin(); it != widgets.end(); ++it) {
    if ((*it)->get_id() == 302) {
      cout << "out_file_name: " << out_file_name << endl;
      cout << "csv_dlm: " << csv_dlm << endl;
    }
  }

  cout << "-------" << endl;
}
