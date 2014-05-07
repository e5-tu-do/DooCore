
// from STL
#include <vector>
#include <utility>

// from Boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

// from ROOT
#include "TRandom3.h"
#include "TMatrixDSym.h"

// from RooFit
#include "RooArgList.h"
#include "RooRealVar.h"

// from DooCore
#include "doocore/statistics/general.h"
#include "doocore/statistics/montecarlo/ErrorEstimator.h"
#include "doocore/io/MsgStream.h"
#include "doocore/lutils/lutils.h"

class MySampleGenerator {
 public:
  MySampleGenerator() : rand_() {}
  
  double Generate() {
    return rand_.Gaus(1, 0.5);
  }
  
 private:
  TRandom3 rand_;
};

class MyCalculator {
 public:
  double Calculate(const RooArgSet* values) const {
    using namespace doocore::io;
//    sdebug << values->getRealValue("p1") << " - " << values->getRealValue("p2") << endmsg;
    return values->getRealValue("p1");// + values->getRealValue("p2");
  }
};


int main() {
  using namespace doocore::io;
  using namespace doocore::statistics::general;
  using namespace doocore::statistics::montecarlo;
  
  swarn << "Starting TestStatistics.cpp" << endmsg;
  
  RooRealVar p1("p1", "p1", 10.0, -100.0, 100.0);
  RooRealVar p2("p2", "p2", 10.0, -100.0, 100.0);
  RooArgList args(p1, p2);
  
  TMatrixDSym cov(2);
  
  cov(0,0) = 0.25;
  cov(0,1) = cov(1,0) = 0.0;
  cov(1,1) = 0.25;
  
  MultiVarGaussianSampleGenerator mvggen(args, cov);
  
  MyCalculator mycalc;
  MySampleGenerator mygen;
  ErrorEstimator<MyCalculator, MultiVarGaussianSampleGenerator> est(mycalc, mvggen);
  
  ValueWithError<double> mcval(est.Sample(10000));
  swarn << "Test of ErrorEstimator:" << endmsg;
  sinfo << mcval << " - " << mcval.value << " +/- " << mcval.error << endmsg;
  
  std::vector<ValueWithError<double>> values;
  
  values  += ValueWithError<double>(1.21, 0.12, 56.8149), ValueWithError<double>(0.30, 0.04, 10.804);
  
  swarn << "Test of std::vector<ValueWithError<double>> printout:" << endmsg;
  sinfo << values << endmsg;

  swarn << "Test of printout for different values and correct usage of auto-precision:" << endmsg;
  sinfo << ValueWithError<double>(0.335646548e-6, 0.335646548e-6) << endmsg;
  sinfo << ValueWithError<double>(122.572427568, 122.572427568) << endmsg;
  sinfo << ValueWithError<double>(3.1415927, 3.1415927) << endmsg;

  sinfo << ValueWithError<double>(35400000, 354000) << endmsg;
  sinfo << ValueWithError<double>(3540000, 35400) << endmsg;
  sinfo << ValueWithError<double>(354000, 3540) << endmsg;
  sinfo << ValueWithError<double>(35400, 354) << endmsg;
  sinfo << ValueWithError<double>(3540, 35.4) << endmsg;
  sinfo << ValueWithError<double>(35.4, 3.54) << endmsg;
  sinfo << ValueWithError<double>(3.54, 0.354) << endmsg;
  sinfo << ValueWithError<double>(3.4, 0.0354) << endmsg;
  sinfo << ValueWithError<double>(0.354, 0.00354) << endmsg;
  sinfo << ValueWithError<double>(0.0354, 0.000354) << endmsg;
  sinfo << ValueWithError<double>(0.00354, 0.0000354) << endmsg;
  sinfo << ValueWithError<double>(0.000354, 0.00000354) << endmsg;

  sinfo << ValueWithError<double>(36000000, 360000) << endmsg;
  sinfo << ValueWithError<double>(3600000, 36000) << endmsg;
  sinfo << ValueWithError<double>(360000, 3600) << endmsg;
  sinfo << ValueWithError<double>(36000, 360) << endmsg;
  sinfo << ValueWithError<double>(3600, 36) << endmsg;
  sinfo << ValueWithError<double>(360, 3.6) << endmsg;
  sinfo << ValueWithError<double>(36, 0.36) << endmsg;
  sinfo << ValueWithError<double>(3.6, 0.036) << endmsg;
  sinfo << ValueWithError<double>(0.36, 0.0036) << endmsg;
  sinfo << ValueWithError<double>(0.036, 0.00036) << endmsg;
  sinfo << ValueWithError<double>(0.0036, 0.000036) << endmsg;
  sinfo << ValueWithError<double>(0.00036, 0.0000036) << endmsg;
  
  sinfo << ValueWithError<double>(100000, 100000) << endmsg;
  sinfo << ValueWithError<double>(10000, 10000) << endmsg;
  sinfo << ValueWithError<double>(1000, 1000) << endmsg;
  sinfo << ValueWithError<double>(100, 100) << endmsg;
  sinfo << ValueWithError<double>(10, 10) << endmsg;
  sinfo << ValueWithError<double>(1.0, 1.0) << endmsg;
  sinfo << ValueWithError<double>(0.10, 0.10) << endmsg;
  sinfo << ValueWithError<double>(0.010, 0.010) << endmsg;
  sinfo << ValueWithError<double>(0.0010, 0.0010) << endmsg;
  sinfo << ValueWithError<double>(0.00010, 0.00010) << endmsg;
  sinfo << ValueWithError<double>(0.000010, 0.000010) << endmsg;
  sinfo << ValueWithError<double>(0.0000010, 0.0000010) << endmsg;
  

  swarn << "Test of WeightedAverage:" << endmsg;
  auto mean_error = doocore::statistics::general::WeightedAverage<double>(values.begin(), values.end());
  sinfo << mean_error << endmsg;
  //sinfo << mean_error.value << " +/- " << mean_error.error << endmsg;
  
  swarn << "Test of PearsonCorrelation:" << endmsg;
  std::vector<double> x_full;
  std::vector<double> y_full;
  x_full += 1, 2, 3, 4, 5, 6, 7, 8, 9;
  y_full += 2, 4, 6, 8, 10, 12, 14, 16, 18;
  sinfo << "Full correlation (1): " << PearsonCorrelation(x_full, y_full) << endmsg;
  std::vector<double> x_full_anti;
  std::vector<double> y_full_anti;
  x_full_anti += 1, 2, 3, 4, 5, 6, 7, 8, 9;
  y_full_anti += 9, 8, 7, 6, 5, 4, 3, 2, 1;
  sinfo << "Full anti-correlation (-1): " << PearsonCorrelation(x_full_anti, y_full_anti) << endmsg;
  std::vector<double> x_rdm;
  std::vector<double> y_rdm;
  x_rdm += 1.3, 8, 9.2, 1.2, 8.8, 1.9, 4.3;
  y_rdm += 9.4, 8.8, 1.2, 0.2, 9.4, 8.4, 10.3;
  sinfo << "No correlation (~0): " << PearsonCorrelation(x_rdm, y_rdm) << endmsg;
}
