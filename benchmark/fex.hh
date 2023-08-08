#ifndef benchmark_fex_hh
#define benchmark_fex_hh

#include <iostream>
#include <memory>
#include <vector>

#include "lsoda/lsoda.h"
#include "runner.hh"

namespace fex_case {

/** lsoda compat */
int fex(double t, double *y, double *ydot, void *data) {
  ydot[0] = 1.0E4 * y[1] * y[2] - .04E0 * y[0];
  ydot[2] = 3.0E7 * y[1] * y[1];
  ydot[1] = -1.0 * (ydot[0] + ydot[2]);
  return (0);
}

/** boost compat */
void fex(const std::vector<double> &y, std::vector<double> &ydot,
         const double t) {
  ydot[0] = 1.0E4 * y[1] * y[2] - .04E0 * y[0];
  ydot[2] = 3.0E7 * y[1] * y[1];
  ydot[1] = -1.0 * (ydot[0] + ydot[2]);
}

int run_lsoda(benchmark_runner &runner) {
  int neq = 3;
  double atol[3], rtol[3];

  struct lsoda_opt_t opt = {0};
  opt.ixpr = 0;
  opt.rtol = rtol;
  opt.atol = atol;
  opt.itask = 1;

  rtol[0] = rtol[2] = 1.0E-4;
  rtol[1] = 1.0E-4;
  atol[0] = 1.0E-6;
  atol[1] = 1.0E-10;
  atol[2] = 1.0E-6;

  struct lsoda_context_t ctx = {
      .function = fex,
      .data = NULL,
      .neq = neq,
      .state = 1,
  };

  lsoda_prepare(&ctx, &opt);

  lsoda_runner runner_ = lsoda_runner(runner);
  runner_.case_name = "lsoda::fex";
  runner_.ctx = &ctx;

  runner_.run();

  lsoda_free(&ctx);

  return 0;
}

int run_boost(benchmark_runner &runner) {
  boost_runner runner_ = boost_runner(runner);
  runner_.case_name = "boost::fex";
  runner_.func = [](const std::vector<double> &y, std::vector<double> &ydot,
                    const double t) { return fex(y, ydot, t); };

  runner_.run();

  return 0;
}

int run() {
  double y0[3];
  y0[0] = 1.0;
  y0[1] = 0.0;
  y0[2] = 0.0;

  benchmark_runner runner{};
  runner.dt = 0.1;
  runner.neq = 3;
  runner.print = false;
  runner.t0 = 0.0;
  runner.t1 = 10.0;
  runner.y0 = y0;
  run_lsoda(runner);
  run_boost(runner);

  return 0;
}
}  // namespace fex_case

#endif /* benchmark_fex_hh */