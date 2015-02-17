///////////////////////////////////////////////////////////////////////////////
//
// Implementation file for the special functions library.
// Adapted from Numerical Recipes in C, 2nd edition.
// Translated to C++ by by Marc Paterno
//
//

#include <cmath>
#include <stdexcept>
#include <sstream>

#include "gsl/gsl_sf_gamma.h"
#include "gsl/gsl_errno.h"

using namespace std;

double normalized_beta_inc(double a, double b, double x)
{
  gsl_sf_result result;
  int status = gsl_sf_beta_inc_e(a,b,x,&result);
  
  if ( status ) {
    if ( status == GSL_EUNDRFLW )
      {
	result.val = 0.0;
      }
    else
      {
	   
	std::ostringstream msg;
	msg << "Error from gsl_ef_beta_inc_e, for:"
	  << " a=" << a
	  << " b=" << b
	  << " x=" << x
	  << "\nerror message: " << gsl_strerror(status)
	  << "\nerror code: " << status
          << ends;
	throw std::runtime_error(msg.str());
      }
    
  }
  return result.val;
}
