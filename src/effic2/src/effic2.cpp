//////////////////////////////////////////////////////////////////////
//
//
// $Id: effic2.cpp,v 1.1 2005/03/02 14:17:46 paterno Exp $
//
// effic2 implemennts a Bayesian efficiency calculation. Given the
// inputs k (number of sucesses), n (number of trials) and conflevel
// (the required confidence level, which must be between 0 and 1) it
// returns the mode of the posterior density for the efficiency (the
// most probably value) and the shortest confidence interval
// containing the require probability content (indicated by the input
// conflevel).
//
// Created 14 Feb 1996        Marc Paterno
// Revised 20 May 2004        Marc Paterno
//////////////////////////////////////////////////////////////////////

#include <cmath>
#include <cstdlib>
#include <stdexcept>

#include <iostream>

#include "effic/effic2.hpp"
#include "effic/specfunc.hpp"
#include "effic/solvers.hpp"

#include "gsl/gsl_errno.h"

using namespace std;

namespace {
  unsigned long GLOBAL_k; // used to pass k[i] into equations
  unsigned long GLOBAL_N; // used to pass N[i] into equations
  double CONFLEVEL;        // confidence level for the interval
}


//////////////////////////////////////////////////////////////////////
//
// Helper functions


// beta_ab(a,b,k,N) calculates the fraction of the area under the
// curve x^k*(1-x)^(N-k) between x=a and x=b

double beta_ab(double a, double b, int k, int N) {
  if (a == b) return 0;    // don't bother integrating over zero range
  int c1 = k+1;
  int c2 = N-k+1;
  return normalized_beta_inc(c1,c2,b)-normalized_beta_inc(c1,c2,a);
}

// searchupper(low,k,N,c) integrates the binomial distribution with
// parameters k,N, and determines what is the upper edge of the
// integration region which starts at low which contains probability
// content c. If an upper limit is found, the value is returned. If no
// solution is found, -1 is returned.

double searchupper(double low, int k, int N, double c) {
  // check to see if there is any solution by verifying that the integral up
  // to the maximum upper limit (1) is greater than c
  double integral = beta_ab(low, 1.0, k, N);
  if (integral == c) return 1.0;		// lucky -- this is the solution
  if (integral < c) return -1.0;    // no solution exists
  double too_high = 1.0;             // upper edge estimate
  double too_low = low;
  double test;

  // use a bracket-and-bisect search
  // now we loop 20 times, to end with a root guaranteed accurate to better
  // than 0.1%
  for (int loop=0; loop<20; loop++) {
    test = 0.5*(too_low + too_high);
    integral = beta_ab(low, test, k, N);
    if (integral > c)  too_high = test;
    else too_low = test;
  }
  return test;
}

// searchlower(high,k,N,c) integrates the binomial distribution with
// parameters k,N, and determines what is the lower edge of the
// integration region which ends at high, and which contains
// probability content c. If a lower limit is found, the value is
// returned. If no solution is found, the -1 is returned.

double searchlower(double high, int k, int N, double c) {
  // check to see if there is any solution by verifying that the integral down
  // to the minimum lower limit (0) is greater than c
  double integral = beta_ab(0.0, high, k, N);
  if (integral == c) return 0.0;		// lucky -- this is the solution
  if (integral < c) return -1.0;      // no solution exists
  double too_low = 0.0; // lower edge estimate
  double too_high = high;
  double test;

  // use a bracket-and-bisect search
  // now we loop 20 times, to end with a root guaranteed accurate to better
  // than 0.1%
  for (int loop=0; loop<20; loop++) {
    test = 0.5*(too_high + too_low);
    integral = beta_ab(test, high, k, N);
    if (integral > c)  too_low = test;
    else too_high = test;
  }
  return test;
}

//
// interval(low) return the length of the interval starting at low
// that contains CONFLEVEL of the x^GLOBAL_k*(1-x)^(GLOBAL_N-GLOBAL_k)
// distribution.
// If there is no sufficient interval starting at low, we return 2.0
double interval(double low) {
  double high = searchupper(low, GLOBAL_k, GLOBAL_N, CONFLEVEL);
  if (high == -1.0) return 2.0; //  so that this won't be the shortest interval
  return (high - low);
}

struct EHGuard
{
  explicit EHGuard(gsl_error_handler_t* oh) : old_handler(oh) { }

  ~EHGuard() { gsl_set_error_handler(old_handler); }

  gsl_error_handler_t* old_handler;
};

//////////////////////////////////////////////////////////////////////////////
//
// efficiency calculation routine
//
extern "C"
void effic2(int k, int N, double conflevel, 
	    double& mode, double& low, double& high)
{
  // Turn off the GSL error handling routine---errors are handled by
  // our code.
  EHGuard guard(gsl_set_error_handler_off());

  // Calculate the most probable value for the posterior cross section.
  // This is easy, 'cause it is just k/N
  double efficiency = static_cast<double>(k)/N;
  
  // Calculate the shortest central confidence interval containing the required
  // probability content.

  // interval(low) returns the length of the interval starting at low
  // that contains CONFLEVEL probability. We use Brent's method,
  // except in two special cases: when k=0, or when k=N
  
  double low_edge;
  double high_edge;

  
  if (k == 0)
    {
      low_edge = 0.0;
      high_edge = searchupper(low_edge, k, N, conflevel);
    }
  else if (k == N)
    {
      high_edge = 1.0;
      low_edge = searchlower(high_edge, k, N, conflevel);
    }
  else
    {
      GLOBAL_k = k;
      GLOBAL_N = N;
      CONFLEVEL = conflevel;
      brent(0.0, 0.5, 1.0, interval, 1.0e-9, &low_edge);
      high_edge = low_edge + interval(low_edge);
    }
  
  // output
  mode = efficiency;
  low = low_edge;
  high = high_edge;
}

extern "C"
void effic2_v(unsigned length,
	      int* k, int* N, double conflevel, 
	      double* mode, double* low, double* high)
{
  for (unsigned slot = 0; slot != length; ++slot)
    effic2(k[slot], N[slot], conflevel,
	   mode[slot], low[slot], high[slot]);
}

extern "C"
void effic2_r(int* length,
	      int* k, int* N, double* conflevel, 
	      double* mode, double* low, double* high)
{
  effic2_v(*length, k, N, *conflevel, mode, low, high);
}
