#include <vector>
#include <utility>
#include <cstring>
#include <iostream>
#include "solver_plugin.hpp"

using namespace std;

class cls_HeunSolver : public ifc_SolverPlugin {
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  // Функция интегрирования диффура
  virtual vector<pair<double, double> >& solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps);
  virtual ~cls_HeunSolver();
};


extern "C" __declspec(dllexport) ifc_BasePlugin* register_plugin() {
  cout << "\tcls_HeunSolver register_plugin()" << endl;
  return new cls_HeunSolver;
};

// Принимает функцию производной
extern "C" __declspec(dllexport)
vector<pair<double, double> >& cls_HeunSolver::solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps) {
  double step = (end - begin) / n_steps;
  vector<pair<double, double> >& ret = *(new vector<pair<double, double> >());

  double y_curr = y0;
  double y_sh = y0;
  for (unsigned i = 0; i <= n_steps; i++) {
    ret.push_back(pair<double, double>(begin + i * step, y_curr));
    y_sh = y_curr + step * der_func(y_curr);
    y_curr += step / 2 * (der_func(y_curr) + der_func(y_sh));
  };
  return ret;
}

extern "C" __declspec(dllexport)
int cls_HeunSolver::get_id() const {
  return 202;
}

extern "C" __declspec(dllexport)
const char* cls_HeunSolver::get_name() const {
  return "cls_HeunSolver";
}

extern "C" __declspec(dllexport)
const char* cls_HeunSolver::get_type() const {
  return "ifc_SolverPlugin";
}

extern "C" __declspec(dllexport)
const char* cls_HeunSolver::get_title() const {
  return "Heun method";
}

extern "C" __declspec(dllexport)
bool cls_HeunSolver::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_SolverPlugin") == 0);
}

extern "C" __declspec(dllexport)
cls_HeunSolver::~cls_HeunSolver() {
  cout << "\tHeunSolver destructor" << endl;
}
