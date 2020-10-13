#ifndef WIDGET_PLUGIN_HPP
#define WIDGET_PLUGIN_HPP

#include <vector>
#include <string>
#include <map>
#include "base_plugin.hpp"

using namespace std;

class ifc_WidgetPlugin : public ifc_BasePlugin {
public:
  virtual int get_id() const = 0;
  virtual const char* get_name() const = 0;
  virtual const char* get_type() const = 0;
  virtual bool is_instance(const char* req_name) const = 0;
  virtual ~ifc_WidgetPlugin() {};
  // Функция запуска виджета, возвращает 0, если всё удачно
  virtual int execute(const map<string, vector<pair<double, double> > >& data, const string& settings) const = 0;
};

#endif
