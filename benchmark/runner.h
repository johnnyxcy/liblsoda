#ifndef lsoda_benchmark_runner_h
#define lsoda_benchmark_runner_h

#include "lsoda/lsoda.h"

struct BenchmarkRunner {
  char* case_name;
  char* case_description;

  char* runner_name;
  char* runner_description;

  int (*run_serial)(struct lsoda_context_t* ctx, double* y_expected, double t,
                    double tout);
};

#endif /* lsoda_benchmark_runner_h */