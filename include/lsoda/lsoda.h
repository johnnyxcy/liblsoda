#ifndef lsoda_lsoda_h
#define lsoda_lsoda_h

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
/* ************************************
 *
 */
struct lsoda_opt_t {
  int ixpr;
  int mxstep;
  int mxhnil;
  int mxordn;
  int mxords;
  double tcrit;
  double h0;
  double hmax;
  double hmin;
  double hmxi;

  /**
   * @brief 1 for normal computation of output values of y at t = TOUT
   */
  int itask;

  /**
   * @brief relative tolerance parameter
   */
  double *rtol;

  /**
   * @brief absolute tolerance parameter
   */
  double *atol;
};

typedef int (*_lsoda_f)(double t, double *y, double *dydt, void *data);

struct lsoda_context_t {
  _lsoda_f function;
  void *data;

  /**
   * @brief number of first order ODEs.
   */
  int neq;

  /**
   * @brief integer flag (input and output).  Set state = 1.
   * state = 2  if DLSODA was successful, negative otherwise.
   *         -1 means excess work done on this call (perhaps wrong JT).
   *         -2 means excess accuracy requested (tolerances too small).
   *         -3 means illegal input detected (see printed message).
   *         -4 means repeated error test failures (check all inputs).
   *         -5 means repeated convergence failures (perhaps bad Jacobian
   *            supplied or wrong choice of JT or tolerances).
   *         -6 means error weight became zero during problem. (Solution
   *            component i vanished, and ATOL or ATOL(i) = 0.)
   *         -7 means work space insufficient to finish (see messages).
   */
  int state;
  char *error;
  /* private for lsoda */
  struct lsoda_common_t *common;
  struct lsoda_opt_t *opt;
};

int lsoda_prepare(struct lsoda_context_t *ctx, struct lsoda_opt_t *opt);
void lsoda_reset(struct lsoda_context_t *ctx);
int lsoda(struct lsoda_context_t *ctx, double *y, double *t, double tout);
void lsoda_free(struct lsoda_context_t *ctx);
void lsoda_free_opt(struct lsoda_opt_t *opt);

struct lsoda_context_t *lsoda_create_ctx();
struct lsoda_opt_t *lsoda_create_opt();

#ifdef __cplusplus
}
#endif

#endif /* lsoda_lsoda_h */
