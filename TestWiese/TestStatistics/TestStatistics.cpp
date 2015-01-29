
// from STL
#include <vector>
#include <utility>

// from Boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope
#include <boost/format.hpp>

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
#include "effic/effic2.hpp"

// from GSL
#include "gsl/gsl_randist.h"
#include "gsl/gsl_rng.h"

// class MySampleGenerator {
//  public:
//   MySampleGenerator() : rand_() {}
  
//   double Generate() {
//     return rand_.Gaus(1, 0.5);
//   }
  
//  private:
//   TRandom3 rand_;
// };

class MyCalculator {
 public:
  double Calculate(const RooArgSet* values) const {
    using namespace doocore::io;
//    sdebug << values->getRealValue("p1") << " - " << values->getRealValue("p2") << endmsg;
    return values->getRealValue("p1") + values->getRealValue("p2");
  }
};


int main() {
  using namespace doocore::io;
  using namespace doocore::statistics::general;
  using namespace doocore::statistics::montecarlo;
  
  swarn << "Starting TestStatistics.cpp" << endmsg;
  swarn << "" << endmsg;
  
  // void effic2(int k, int N, double conflevel, 
  //       double& mode, double& low, double& high);

  double eff, low, high;
  effic2(100, 1000, 0.683, eff, low, high);
  sinfo << "eff = " << 100 << "/" << 1000 << " = " << eff << " - " << eff-low << " + " << high-eff << endmsg;

  ValueWithError<double> test_num(0.99,0.109);
  sinfo << "My parameter is " << test_num << doocore::io::endmsg;

  sinfo << boost::format("%.2f") % 0.109 << endmsg;
  
  RooRealVar p1("p1", "p1", 10.0, -100.0, 100.0);
  RooRealVar p2("p2", "p2", 10.0, -100.0, 100.0);
  RooArgList args(p1, p2);
  
  p1.setError(1.0);
  p2.setError(1.0);

  TMatrixDSym cov(2);
  
  cov(0,0) = 0.25;
  cov(0,1) = cov(1,0) = 0.0;
  cov(1,1) = 0.25;
  
  MultiVarGaussianSampleGenerator mvggen(args, cov);
  VaryParameterErrorsGenerator varygen(args);

  MyCalculator mycalc;
  //MySampleGenerator mygen;
  ErrorEstimator<MyCalculator, VaryParameterErrorsGenerator> est(mycalc, varygen);
  
  ValueWithError<double> mcval(est.Sample(10000));
  swarn << "Test of ErrorEstimator:" << endmsg;
  sinfo << mcval << " - " << mcval.value << " +/- " << mcval.error << endmsg;
  
  sinfo << "Boundaries: " << est.MinimumGeneratedValue() << " - " << est.MaximumGeneratedValue() << endmsg;
  sinfo << "Minimum parameters: " << *varygen.MinimumParameterSet() << endmsg;
  sinfo << "Maximum parameters: " << *varygen.MaximumParameterSet() << endmsg;

  std::vector<ValueWithError<double>> values;
  
  values  += 
  ValueWithError<double>(1.2, 0.034, 0.2),
  ValueWithError<double>(2.6, 0.12, 1.3),
  ValueWithError<double>(4.9, 0.94, 0.9),
  ValueWithError<double>(7.2, 1.45, 1.2),
  ValueWithError<double>(10.3, 0.87, 1.4);
  swarn << "" << endmsg;
  
  swarn << "Test of std::vector<ValueWithError<double>> printout:" << endmsg;
  sinfo << values << endmsg;
  swarn << "" << endmsg;

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

  sinfo << ValueWithError<double>(35400000, 354000, 354000, 356000) << endmsg;
  sinfo << ValueWithError<double>(3540000, 35400, 35400, 35600) << endmsg;
  sinfo << ValueWithError<double>(354000, 3540, 3540, 3560) << endmsg;
  sinfo << ValueWithError<double>(35400, 354, 354, 356) << endmsg;
  sinfo << ValueWithError<double>(3540, 35.4, 35.4, 35.6) << endmsg;
  sinfo << ValueWithError<double>(35.4, 3.54, 3.54, 3.56) << endmsg;
  sinfo << ValueWithError<double>(3.54, 0.354, 0.354, 0.356) << endmsg;
  sinfo << ValueWithError<double>(3.4, 0.0354, 0.0354, 0.0356) << endmsg;
  sinfo << ValueWithError<double>(0.354, 0.00354, 0.00354, 0.00356) << endmsg;
  sinfo << ValueWithError<double>(0.0354, 0.000354, 0.000354, 0.000356) << endmsg;
  sinfo << ValueWithError<double>(0.00354, 0.0000354, 0.0000354, 0.0000356) << endmsg;
  sinfo << ValueWithError<double>(0.000354, 0.00000354, 0.00000354, 0.00000356) << endmsg;

  sinfo << ValueWithError<double>(35400000, 354000, 356000, 356000) << endmsg;
  sinfo << ValueWithError<double>(3540000, 35400, 35600, 35600) << endmsg;
  sinfo << ValueWithError<double>(354000, 3540, 3560, 3560) << endmsg;
  sinfo << ValueWithError<double>(35400, 354, 356, 356) << endmsg;
  sinfo << ValueWithError<double>(3540, 35.4, 35.6, 35.6) << endmsg;
  sinfo << ValueWithError<double>(35.4, 3.54, 3.56, 3.56) << endmsg;
  sinfo << ValueWithError<double>(3.54, 0.354, 0.356, 0.356) << endmsg;
  sinfo << ValueWithError<double>(3.4, 0.0354, 0.0356, 0.0356) << endmsg;
  sinfo << ValueWithError<double>(0.354, 0.00354, 0.00356, 0.00356) << endmsg;
  sinfo << ValueWithError<double>(0.0354, 0.000354, 0.000356, 0.000356) << endmsg;
  sinfo << ValueWithError<double>(0.00354, 0.0000354, 0.0000356, 0.0000356) << endmsg;
  sinfo << ValueWithError<double>(0.000354, 0.00000354, 0.00000356, 0.00000356) << endmsg;
  swarn << "" << endmsg;

  swarn << "Test of printout with and without usage of auto-precision:" << endmsg;
  ValueWithError<double> num(122.572427568, 122.572427568);
  sinfo << num << endmsg;
  num.set_full_precision_printout(true);
  sinfo << num << endmsg;
  
  swarn << "Test of printout with and without usage of auto-precision with asymmetric errors:" << endmsg;
  ValueWithError<double> num_asym(3.928191, 0.3472, 0.3627, 0.3231);
  sinfo << num_asym << endmsg;
  num_asym.set_full_precision_printout(true);
  sinfo << num_asym << endmsg;

  
  swarn << "Test of WeightedAverage:" << endmsg;
  auto mean_error = doocore::statistics::general::WeightedAverage<double>(values.begin(), values.end());
  sinfo << mean_error << endmsg;
  // sdebug << mean_error.value << " +/- " << mean_error.error << endmsg;
  std::vector<double> x_wavg;
  std::vector<double> w_wavg_neg;
  std::vector<double> w_wavg_pos;
  x_wavg += 1.2, 2.6, 4.9, 7.2, 10.3;
  w_wavg_neg += -6.3, 2.8, 1.4, -3.7, 10.8;
  w_wavg_pos += 0.2, 1.3, 0.9, 1.2, 1.4;
  sinfo << WeightedAverage<double>(x_wavg.begin(), x_wavg.end(), w_wavg_pos.begin()) << endmsg;
  swarn << "" << endmsg;
  
  swarn << "Test of WeightedCovariance:" << endmsg;
  std::vector<double> y_wavg;
  y_wavg += 3.4, 1.9, 6.8, 5.8, 11.4;
  sinfo << Covariance(x_wavg, y_wavg, w_wavg_pos) << endmsg;
  swarn << "" << endmsg;

  swarn << "Test of PearsonCorrelation (incl. permutation test and bootstrap test):" << endmsg;
  std::vector<double> x_full;
  std::vector<double> y_full;
  x_full += 1, 2, 3, 4, 5, 6, 7, 8, 9;
  y_full += 2, 4, 6, 8, 10, 12, 14, 16, 18;
  sinfo << "Full correlation (1): " << PearsonCorrelation(x_full, y_full) << endmsg;
  sinfo << "Permutation Test: p = " << PermutationTest(x_full, y_full) << endmsg;
  std::pair<double, double> xy_full_corr_conv = BootstrapTest(x_full, y_full);
  sinfo << "Bootstrap Test: LOW: " << xy_full_corr_conv.first << ", HIGH: " << xy_full_corr_conv.second << endmsg;
  sinfo << "" << endmsg;

  std::vector<double> x_full_anti;
  std::vector<double> y_full_anti;
  x_full_anti += 1, 2, 3, 4, 5, 6, 7, 8, 9;
  y_full_anti += 9, 8, 7, 6, 5, 4, 3, 2, 1;
  sinfo << "Full anti-correlation (-1): " << PearsonCorrelation(x_full_anti, y_full_anti) << endmsg;
  sinfo << "Permutation Test: p = " << PermutationTest(x_full_anti, y_full_anti) << endmsg;
  std::pair<double, double> xy_full_anti_corr_conv = BootstrapTest(x_full_anti, y_full_anti);
  sinfo << "Bootstrap Test: LOW: " << xy_full_anti_corr_conv.first << ", HIGH: " << xy_full_anti_corr_conv.second << endmsg;
  sinfo << "" << endmsg;

  std::vector<double> x_rdm;
  std::vector<double> y_rdm;
  std::vector<double> w_rdm;
  x_rdm += 1.3, 8, 9.2, 1.2, 8.8, 1.9, 4.3, 3.4;
  y_rdm += 9.4, 8.8, 1.2, 0.2, 9.4, 8.4, 10.3, 7.2;
  // w_rdm += 0.1, 0.2, 0.3, 0.4, 0.9, 0.8, 0.7, 0.6;
  w_rdm += 1, 1, 1, 1, 1, 1, 1, 1;

  // std::generate(x_rdm.begin(), x_rdm.end(), std::default_random_engine(1));
  // std::generate(y_rdm.begin(), y_rdm.end(), std::default_random_engine(10000));
  sinfo << "No correlation: " << PearsonCorrelation(x_rdm, y_rdm) << endmsg;
  sinfo << "Permutation Test: p = " << PermutationTest(x_rdm, y_rdm) << endmsg;
  std::pair<double, double> xy_rdm_corr_conv = BootstrapTest(x_rdm, y_rdm);
  sinfo << "Bootstrap Test: LOW: " << xy_rdm_corr_conv.first << " , HIGH:" << xy_rdm_corr_conv.second << endmsg;
  sinfo << "" << endmsg;

  std::vector<double> x_rdm_wc;
  std::vector<double> y_rdm_wc;
  // std::vector<double> w_rdm_wc(500, 1);

  gsl_rng * r = gsl_rng_alloc (gsl_rng_taus);
  double a;
  double b;
  for (int i = 0; i < 500; i++){
    gsl_ran_bivariate_gaussian(r, 0.5, 0.5, 0.1, &a, &b);
    x_rdm_wc.push_back(a);
    y_rdm_wc.push_back(b);  
  }

  sinfo << "rho = 0.1: " << PearsonCorrelation(x_rdm_wc, y_rdm_wc) << endmsg;
  sinfo << "Permutation Test: p = " << PermutationTest(x_rdm_wc, y_rdm_wc) << endmsg;
  std::pair<double, double> xy_rdm_wc_corr_conv = BootstrapTest(x_rdm_wc, y_rdm_wc);
  sinfo << "Bootstrap Test: LOW: " << xy_rdm_wc_corr_conv.first << " , HIGH:" << xy_rdm_wc_corr_conv.second << endmsg;
  sinfo << "" << endmsg;

  swarn << "Test of weighted PearsonCorrelation (incl. bootstrap test):" << endmsg;
  sinfo << PearsonCorrelation(x_rdm, y_rdm, w_rdm) << endmsg;
  sinfo << BootstrapTest(x_rdm, y_rdm, w_rdm) << endmsg;

  // sinfo << PearsonCorrelation(x_rdm_wc, y_rdm_wc, w_rdm_wc) << endmsg;
  // sinfo << BootstrapTest(x_rdm_wc, y_rdm_wc, w_rdm_wc) << endmsg;

  swarn << "Correlation between 'Divorce rate in Maine' and 'Per capita consuption of margarine (US)':" << endmsg;
  std::vector<double> divorce_rate_in_maine;
  divorce_rate_in_maine += 5.0, 4.7, 4.6, 4.4, 4.3, 4.1, 4.2, 4.2, 4.2, 4.1;
  std::vector<double> margarine_consuption;
  margarine_consuption += 8.2, 7.0, 6.5, 5.3, 5.2, 4.0, 4.6, 4.5, 4.2, 3.7;
  sinfo << "rho = " << PearsonCorrelation(divorce_rate_in_maine, margarine_consuption) << endmsg;
  sinfo << "p = " << PermutationTest(divorce_rate_in_maine, margarine_consuption) << endmsg;
  sinfo << "95% CL: " << BootstrapTest(divorce_rate_in_maine, margarine_consuption) << endmsg;
  sinfo << "" << endmsg;

  swarn << "Correlation between 'Number of people who drowned by falling into a swimming-pool' and 'Number of films Nicolas Cage appeared in':" << endmsg;
  std::vector<double> drowned_in_swimming_pool;
  drowned_in_swimming_pool += 109, 102, 102, 98, 85, 95, 96, 98, 123, 94, 102;
  std::vector<double> number_of_films_w_nicolas_cage;
  number_of_films_w_nicolas_cage += 2, 2, 2, 3, 1, 1, 2, 3, 4, 1, 4;
  sinfo << "rho = " << PearsonCorrelation(drowned_in_swimming_pool, number_of_films_w_nicolas_cage) << endmsg;
  sinfo << "p = " << PermutationTest(drowned_in_swimming_pool, number_of_films_w_nicolas_cage) << endmsg;
  sinfo << "95% CL: " << BootstrapTest(drowned_in_swimming_pool, number_of_films_w_nicolas_cage) << endmsg;
  sinfo << "" << endmsg;

  swarn << "Correlation between 'Honey producing bee colonies (US)' and 'Juveline arrests for possession of marijuana':" << endmsg;
  std::vector<double> honey_bees;
  honey_bees += 3.220, 3.211, 3.045, 2.875, 2.783, 2.655, 2.581, 2.631, 2.637, 2.652, 2.622, 2.550, 2.574, 2.599, 2.554, 2.409, 2.394, 2.443, 2.342, 2.498;
  std::vector<double> marijuana_arrests;
  marijuana_arrests += 20.940, 16.490, 25.004, 37.915, 61.003, 82.015, 87.712, 94.046, 91.467, 89.523, 95.962, 97.088, 85.769, 87.909, 87.717, 88.909, 95.120, 97.671, 93.042, 90.927;
  sinfo << "rho = " << PearsonCorrelation(honey_bees, marijuana_arrests) << endmsg;
  sinfo << "p = " << PermutationTest(honey_bees, marijuana_arrests) << endmsg;
  sinfo << "95% CL: " << BootstrapTest(honey_bees, marijuana_arrests) << endmsg;
  sinfo << "" << endmsg;

  swarn << "Correlation between 'Sunlight in Arkansas' and 'Female Editors on Harvard Law Review':" << endmsg;
  std::vector<double> sunlight_in_arkansas;
  sunlight_in_arkansas += 17243.83, 17327.61, 16681.82, 17031.89, 16475.66;
  std::vector<double> female_editors_harvard_law_review;
  female_editors_harvard_law_review += 9, 14, 19, 12, 19;
  sinfo << "rho = " << PearsonCorrelation(sunlight_in_arkansas, female_editors_harvard_law_review) << endmsg;
  sinfo << "p = " << PermutationTest(sunlight_in_arkansas, female_editors_harvard_law_review) << endmsg;
  sinfo << "95% CL: " << BootstrapTest(sunlight_in_arkansas, female_editors_harvard_law_review) << endmsg;
  sinfo << "" << endmsg;
}
