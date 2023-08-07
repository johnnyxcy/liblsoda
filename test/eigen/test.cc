#include <Eigen/Dense>
#include <functional>
#include <iostream>

#include "lsoda/lsoda.h"

struct lsoda_wrapped_eigen_data_t {
  std::function<void(double t, const Eigen::Map<Eigen::VectorXd>& y,
                     Eigen::Map<Eigen::VectorXd>& dydt)>
      f;

  const size_t neq;
};

int _raw_lsoda_wrapped_f(double t, double* y, double* dydt, void* data) {
  auto data_ = (lsoda_wrapped_eigen_data_t*)(data);

  Eigen::Map<Eigen::VectorXd> y_(y, data_->neq);

  Eigen::Map<Eigen::VectorXd> dydt_(dydt, data_->neq);

  data_->f(t, y_, dydt_);

  // for (int i = 0; i < dydt_.size(); i++) {
  //   std::cout << "dydt_[" << i << "] = " << dydt_[i] << std::endl;
  //   std::cout << "dydt[" << i << "] = " << dydt[i] << std::endl;
  // }

  return 0;
}

void test_case(double t, const Eigen::Map<Eigen::VectorXd>& y,
               Eigen::Map<Eigen::VectorXd>& ydot) {
  ydot[0] = 1.0E4 * y[1] * y[2] - .04E0 * y[0];
  ydot[2] = 3.0E7 * y[1] * y[1];
  ydot[1] = -1.0 * (ydot[0] + ydot[2]);
}

int main() {
  const size_t neq = 3;
  double atol[neq], rtol[neq], t, tout;
  int iout;

  t = 0.0E0;
  tout = 0.4E0;

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

  Eigen::VectorXd y0 = Eigen::VectorXd::Zero(neq);
  Eigen::VectorXd dydt = Eigen::VectorXd::Zero(neq);

  struct lsoda_wrapped_eigen_data_t wrapped_dat = {
      .f = &test_case,
      .neq = neq,
  };

  struct lsoda_context_t ctx = {
      .function = _raw_lsoda_wrapped_f,
      .data = &wrapped_dat,
      .neq = neq,
      .state = 1,
  };
  lsoda_prepare(&ctx, &opt);

  Eigen::VectorXd y = Eigen::VectorXd::Zero(neq);
  y(0) = 1;

  for (iout = 1; iout <= 12; iout++) {
    lsoda(&ctx, y.data(), &t, tout);
    printf(" at t= %12.4e y= %14.6e %14.6e %14.6e\n", t, y[0], y[1], y[2]);
    if (ctx.state <= 0) {
      printf("error istate = %d\n", ctx.state);
      exit(0);
    }
    tout = tout * 10.0E0;
  }

  lsoda_free(&ctx);

  return 0;
}