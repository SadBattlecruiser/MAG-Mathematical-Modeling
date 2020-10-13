#include <vector>
#include <utility>
#include <cstring>
#include <iostream>
#include "solver_plugin.hpp"

using namespace std;

class cls_EulerSolver : public ifc_SolverPlugin {
public:
  virtual int get_id() const;
  virtual const char* get_name() const;
  virtual const char* get_type() const;
  const char* get_title() const;
  virtual bool is_instance(const char* req_name) const;
  // Функция интегрирования диффура
  virtual vector<pair<double, double> >& solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps);
  virtual ~cls_EulerSolver();
};

extern "C" __declspec(dllexport)
ifc_BasePlugin* register_plugin() {
  cout << "cls_EulerSolver register_plugin()" << endl;
  return new cls_EulerSolver;
};

// Принимает функцию производной
vector<pair<double, double> >& cls_EulerSolver::solve(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps) {
  cout << "IN SOLVE" << endl;
  double step = (end - begin) / n_steps;
  vector<pair<double, double> >& ret = *(new vector<pair<double, double> >());
  double y_curr = y0;
  for (unsigned i = 0; i <= n_steps; i++) {
    ret.push_back(pair<double, double>(begin + i * step, y_curr));
    y_curr += der_func(y_curr) * step;
  };
  return ret;
}


int cls_EulerSolver::get_id() const {
  return 201;
}


const char* cls_EulerSolver::get_name() const {
  return "cls_EulerSolver";
}


const char* cls_EulerSolver::get_type() const {
  return "ifc_SolverPlugin";
}


const char* cls_EulerSolver::get_title() const {
  return "Euler method";
}


bool cls_EulerSolver::is_instance(const char* req_name) const {
  return (strcmp(req_name, "ifc_SolverPlugin") == 0);
}


cls_EulerSolver::~cls_EulerSolver() {
  cout << "EulerSolver destructor" << endl;
}
