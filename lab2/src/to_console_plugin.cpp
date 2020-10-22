#include "widget_plugin.hpp"
#include <cstdio>
#include <iostream>
#include <map>
#include <cstring>

using namespace std;

class cls_ToConsolePlugin : public ifc_WidgetPlugin {
public:
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  virtual const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  virtual ~cls_ToConsolePlugin();
  // Функция запуска виджета, возвращает 0, если всё удачно
  virtual int execute(map<string, vector<pair<double, double> > >& data, const char* settings = "");
};


extern "C" __declspec(dllexport)
ifc_BasePlugin* register_plugin() {
  cout << "\tcls_ToCsvPlugin register_plugin()" << endl;
  ifc_BasePlugin* temp = new cls_ToConsolePlugin();
  return temp;
};



int cls_ToConsolePlugin::get_id() const {
  return 301;
}


const char* cls_ToConsolePlugin::get_name() const {
  return "cls_ToConsolePlugin";
}


const char* cls_ToConsolePlugin::get_type() const {
  return "ifc_WidgetPlugin";
}


const char* cls_ToConsolePlugin::get_title() const {
  return "prints the result to the console";
}


bool cls_ToConsolePlugin::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_WidgetPlugin") == 0);
}


cls_ToConsolePlugin::~cls_ToConsolePlugin() {
  cout << "\tcls_ToConsolePlugin destructor" << endl;
}


int cls_ToConsolePlugin::execute(map<string, vector<pair<double, double> > >& data, const char* settings) {
  // Названия столбцов
  vector<string> names;
  for (auto it = data.begin(); it != data.end(); it++) {
    //cout << "test" << endl;
    names.emplace_back(it->first);
  }
  // Выводим в консоль названия
  cout << "\t-------------------------" << endl;
  cout << "\tCalculation results" << endl;
  cout << "\t        Time";
  for (size_t i = 0; i < names.size(); i++){
    //out_file << names[i];
    printf("\t%12.12s", names[i].c_str());
  }
  cout << endl;
  // Выводим в консоль значения
  for (size_t i = 0; i < data["analytic"].size(); i++) {
    // Сначала время, которое берем из вектора аналитического решения
    printf("\t%12f", data["analytic"][i].first);
    //out_file << data["analytic"][i].first << dlm;
    // Затем сами значения для всех решений, включая аналитическое
    for (size_t j = 0; j < names.size(); j++){
      //out_file << data[names[j]][i].second;
      printf("\t%12f", data[names[j]][i].second);
    }
    cout << endl;
  }
  cout << "\tEnd of calculation results" << endl;
  cout << "\t-------------------------" << endl;
  return 0;
}
