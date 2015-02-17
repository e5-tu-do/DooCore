#ifndef SOLVERS_INC
#define SOLVERS_INC
///////////////////////////////////////////////////////////////////////////////
//
// Declaration file for the numerical equation solver library.
// Adapted from Numerical Recipes in C, 2nd edition.
// Translated to C++ by Marc Paterno
//

// Brent's superlinear minimization in 1 dimension

typedef double (*ONEARGFUNC)(double);

double brent(double ax, double bx, double cx, 
	     ONEARGFUNC f, double tol, 
	     double *xmin);
#endif
