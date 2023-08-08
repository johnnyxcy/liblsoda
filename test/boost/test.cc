#include <boost/numeric/odeint.hpp>
#include <chrono>
#include <functional>
#include <iostream>

#include "lsoda/lsoda.h"

using namespace std;

// const std::vector<double>&, std::vector<double>&, const double
void fex(const std::vector<double>& y, std::vector<double>& ydot,
         const double t) {
  ydot[0] = 1.0E4 * y[1] * y[2] - .04E0 * y[0];
  ydot[2] = 3.0E7 * y[1] * y[1];
  ydot[1] = -1.0 * (ydot[0] + ydot[2]);
}

int test() {
  typedef boost::numeric::odeint::runge_kutta_cash_karp54<std::vector<double>>
      stepper_type;
  int i;

  double t, tout;
  t = 0.0e0;
  tout = 0.4e0;
  double rtol = 1e-4;
  double atol = 1e-6;

  vector<double> y(3);
  y[0] = 1.0;
  y[1] = 0.0;
  y[2] = 0.0;

  for (i = 0; i < 12; i++) {
    integrate_const(make_controlled(atol, rtol, stepper_type()), fex, y, t,
                    tout, tout - t);
    printf(" at t= %12.4e y= %14.6e %14.6e %14.6e\n", t, y[0], y[1], y[2]);
    t = tout;
    tout = tout * 10e0;
  }

  return 0;
}

int main() {
  chrono::steady_clock::time_point begin = chrono::steady_clock::now();

  int N = 1;
  int i;
  for (i = 0; i < N; i++) {
    test();
  }
  chrono::steady_clock::time_point end = chrono::steady_clock::now();
  double dt = chrono::duration_cast<chrono::microseconds>(end - begin).count();
  cout << "Time taken (us/per loop) = " << dt / N << endl;
  return 0;
}