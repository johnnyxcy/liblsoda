#ifndef lsoda_benchmark_runner_h
#define lsoda_benchmark_runner_h

struct BenchmarkRunner {
  char* case_name;
  char* case_description;

  char* runner_name;
  char* runner_description;

  int (*run_serial)(void);
};

#endif /* lsoda_benchmark_runner_h */