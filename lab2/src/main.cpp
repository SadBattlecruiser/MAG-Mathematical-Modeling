#include <iostream>
#include <fstream>
#include <vector>
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
string csv_dlm = ",";             // Туда же разделитель

string ftp_file_name = "out.csv"; // На случай, если есть плагин работы с ftp
string ftp_host = "127.0.0.1";
string ftp_port = "21";
string ftp_login = "admin";
string ftp_password = "12345678";

// Ф-я производной
double der_func(double T_curr);
// Аналитическое решение
double analytic(double time);
// Найти все .plg в папке
vector<string>& find_plug_files(const char* path);
// Попытаться загрузить файлы и инициировать плагины. Возвращает вектор открытых плагинов, надо их закрыть потом.
vector<HMODULE>& register_plugins(const vector<string>& names, vector<ifc_ConfigPlugin*>& configs,
                      vector<ifc_SolverPlugin*>& solvers, vector<ifc_WidgetPlugin*>& widgets);

// Установить глобальные переменные по структуре, которую вернет конфигуратор
// Костыль, но я не смог грамотно передать глобальные переменные в плагин и установить их там, что неудивительно
void set_global(config_struct cs);
// Распечатать в консоль текущие параметры и набор плагинов
void print_report(const vector<ifc_ConfigPlugin*>& configs, const vector<ifc_SolverPlugin*>& solvers,
                  const vector<ifc_WidgetPlugin*>& widgets);



////////////////////////////////////
int main(int argc, char* argv[]) {
  // Имена файлов с плагинами
  cout << "Find .plg files..." << endl;
  vector<string> names;
  if (argc == 2) {  // Если передали путь к плагинами
    names = find_plug_files(argv[1]);
  }
  else {
    names = find_plug_files(".");
  }
  for (size_t i = 0; i < names.size(); i++) {
    cout << "\t" << names[i] << endl;
  }
  cout << "Done." << endl;
  cout << "-----------------------------" << endl;
  // Инициализируем плагины
  cout << "Plugin initialization..." << endl;
  vector<ifc_ConfigPlugin*> configs;
  vector<ifc_SolverPlugin*> solvers;
  vector<ifc_WidgetPlugin*> widgets;
  vector<HMODULE>& plug_files = register_plugins(names, configs, solvers, widgets);
  cout << "Done." << endl;
  cout << "-----------------------------" << endl;
  // Нет конфигураторов - дефолтные настройки, больше 1 конф. - берем 1-й.
  cout << "Configuration..." << endl;
  if (configs.size() == 0) {
    cout << "WARNING: No ifc_ConfigPlugin have been registered" << endl;
    cout << "\tDefault parameters used" << endl;
  }
  else {
    // Выставляем глобальные переменные по возвращенной структуре
    set_global(configs[0]->config());
  }
  cout << "Done." << endl;
  cout << "-----------------------------" << endl;
  // Контейнер с результатами для каждого решателя
  map<string, vector<pair<double, double> > > res_map;
  // Аналитическое решение
  cout << "Analytical calculation..." << endl;
  res_map["analytic"] = vector<pair<double, double> >();
  for (double time_curr = time_beg; time_curr <= time_end*1.00001; time_curr += (time_end-time_beg)/n_steps) {
    res_map["analytic"].push_back(pair<double, double>(time_curr, analytic(time_curr)));
    //cout << "a_time: " << time_curr << endl;
  }
  cout << "Done." << endl;
  cout << "-----------------------------" << endl;
  // Численные решения
  cout << "Numerical calculation..." << endl;
  if (solvers.size() == 0) {
    cout << "WARNING: No ifc_SolverPlugin have been registered" << endl;
  }
  else {
    for (size_t i = 0; i < solvers.size(); i++) {
      cout << solvers[i]->get_name() << endl;
      res_map[solvers[i]->get_name()] = solvers[i]->solve(der_func, T_beg, time_beg, time_end, n_steps + 1);
    }
    cout << "Done." << endl;
  }
  cout << "-----------------------------" << endl;
  // Используем виджеты
  cout << "Using widgets..." << endl;
  if (widgets.size() == 0) {
    cout << "WARNING: No ifc_WidgetPlugin have been registered" << endl;
  }
  else {
    for (size_t i = 0; i < widgets.size(); i++) {
      string settings;
      cout << widgets[i]->get_name() << endl;
      // Обрабатываем отдельно случаи вывода в файл и по ftp - передаем им настройки
      // Вывод в файл
      if (widgets[i]->get_id() == 302) {
        settings = csv_dlm + out_file_name;
      }
      // FTP
      else if (widgets[i]->get_id() == 303) {
        settings = ftp_file_name + "|" + ftp_host + "|" + ftp_port + "|" + ftp_login + "|" + ftp_password;
      }
      // Собственно, дергаем виджет
      if (widgets[i]->execute(res_map, settings.c_str())) {
        cout << "\tErrors are possible" << endl;
      };
    }
    cout << "Done." << endl;
  }
  cout << "-----------------------------" << endl;
  // Печаем отчет
  print_report(configs, solvers, widgets);
  // Закрываем все плагины
  cout << "Close plugins..." << endl;
  for (size_t i = 0; i < configs.size(); i++) {
    delete configs[i];
  };
  for (size_t i = 0; i < solvers.size(); i++) {
    delete solvers[i];
  };
  for (size_t i = 0; i < widgets.size(); i++) {
    delete widgets[i];
  };
  for (size_t i = 0; i < plug_files.size(); i++) {
    FreeLibrary(plug_files[i]);
  };
  cout << "Done." << endl;
  cout << "-----------------------------" << endl;
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
    cout << "WARNING: INVALID_HANDLE_VALUE in FindFirstFile" << endl;
    cout << "\tThere may be no .plg files in the " << path << " folder" << endl;
  }
  return names;
}

