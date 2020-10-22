#include "widget_plugin.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <cstring>
#include <sstream>
#include <wininet.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

class cls_ToFtpPlugin : public ifc_WidgetPlugin {
public:
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  virtual const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  virtual ~cls_ToFtpPlugin();
  // Функция запуска виджета, возвращает 0, если всё удачно
  virtual int execute(map<string, vector<pair<double, double> > >& data, const char* settings = "");
private:
  // Чтобы создавать временный .csv
  int write_csv(map<string, vector<pair<double, double> > >& data);
};


extern "C" __declspec(dllexport)
ifc_BasePlugin* register_plugin() {
  cout << "\tcls_ToFtpPlugin register_plugin()" << endl;
  ifc_BasePlugin* temp = new cls_ToFtpPlugin();
  return temp;
};



int cls_ToFtpPlugin::get_id() const {
  return 303;
}


const char* cls_ToFtpPlugin::get_name() const {
  return "cls_ToFtpPlugin";
}


const char* cls_ToFtpPlugin::get_type() const {
  return "ifc_WidgetPlugin";
}


const char* cls_ToFtpPlugin::get_title() const {
  return "ftp client";
}


bool cls_ToFtpPlugin::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_WidgetPlugin") == 0);
}


cls_ToFtpPlugin::~cls_ToFtpPlugin() {
  cout << "\tcls_ToFtpPlugin destructor" << endl;
}


int cls_ToFtpPlugin::execute(map<string, vector<pair<double, double> > >& data, const char* settings) {
  string file_name;
  string host;
  string port_str;
  unsigned port = 0;
  string login;
  string password;
  // Разделяем settings обратно
  istringstream iss(settings);
  getline(iss, file_name, '|');
  getline(iss, host, '|');
  getline(iss, port_str, '|');
  sscanf(port_str.c_str(), "%u", &port);
  getline(iss, login, '|');
  getline(iss, password, '|');
  cout << "\t!!!!!" << file_name << endl;
  cout << "\t!!!!!" << host << endl;
  cout << "\t!!!!!" << port << endl;
  cout << "\t!!!!!" << login << endl;
  cout << "\t!!!!!" << password << endl;
  // Устанавливаем соединение
  HINTERNET hInternet;
  HINTERNET hFtpSession;
  hInternet = InternetOpen(NULL, INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
  if (hInternet == NULL)
  {
    cout << "\tcls_ToFtpPlugin ERROR: " << GetLastError() << endl;
  }
  else
  {
    hFtpSession = InternetConnect(hInternet, host.c_str(), port, login.c_str(), password.c_str(), INTERNET_SERVICE_FTP, 0, 0);
    if (hFtpSession == NULL)
    {
      cout << "\tcls_ToFtpPlugin ERROR: " << GetLastError() << endl;
    }
    else
    {
      // Пишем временный файл
      write_csv(data);
      if (!FtpPutFile(hFtpSession, "temp_ftp_out.csv", file_name.c_str(), FTP_TRANSFER_TYPE_ASCII, 0))
      {
        cout << "\tcls_ToFtpPlugin ERROR: " << GetLastError() << endl;
      }
    }
  }
  // Удаляем временный файл
  remove("temp_ftp_out.csv");
  return 0;
}

int cls_ToFtpPlugin::write_csv(map<string, vector<pair<double, double> > >& data) {
  char dlm = ',';
  ofstream out_file("temp_ftp_out.csv");
  // Названия столбцов
  vector<string> names;
  for (auto it = data.begin(); it != data.end(); it++) {
    //cout << "test" << endl;
    names.emplace_back(it->first);
  }
  // Записываем их в файл
  for (size_t i = 0; i < names.size(); i++){
    out_file << names[i];
    if (i != names.size() - 1) {
      out_file << dlm;
    }
    else {
      out_file << endl;
    }
  }
  // Значения в файл
  for (size_t i = 0; i < data["analytic"].size(); i++) {
    // Сначала время, которое берем из вектора аналитического решения
    out_file << data["analytic"][i].first << dlm;
    // Затем сами значения для всех решений, включая аналитическое
    for (size_t j = 0; j < names.size(); j++){
      //cout << "i: " << i << " j: "<< j << " names[j]: " << names[j] << " data[names[j]][i].second: "<< data[names[j]][i].second << endl;
      out_file << data[names[j]][i].second;
      if (j != names.size() - 1) {
        out_file << dlm;
      }
      else {
        out_file << endl;
      }
    }
  }
  out_file.close();
  return 0;
}
