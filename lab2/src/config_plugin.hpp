#ifndef CONFIG_PLUGIN_HPP
#define CONFIG_PLUGIN_HPP

#include <string>
#include "base_plugin.hpp"

using namespace std;

// Структура, которую возвращают конфигураторы
struct config_struct {
  unsigned gam_type;          // Тип задания gam. 1 - значение, 2 - интервал, 3 - M(gam) и D(gam)
  double gam;
  double T_s;
  double T_beg;
  double time_beg;
  double time_end;
  unsigned n_steps;
  string out_file_name;       // Дефолтное имя выходного файла на случай, если есть плагин записи в файл
  string csv_dlm;             // Туда же разделитель
};

// Сам плагин конфигуратора
class ifc_ConfigPlugin : public ifc_BasePlugin {
public:
  virtual int get_id() const = 0;
  virtual const char* get_name() const = 0;
  virtual const char* get_type() const = 0;
  virtual bool is_instance(const char* req_name) const = 0;
  virtual ~ifc_ConfigPlugin() {};
  // Функция задания глобальных параметров
  virtual config_struct config(const char* settings = "") = 0;
};

#endif
