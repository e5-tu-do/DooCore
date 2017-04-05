///////////////////////////////////////////////////////////////////////////////
//
// Implementation file for the numerical equation solver library.
// This includes root finding and minimum finding algorithms.
// Adapted from Numerical Recipes in C, 2nd edition.
// Translated to C++ by Marc Paterno
//
#include <algorithm>
#include <cmath>
#include <sstream>
#include <stdexcept>

#include "effic/solvers.hpp"

template <class T> T sign(T a, T b) { return b>0.0 ? std::abs(a) : -std::abs(a);}

template <class T> void shift(T& a, T& b, T& c, const T& d)
{
  a = b;
  b = c;
  c = d;
}

double brent(double ax, double bx, double cx, 
	     ONEARGFUNC f, double tol, 
	     double *xmin) {
  const int ITMAX = 100;
  const double CGOLD =  0.3819660;
  const double ZEPS = 1.0e-10;

  int iter;
  double a,b,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;
  double d=0.0;
  double e=0.0;

  a=(ax < cx ? ax : cx);
  b=(ax > cx ? ax : cx);
  x=w=v=bx;
  fw=fv=fx=(*f)(x);
  for (iter=1;iter<=ITMAX;iter++) {
    xm=0.5*(a+b);
    tol2=2.0*(tol1=tol*std::abs(x)+ZEPS);
    if (std::abs(x-xm) <= (tol2-0.5*(b-a))) {
      *xmin=x;
      return fx;
    }
    if (std::abs(e) > tol1) {
      r=(x-w)*(fx-fv);
      q=(x-v)*(fx-fw);
      p=(x-v)*q-(x-w)*r;
      q=2.0*(q-r);
      if (q > 0.0) p = -p;
      q=std::abs(q);
      etemp=e;
      e=d;
      if (std::abs(p) >= std::abs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
        d=CGOLD*(e=(x >= xm ? a-x : b-x));
      else {
        d=p/q;
        u=x+d;
	      if (u-a < tol2 || b-u < tol2)
	        d=sign(tol1,xm-x);
      }
    } else {
      d=CGOLD*(e=(x >= xm ? a-x : b-x));
    }
    u=(std::abs(d) >= tol1 ? x+d : x+sign(tol1,d));
    fu=(*f)(u);
    if (fu <= fx) {
      if (u >= x) a=x; else b=x;
      shift(v,w,x,u);
      shift(fv,fw,fx,fu);
    } else {
      if (u < x) a=u; else b=u;
      if (fu <= fw || w == x) {
	      v=w;
	      w=u;
	      fv=fw;
	      fw=fu;
      } else if (fu <= fv || v == x || v == w) {
	      v=u;
	      fv=fu;
      }
    }
  }
  std::ostringstream oss;
  oss << "Too many iterations in brent" << std::endl;
  throw std::runtime_error(oss.str());
//   *xmin=x;
//   return fx;
}

