#include <vector>
#include <utility>
#include <cstring>
#include <iostream>
#include "solver_plugin.hpp"

using namespace std;

class cls_EulerSolver : public ifc_SolverPlugin {
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  virtual bool is_instance(const char* req_name) const;
  // Функция интегрирования диффура
  virtual vector<pair<double, double> >& solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps);
  virtual ~cls_EulerSolver();
};

extern "C" __declspec(dllexport) ifc_BasePlugin* registerPlugin() {
  return new cls_EulerSolver;
};

// Принимает функцию производной
extern "C" __declspec(dllexport)
vector<pair<double, double> >& cls_EulerSolver::solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps) {
  double step = (end - begin) / n_steps;
  vector<pair<double, double> >& ret = *(new vector<pair<double, double> >());
  double y_curr = y0;
  for (unsigned i = 0; i <= n_steps; i++) {
    ret.push_back(pair<double, double>(begin + i * step, y_curr));
    y_curr += der_func(y_curr) * step;
  };
  return ret;
}

extern "C" __declspec(dllexport)
int cls_EulerSolver::get_id() const {
  return 101;
}

extern "C" __declspec(dllexport)
const char* cls_EulerSolver::get_name() const {
  return "Euler method";
}

extern "C" __declspec(dllexport)
const char* cls_EulerSolver::get_type() const {
  return "ifc_SolverPlugin";
}

extern "C" __declspec(dllexport)
bool cls_EulerSolver::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_SolverPlugin") == 0);
}

extern "C" __declspec(dllexport)
cls_EulerSolver::~cls_EulerSolver() {
  cout << "EulerSolver destructor" << endl;
}
