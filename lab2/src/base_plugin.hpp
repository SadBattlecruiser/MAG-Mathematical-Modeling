#ifndef BASE_PLUGIN_HPP
#define BASE_PLUGIN_HPP

//#include <string>

class ifc_BasePlugin {
public:
  virtual int get_id() const = 0;
  virtual const char* get_name() const = 0;
  virtual const char* get_type() const = 0;
  virtual bool is_instance(const char* req_name) const = 0;
  virtual ~ifc_BasePlugin() {};
};

#endif
