#ifndef SOLVER_PLUGIN_HPP
#define SOLVER_PLUGIN_HPP

#include <vector>
#include "base_plugin.hpp"

class ifc_SolverPlugin : public ifc_BasePlugin {
public:
  virtual int get_id() const = 0;
  virtual const char* get_name() const = 0;
  virtual const char* get_type() const = 0;
  virtual bool is_instance(const char* req_name) const = 0;
  virtual ~ifc_SolverPlugin() {};
  // Функция интегрирования диффура
  virtual std::vector<std::pair<double, double> >& solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps) = 0;
};

#endif