vector<HMODULE>& register_plugins(const vector<string>& names, vector<ifc_ConfigPlugin*>& configs, vector<ifc_SolverPlugin*>& solvers, vector<ifc_WidgetPlugin*>& widgets) {
  vector<HMODULE>& plug_files = *(new vector<HMODULE>);
  for (size_t i = 0; i < names.size(); i++) {
    HMODULE curr_plug = LoadLibrary(names[i].c_str());
    if (curr_plug) {
      plug_files.emplace_back(curr_plug);
      ifc_BasePlugin* (*reg_plg)() = (ifc_BasePlugin* (*)()) GetProcAddress(plug_files.back(), "register_plugin");
      if (reg_plg) {
        ifc_BasePlugin* curr_plug_p = reg_plg();
        if (curr_plug_p->is_instance("ifc_SolverPlugin")) {
          solvers.emplace_back((ifc_SolverPlugin*) curr_plug_p);
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
  }
  return plug_files;
}

void set_global(config_struct cs) {
  gam_type = cs.gam_type;
  gam = cs.gam;
  T_s = cs.T_s;
  T_beg = cs.T_beg;
  time_beg = cs.time_beg;
  time_end = cs.time_end;
  n_steps  = cs.n_steps;
  out_file_name = cs.out_file_name;
  csv_dlm = cs.csv_dlm;
}

void print_report(const vector<ifc_ConfigPlugin*>& configs, const vector<ifc_SolverPlugin*>& solvers, const vector<ifc_WidgetPlugin*>& widgets) {
  cout << "-----------------------------" << endl;
  cout << "REPORT" << endl;

  cout << endl << "Model parameters:" << endl;
  cout << "\tgam_type: " << gam_type << endl;
  cout << "\tgam: " << gam << endl;
  cout << "\tT_s: " << T_s << endl;
  cout << "\tT_beg: " << T_beg << endl;
  cout << "\ttime_beg: " << time_beg << endl;
  cout << "\ttime_end: " << time_end << endl;
  cout << "\tn_steps: " << n_steps << endl;

  // Если есть вывод в консоль
  for (size_t i = 0; i < widgets.size(); i++) {
    if (widgets[i]->get_id() == 301) {
      cout << endl << "Console output:" << endl;
      cout << "\tTrue (see above)" << endl;
    }
  }
  // Если есть вывод в файл
  for (size_t i = 0; i < widgets.size(); i++) {
    if (widgets[i]->get_id() == 302) {
      cout << endl << "File output settings:" << endl;
      cout << "\tout_file_name: " << out_file_name << endl;
      cout << "\tcsv_dlm: " << csv_dlm << endl;
    }
  }
  // Если есть вывод через ftp
  for (size_t i = 0; i < widgets.size(); i++) {
    if (widgets[i]->get_id() == 303) {
      cout << endl << "FTP:" << endl;
      cout << endl << "FTP:" << endl;
      cout << endl << "FTP:" << endl;
      cout << endl << "FTP:" << endl;
    }
  }

  // Плагины
  cout << endl << "Registered plugins:" << endl;
  cout << "ifc_ConfigPlugin" << endl;
  if (configs.size() > 0) {
    for (size_t i = 0; i < configs.size(); i++) {
      cout << '\t' << configs[i]->get_name() << " - " << configs[i]->get_title() << endl;
    }
    cout << "  used: " << configs[0]->get_name() << endl;
  }
  else {
    cout << "\tnone" << endl;
  }

  cout << "ifc_SolverPlugin" << endl;
  if (solvers.size() > 0) {
    for (size_t i = 0; i < solvers.size(); i++) {
      cout << '\t' << solvers[i]->get_name() << " - " << solvers[i]->get_title() << endl;
    }
  }
  else {
    cout << "\tnone" << endl;
  }

  cout << "ifc_WidgetPlugin" << endl;
  if (widgets.size() > 0) {
    for (size_t i = 0; i < widgets.size(); i++) {
      cout << '\t' << widgets[i]->get_name() << " - " << widgets[i]->get_title() << endl;
    }
  }
  else {
    cout << "\tnone" << endl;
  }
  cout << "-----------------------------" << endl;
}
