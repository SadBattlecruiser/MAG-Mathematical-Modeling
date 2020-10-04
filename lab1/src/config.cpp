#include "config.hpp"

using namespace std;

void create_default_config() {
  ofstream out("config.txt");
  out << "\t# Имена библиотек с решателями без .dll" << endl;
  out << "\t# В каждом файле должен быть определена функция vector<pair<double, double> >& solver(double (*)(double), double, double, double, unsigned);" << endl;
  out << "solv_names=euler,heun" << endl;
  out << "\t# Тип задания коэффициента гамма математической модели" << endl;
  out << "\t# 1 для точного значения, 2 для интервала, 3 для M(gam) и D(gam)" << endl;
  out << "gam_type=1" << endl;
  out << "\t# Коэффициент гамма математической модели. Для gam_type=1 - одно число, для 2 и 3 - два числа через запятую" << endl;
  out << "gam=0.1" << endl;
  out << "\t# Температура окружающей среды" << endl;
  out << "T_s=25." << endl;
  out << "\t# Начальная температура тела" << endl;
  out << "T_beg=150." << endl;
  out << "\t# Начальное время" << endl;
  out << "time_beg=0." << endl;
  out << "\t# Конечное время" << endl;
  out << "time_end=100." << endl;
  out << "\t# Желаемое количество шагов интегрирования" << endl;
  out << "n_steps=1000" << endl;
  out << "\t# Имя выходного .csv файла" << endl;
  out << "out_file_name=out.csv" << endl;
  out << "\t# Разделитель в выходном файле" << endl;
  out << "csv_dlm=," << endl;
  out.close();
};

void read_config_from_file(ifstream& inp) {
  cout << "Reading configuration from file..." << endl;
  string curr_line;
  string var_name;
  unsigned eq_pos = 0; // Будет указывать на знак равенства
  while(getline(inp, curr_line)) {
    if(curr_line[1] != '#') {
      eq_pos = curr_line.find("=");
      var_name = curr_line.substr(0, eq_pos);
      if(var_name == "solv_names") {
        // Формируем список имен файлов решателей
        // Вышло откровенно плохо, но работает
        unsigned prev_dlm = eq_pos;
        unsigned next_dlm = eq_pos + curr_line.substr(prev_dlm + 1).find(",") + 1;
        while (next_dlm != prev_dlm) {
          solv_names.push_back(curr_line.substr(prev_dlm + 1, next_dlm - prev_dlm - 1));
          prev_dlm = next_dlm;
          next_dlm += curr_line.substr(prev_dlm + 1).find(",") + 1;
        }
        solv_names.push_back(curr_line.substr(next_dlm + 1));
        n_solv = solv_names.size();
      }
      else if (var_name == "gam_type") {
        sscanf(curr_line.c_str()+eq_pos+1, "%u", &gam_type);
      }
      else if (var_name == "gam") {
        if (gam_type == 1) {
          sscanf(curr_line.c_str()+eq_pos+1, "%d", &gam);
        }
        else if (gam_type == 2) {
          cout << "gam_type 2 not supported" << &endl;
          exit(2);
        }
        else if (gam_type == 3) {
          cout << "gam_type 3 not supported" << &endl;
          exit(3);
        }
      }
      else if (var_name == "T_s") {
        sscanf(curr_line.c_str()+eq_pos+1, "%d", &T_s);
      }
      else if (var_name == "T_beg") {
        sscanf(curr_line.c_str()+eq_pos+1, "%d", &T_beg);
      }
      else if (var_name == "time_beg") {
        sscanf(curr_line.c_str()+eq_pos+1, "%d", &time_beg);
      }
      else if (var_name == "time_end") {
        sscanf(curr_line.c_str()+eq_pos+1, "%d", &time_end);
      }
      else if (var_name == "n_steps") {
        sscanf(curr_line.c_str()+eq_pos+1, "%u", &n_steps);
      }
      else if (var_name == "out_file_name") {
        sscanf(curr_line.c_str()+eq_pos+1, "%s", out_file_name);
      }
      else if (var_name == "csv_dlm") {
        csv_dlm = curr_line[eq_pos + 1];
      }
    };
  };
  cout << "Done." << endl;
};
