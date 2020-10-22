#include <fstream>
#include <iostream>
#include <cstring>
#include <sstream>
#include <vector>
#include "config_plugin.hpp"

using namespace std;

void create_default_config();
void read_config_from_file(ifstream& inp);

class cls_TxtConfig : public ifc_ConfigPlugin {
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  virtual const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  virtual ~cls_TxtConfig();
  // Функция задания глобальных параметров
  virtual config_struct config(const char* settings = "");
};



extern "C" __declspec(dllexport)
ifc_BasePlugin* register_plugin() {
  cout << "\tcls_TxtConfig register_plugin()" << endl;
  ifc_BasePlugin* temp = new cls_TxtConfig();
  return temp;
};

void create_default_config() {
  ofstream out("config.txt");
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
  out << "\t# Настройки для виджета, выводящего результаты в файл" << endl;
  out << "\t# Имя выходного .csv файла" << endl;
  out << "out_file_name=out.csv" << endl;
  out << "\t# Разделитель в выходном файле" << endl;
  out << "csv_dlm=," << endl;
  out << "\t# Настройки для виджета, передающего результаты по FTP" << endl;
  out << "\t# Имя создаваемого на сервере .csv файла" << endl;
  out << "ftp_file_name=out.csv" << endl;
  out << "\t# Хост" << endl;
  out << "ftp_host=127.0.0.1" << endl;
  out << "\t# Порт" << endl;
  out << "ftp_port=21" << endl;
  out << "\t# Логин" << endl;
  out << "ftp_login=admin" << endl;
  out << "\t# Пароль" << endl;
  out << "ftp_password=12345678" << endl;
  out.close();
};

void read_config_from_file(ifstream& inp, config_struct& cs) {
  //cout << "Reading configuration from file..." << endl;
  string curr_line;
  string var_name;
  unsigned eq_pos = 0; // Будет указывать на знак равенства
  //cout << "!!!!!! IN read_config" << curr_line << endl;
  while(getline(inp, curr_line)) {
    //cout << "!! " << curr_line << endl;
    if(curr_line[1] != '#') {
      eq_pos = curr_line.find("=");
      var_name = curr_line.substr(0, eq_pos);
      if (var_name == "gam_type") {
        sscanf(curr_line.c_str()+eq_pos+1, "%u", &cs.gam_type);
        cout << "\tset gam_type" << endl;
      }
      else if (var_name == "gam") {
        if (cs.gam_type == 1) {
          sscanf(curr_line.c_str()+eq_pos+1, "%lf", &cs.gam);
          cout << "\tset gam" << endl;
        }
        else if (cs.gam_type == 2) {
          cout << "\tgam_type 2 not supported" << &endl;
          exit(2);
        }
        else if (cs.gam_type == 3) {
          cout << "\tgam_type 3 not supported" << &endl;
          exit(3);
        }
      }
      else if (var_name == "T_s") {
        sscanf(curr_line.c_str()+eq_pos+1, "%lf", &cs.T_s);
        cout << "\tset T_s" << endl;
      }
      else if (var_name == "T_beg") {
        sscanf(curr_line.c_str()+eq_pos+1, "%lf", &cs.T_beg);
        cout << "\tset T_beg" << endl;
      }
      else if (var_name == "time_beg") {
        sscanf(curr_line.c_str()+eq_pos+1, "%lf", &cs.time_beg);
        cout << "\tset time_beg" << endl;
      }
      else if (var_name == "time_end") {
        sscanf(curr_line.c_str()+eq_pos+1, "%lf", &cs.time_end);
        cout << "\tset time_end" << endl;
      }
      else if (var_name == "n_steps") {
        sscanf(curr_line.c_str()+eq_pos+1, "%u", &cs.n_steps);
        cout << "\tset n_steps" << endl;
      }
      // Вывод в файл
      else if (var_name == "out_file_name") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", temp);
        cs.out_file_name = temp;
        cout << "\tset out_file_name" << endl;
      }
      else if (var_name == "csv_dlm") {
        char temp[1];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", temp);
        cs.csv_dlm = temp;
        cout << "\tset scv_dlm" << endl;
      }
      // FTP
      else if (var_name == "ftp_file_name") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", &temp);
        cs.ftp_file_name = temp;
        cout << "\tset ftp_file_name" << endl;
      }
      else if (var_name == "ftp_host") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", &temp);
        cs.ftp_host = temp;
        cout << "\tset ftp_host" << endl;
      }
      else if (var_name == "ftp_port") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", &temp);
        cs.ftp_port = temp;
        cout << "\tset ftp_port" << endl;
      }
      else if (var_name == "ftp_login") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", &temp);
        cs.ftp_login = temp;
        cout << "\tset ftp_login" << endl;
      }
      else if (var_name == "ftp_password") {
        char temp[512];
        sscanf(curr_line.c_str()+eq_pos+1, "%s", &temp);
        cs.ftp_password = temp;
        cout << "\tset ftp_password" << endl;
      }
    };
  };
  //cout << "Done." << endl;
};




int cls_TxtConfig::get_id() const {
  return 101;
}


const char* cls_TxtConfig::get_name() const {
  return "cls_TxtConfig";
}


const char* cls_TxtConfig::get_type() const {
  return "ifc_ConfigPlugin";
}


const char* cls_TxtConfig::get_title() const {
  return "txt config reader";
}


bool cls_TxtConfig::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_ConfigPlugin") == 0);
}


cls_TxtConfig::~cls_TxtConfig() {
  cout << "\tcls_TxtConfig destructor" << endl;
}

config_struct cls_TxtConfig::config(const char* settings) {
  config_struct ret;
  // Можно было бы сделать отдельный конструктор для структуры, но...
  ret.gam_type = 1;
  ret.gam = 0.1;
  ret.T_s = 25;
  ret.T_beg = 150;
  ret.time_beg = 0;
  ret.time_end = 100;
  ret.n_steps = 1000;
  ret.out_file_name = "out.csv";
  ret.csv_dlm = ',';
  ret.ftp_file_name = "out.csv"; // На случай, если есть плагин работы с ftp
  ret.ftp_host = "127.0.0.1";
  ret.ftp_port = "21";
  ret.ftp_login = "admin";
  ret.ftp_password = "12345678";

  ifstream config_file("config.txt");
  if (!config_file) {
    cout << "WARNING: config.txt not found. ";
    cout << "A default config.txt was created." << endl;
    create_default_config();
    config_file.open("config.txt");
  }
  //config_file.open("config.txt");
  read_config_from_file(config_file, ret);
  config_file.close();
  return ret;
}
