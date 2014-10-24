#ifndef DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H
#define DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H

// from STL
#include <vector>

// from ROOT
#include "TRandom3.h"

// from RooFit
#include "RooMultiVarGaussian.h"

// from DooCore
#include "doocore/io/MsgStream.h"
#include "doocore/io/Progress.h"
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
 *  SampleGenerator::Generate(). Additionally SampleGenerator needs the 
 *  following functions: SampleGenerator::SaveMinimumParameterSet() and
 *  SampleGenerator::SaveMaximumParameterSet() to save the generated parameter 
 *  sets that result in the largest variation of the target value.
 *
 *  @section sec_example Usage example
 *
 *  For a usage example see TestStatistics.cpp.
 *
 *  @section sec_samplesize Sample size guideline
 *
 *  The number of required samples is depending on the required precision in the
 *  error estimate. According to Blobel (eBook p. 92) the standard deviation of
 *  the error estimate can be approximated as
 *
 *  @code
 *  sigma(error) = error/sqrt(2*(n-1))
 *  @endcode
 *
 *  This means, to get a relative error precision of error/sigma(error) = 10 the
 *  number of samples would be 49.
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
    doocore::io::Progress p("Sampling distribution for ErrorEstimator", num_samples);
    for (unsigned int i=0; i<num_samples; ++i) {
      DrawSingleValue();
      ++p;
    }
    p.Finish();
    
    return doocore::statistics::general::ArithmeticMean<double>(generated_values_.begin(), generated_values_.end());
  }

  /**
   *  @brief Get minimum generated value
   */
  double MinimumGeneratedValue() const { return min_generated_value_; }

  /**
   *  @brief Get maximum generated value
   */
  double MaximumGeneratedValue() const { return max_generated_value_; }
  
 protected:
  /**
   *  @brief Draw one target value
   */
  void DrawSingleValue() {
    double single_value(calculator_.Calculate(sample_generator_.Generate()));

    if (generated_values_.size() == 0) {
      min_generated_value_ = single_value;
      sample_generator_.SaveMinimumParameterSet();
      max_generated_value_ = single_value;
      sample_generator_.SaveMaximumParameterSet();
    } else {
      if (single_value < min_generated_value_) {
        min_generated_value_ = single_value;
        sample_generator_.SaveMinimumParameterSet();
      }
      if (single_value > max_generated_value_) {
        max_generated_value_ = single_value;
        sample_generator_.SaveMaximumParameterSet();
      }
    }

    generated_values_.push_back(single_value);
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

  /**
   *  @brief Minimum target value
   */
  double min_generated_value_;  

  /**
   *  @brief Minimum target value
   */
  double max_generated_value_;
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

  void SaveMinimumParameterSet();
  void SaveMaximumParameterSet();

  const RooArgSet* MinimumParameterSet() const {
    return min_generated_set_;
  }

  const RooArgSet* MaximumParameterSet() const {
    return max_generated_set_;
  }

private:
  RooMultiVarGaussian* mvg_;
  
  RooArgSet* values_expected_;
  RooArgList* values_mu_;
  
  /**
   *  @brief Last generated set of values
   */
  const RooArgSet* last_generated_set_;

  /**
   *  @brief Set of values for minimum target value
   */
  RooArgSet* min_generated_set_;

  /**
   *  @brief Set of values for minimum target value
   */
  RooArgSet* max_generated_set_;

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

/** @class doocore::statistics::montecarlo::VaryParameterErrorsGenerator
 *  @brief Sample generator varying set of values up and down their given error
 *
 *  Based on a given set of expectation values for a list of parameters incl. 
 *  errors, this sample generator draws a sample of parameters by varying the 
 *  parameters errors up and down randomly.
 *
 *  @author Florian Kruse
 */
class VaryParameterErrorsGenerator {
public:
  VaryParameterErrorsGenerator(const RooArgList& values_expected);
  
  ~VaryParameterErrorsGenerator();
  
  const RooArgSet* Generate();

  void SaveMinimumParameterSet();
  void SaveMaximumParameterSet();

  const RooArgSet* MinimumParameterSet() const {
    return min_generated_set_;
  }

  const RooArgSet* MaximumParameterSet() const {
    return max_generated_set_;
  }

private:
  RooArgList* values_;

  /**
   *  @brief Last generated set of values
   */
  RooArgSet* last_generated_set_;

  /**
   *  @brief Set of values for minimum target value
   */
  RooArgSet* min_generated_set_;

  /**
   *  @brief Set of values for minimum target value
   */
  RooArgSet* max_generated_set_;

  TRandom3 random_;
};

} // namespace montecarlo
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_MONTECARLO_ERRORESTIMATOR_H
