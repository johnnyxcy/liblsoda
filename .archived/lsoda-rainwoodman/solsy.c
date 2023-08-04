#include "lsoda.h"
#include "lsoda_internal.h"
#include "common.h"
#include <stdio.h>
#include <stdlib.h>
#include "blas.h"

int solsy(struct lsoda_context_t * ctx, double *y)

/*
   This routine manages the solution of the linear system arising from
   a chord iteration.  It is called if _C(miter) != 0.
   If _C(miter) is 2, it calls dgesl to accomplish this.
   If _C(miter) is 5, it calls dgbsl.

   y = the right-hand side vector on input, and the solution vector
       on output.
*/

{
	const int neq = ctx->neq;
	if (_C(miter) != 2) {
		/* miter has to be 2. the miter=5 case is not implemented. */
		abort();
	}
	if (_C(miter) == 2)
		dgesl(_C(wm), neq, _C(ipvt), y, 0);
	return 1;

}

