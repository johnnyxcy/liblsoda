#ifndef lsoda_common_h
#define lsoda_common_h

#define __lsoda_common_max(a, b) ((a) > (b) ? (a) : (b))
#define __lsoda_common_min(a, b) ((a) < (b) ? (a) : (b))

#define __lsoda_common_ETA 2.2204460492503131e-16
#define __lsoda_common_SQRTETA 1.4901161193847656e-08
#define __lsoda_common_CCMAX 0.3
#define __lsoda_common_MAXCOR 3
#define __lsoda_common_MSBP 20
#define __lsoda_common_MXNCF 10
#define __lsoda_common_RATIO 5.0

extern double sm1[13];
/* newly added static variables */

struct lsoda_common_t {
  double **yh, **wm, *ewt, *savf, *acor;
  int *ipvt;
  void *memory;

  /* static variables for lsoda() */

  double h, hu, rc, tn;
  double tsw, pdnorm;

  /* no static variable for prja(), solsy() */
  /* static variables for stoda() */

  double crate, el[14];
#ifdef CFODE_STATIC
  double (*elco)[14], (*tesco)[4];
#else
  double elco[13][14], tesco[13][4];
#endif
  double hold, rmax;
  double pdest, pdlast;

  /* static variables for various vectors and the Jacobian. */

  int ialth, ipup, nslp;
  int icount, irflag;
  int imxer;
  int illin, nhnil, nslast, jcur, meth, mused, nq, nst, ncf, nfe, nje, nqu,
      miter;
};
#define _C(x) (ctx->common->x)

#endif /* lsoda_common_h */