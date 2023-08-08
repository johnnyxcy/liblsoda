#ifndef lsoda_benchmark_runner_hh
#define lsoda_benchmark_runner_hh

#include <boost/numeric/odeint.hpp>
#include <chrono>
#include <functional>
#include <iostream>

#include "lsoda/lsoda.h"

struct benchmark_runner {
 public:
  char* case_name;

  double* y0;
  double t0;
  double t1;
  double dt;
  int neq;

  bool print;

  virtual void integrate(double* y, double t0, double t1){};

  void run() {
    std::cout << "Running Case " << this->case_name << std::endl;
    std::cout << std::setprecision(6);
    std::chrono::steady_clock::time_point begin =
        std::chrono::steady_clock::now();

    double t0_ = this->t0;
    double t1_ = this->t1;
    double y0_[this->neq];
    std::copy(this->y0, this->y0 + this->neq, y0_);
    while (t0_ <= t1_) {
      this->integrate(y0_, t0_, t0_ + this->dt);
      if (this->print) {
        std::cout << " at t= " << t0_ + this->dt << " y= " << y0_[0] << " "
                  << y0_[1] << " " << y0_[2] << std::endl;
      }
      t0_ += this->dt;
    }

    std::chrono::steady_clock::time_point end =
        std::chrono::steady_clock::now();
    double dt =
        std::chrono::duration_cast<std::chrono::microseconds>(end - begin)
            .count();
    std::cout << "Time taken (us) = " << dt << std::endl;
  }
};

struct lsoda_runner : public benchmark_runner {
 public:
  lsoda_runner(benchmark_runner& runner) {
    this->case_name = runner.case_name;
    this->dt = runner.dt;
    this->neq = runner.neq;
    this->print = runner.print;
    this->t0 = runner.t0;
    this->t1 = runner.t1;
    this->y0 = runner.y0;
  }

  void integrate(double* y, double t0, double t1) {
    lsoda(this->ctx, y, &t0, t1);
  }

  struct lsoda_context_t* ctx;
};

struct boost_runner : public benchmark_runner {
 public:
  typedef std::function<void(const std::vector<double>& y,
                             std::vector<double>& ydot, const double t)>
      function_type;

  boost_runner(benchmark_runner& runner) {
    this->case_name = runner.case_name;
    this->dt = runner.dt;
    this->neq = runner.neq;
    this->print = runner.print;
    this->t0 = runner.t0;
    this->t1 = runner.t1;
    this->y0 = runner.y0;
  }

  void integrate(double* y, double t0, double t1) {
    typedef boost::numeric::odeint::runge_kutta_dopri5<std::vector<double>>
        stepper_type;

    double atol = 1e-6;
    double rtol = 1e-6;

    std::vector<double> y_(y, y + this->neq);

    integrate_const(make_controlled(atol, rtol, stepper_type()), this->func, y_,
                    t0, t1, t1 - t0);

    std::copy(y_.begin(), y_.end(), y);
  }

  function_type func;
};

#endif /* lsoda_benchmark_runner_hh */