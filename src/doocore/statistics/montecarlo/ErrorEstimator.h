#ifndef DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H
#define DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H

// from STL
#include <vector>

// from RooFit
#include "RooMultiVarGaussian.h"

// from DooCore
#include "doocore/io/MsgStream.h"
#include "doocore/statistics/general.h"

// forward declarations
//class TMatrixDSym;
class RooArgList;
class RooDataSet;

/**
 * @namespace doocore::statistics::montecarlo
 * @brief DooCore Monte Carlo based statistics
 *
 * This namespace contains all DooCore functionality that does statistics
 * calculations using Monte Carlo based methods.
 *
 * @author Florian Kruse
 */


/** @class doocore::statistics::montecarlo::ErrorEstimator
 *  @brief Monte Carlo-driven abstract error estimator.
 *
 *  Based on a given generator for parameters (e.g. a multivariate Gaussian to 
 *  draw a parameter set) and a calculator (to calculate a value based on the
 *  generated parameter set), this class will get an error estimate for the 
 *  calculator's taget value.
 *
 *  @section sec_interfaces Required interfaces
 *
 *  SampleGenerator::Generate() must be defined as well as 
 *  Calculator::Calculate(inputs) where inputs is the return value of 
 *  SampleGenerator::Generate().
 *
 *  @section sec_example Usage example
 *
 *  For a usage example see TestStatistics.cpp.
 *
 *  @author Florian Kruse
 */
namespace doocore {
namespace statistics {
namespace montecarlo {

template<typename Calculator, typename SampleGenerator>
class ErrorEstimator {
 public:
  /**
   *  @brief Default constructor for ErrorEstimator
   *
   *  Detailed description.
   *
   *  @param parameter description
   */
  ErrorEstimator(Calculator& calculator, SampleGenerator& sample_generator) : calculator_(calculator), sample_generator_(sample_generator) {}
  
  ~ErrorEstimator() {}
  
  /**
   *  @brief Sample value calculation for n events
   *
   *  @param num_samples number of calculations to perform
   *  @return mean value and sqrt(sample variance) as ValueWithError<double>
   */
  doocore::statistics::general::ValueWithError<double> Sample(unsigned int num_samples) {
    for (int i=0; i<num_samples; ++i) {
      DrawSingleValue();
    }
    
    return doocore::statistics::general::ArithmeticMean<double>(generated_values_.begin(), generated_values_.end());
  }
  
 protected:
  /**
   *  @brief Draw one target value
   */
  void DrawSingleValue() {
    generated_values_.push_back(calculator_.Calculate(sample_generator_.Generate()));
  }
  
 private:
  /**
   *  @brief Reference to the Calculator
   */
  Calculator& calculator_;
  
  /**
   *  @brief Reference to the SampleGenerator
   */
  SampleGenerator& sample_generator_;

  /**
   *  @brief Generated target values
   */
  std::vector<double> generated_values_;
};
  
/** @class doocore::statistics::montecarlo::MultiVarGaussianSampleGenerator
 *  @brief Sample generator using a multivariate Gaussian to draw a set of values
 *
 *  Based on a given set of expectation values and a covariance matrix, this 
 *  sample generator draws a sample of parameters according to the multivariate
 *  Gaussian distribution.
 *
 *  @author Florian Kruse
 */
class MultiVarGaussianSampleGenerator {
public:
  MultiVarGaussianSampleGenerator(const RooArgList& values_expected, const TMatrixDSym& covariance);
  
  ~MultiVarGaussianSampleGenerator();
  
  const RooArgSet* Generate();
private:
  RooMultiVarGaussian* mvg_;
  
  RooArgSet* values_expected_;
  RooArgList* values_mu_;
  
  /**
   *  @brief Number of generated events so far
   */
  unsigned int num_generated_;
  
  /**
   *  @brief Position in currently generated dataset
   */
  unsigned int pos_dataset_;
  
  /**
   *  @brief Dataset with drawn values
   */
  RooDataSet* dataset_;
};

} // namespace montecarlo
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H
