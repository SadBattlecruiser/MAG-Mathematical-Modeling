#include <vector>
#include <utility>

using namespace std;

// Принимает функцию производной
extern "C" vector<pair<double, double>>& solver(double (*der_func)(double), double y0, double begin, double end, unsigned n_steps) {
  double step = (end - begin) / n_steps;
  vector<pair<double, double>>& ret = *(new vector<pair<double, double>>());

  double y_curr = y0;
  for (unsigned i = 0; i <= n_steps; i++) {
    ret.push_back(pair<double, double>(begin + i * step, y_curr));
    y_curr += der_func(y_curr) * step;
  };

  return ret;
}
