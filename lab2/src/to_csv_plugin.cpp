#include "widget_plugin.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <cstring>

using namespace std;

class cls_ToCsvPlugin : public ifc_WidgetPlugin {
public:
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  virtual const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  virtual ~cls_ToCsvPlugin();
  // Функция запуска виджета, возвращает 0, если всё удачно
  virtual int execute(map<string, vector<pair<double, double> > >& data, const char* settings = "");
};


extern "C" __declspec(dllexport)
ifc_BasePlugin* register_plugin() {
  cout << "\tcls_ToCsvPlugin register_plugin()" << endl;
  ifc_BasePlugin* temp = new cls_ToCsvPlugin();
  return temp;
};



int cls_ToCsvPlugin::get_id() const {
  return 302;
}


const char* cls_ToCsvPlugin::get_name() const {
  return "cls_ToCsvPlugin";
}


const char* cls_ToCsvPlugin::get_type() const {
  return "ifc_WidgetPlugin";
}


const char* cls_ToCsvPlugin::get_title() const {
  return ".csv writer";
}


bool cls_ToCsvPlugin::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_WidgetPlugin") == 0);
}


cls_ToCsvPlugin::~cls_ToCsvPlugin() {
  cout << "\tcls_ToCsvPlugin destructor" << endl;
}


int cls_ToCsvPlugin::execute(map<string, vector<pair<double, double> > >& data, const char* settings) {
  string out_path;
  char dlm;
  if (strcmp(settings, "") != 0) {
    out_path = settings + 1;
    dlm = settings[0];
  }
  else {
    out_path = "out.csv";
    dlm = ',';
  }
  // Записываем в .csv
  cout << "Writing results to " << out_path << ", delimeter '" << dlm << "'"<< endl;
  ofstream out_file(out_path);
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
