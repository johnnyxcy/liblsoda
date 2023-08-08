#ifndef benchmark_pk_hh
#define benchmark_pk_hh

#include <iostream>
#include <memory>
#include <vector>

#include "lsoda/lsoda.h"
#include "runner.hh"

namespace pk_case {

/** lsoda compat */
int pk(double t, double *y, double *ydot, void *data) {
  double cl = 0.134;
  double v = 7.81;
  double ka = 0.571;
  double k = cl / v;
  ydot[0] = -ka * y[0];
  ydot[1] = ka * y[0] - k * y[1];
  return (0);
}

/** boost compat */
void pk(const std::vector<double> &y, std::vector<double> &ydot,
        const double t) {
  double cl = 0.134;
  double v = 7.81;
  double ka = 0.571;
  double k = cl / v;
  ydot[0] = -ka * y[0];
  ydot[1] = ka * y[0] - k * y[1];
}

int run_lsoda(benchmark_runner &runner) {
  int neq = 2;
  double atol[2], rtol[2];

  struct lsoda_opt_t opt = {0};
  opt.ixpr = 0;
  opt.rtol = rtol;
  opt.atol = atol;
  opt.itask = 1;

  rtol[0] = 1.0E-4;
  rtol[1] = 1.0E-4;
  atol[0] = 1.0E-6;
  atol[1] = 1.0E-6;

  struct lsoda_context_t ctx = {
      .function = pk,
      .data = NULL,
      .neq = neq,
      .state = 1,
  };

  lsoda_prepare(&ctx, &opt);

  lsoda_runner runner_ = lsoda_runner(runner);
  runner_.case_name = "lsoda::pk";
  runner_.ctx = &ctx;

  runner_.run();

  lsoda_free(&ctx);

  return 0;
}

int run_boost(benchmark_runner &runner) {
  boost_runner runner_ = boost_runner(runner);
  runner_.case_name = "boost::pk";
  runner_.rtol = 1e-4;
  runner_.atol = 1e-6;
  runner_.func = [](const std::vector<double> &y, std::vector<double> &ydot,
                    const double t) { return pk(y, ydot, t); };

  runner_.run();

  return 0;
}

int run() {
  double y0[2];
  y0[0] = 100.0;
  y0[1] = 0.0;

  benchmark_runner runner{};
  runner.dt = 0.001;
  runner.neq = 2;
  runner.print = false;
  runner.t0 = 0.0;
  runner.t1 = 24.0;
  runner.y0 = y0;
  run_lsoda(runner);
  run_boost(runner);

  return 0;
}
}  // namespace pk_case

#endif /* benchmark_fex_hh */