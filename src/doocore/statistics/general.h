#ifndef DOOCORE_STATISTICS_GENERAL_H
#define DOOCORE_STATISTICS_GENERAL_H

// from STL
#include <utility>
#include <cmath>
#include <cfenv>
#include <string>
#include <chrono>
#include <random>
#include <iomanip>
#include <iostream>

// from Boost
#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

// from ROOT
#include "TMath.h"
#include "TRandom3.h"
#include "TEfficiency.h"

// from RooFit
#include "RooDataSet.h"

// from DooCore
#include "doocore/io/MsgStream.h"
#include "effic/effic2.hpp"

// from GSL
#include "gsl/gsl_statistics.h"
#include "gsl/gsl_sort.h"

// forward decalarations

/**
 * @namespace doocore::statistics
 * @brief DooCore statistics functionality namespace
 *
 * This namespace contains all DooCore functionality that does statistics
 * calculations.
 *
 * @author Florian Kruse, Christophe Cauet
 */

/**
 * @namespace doocore::statistics::general
 * @brief DooCore general statistics functionality namespace
 *
 * This namespace contains all DooCore functionality that does general 
 * statistics calculations that is not part of a more specialised namespace.
 */

namespace doocore {
namespace statistics {
namespace general {
  
  /**
   *  @class doocore::statistics::general::ValueWithError
   *  @brief Simple value with error compound type (and optional weight)
   *
   *  This class allows to store values and additional information such as
   *  an error, a weight and asymmetric errors. It is intended as a container
   *  to simplify printout and handling in calculations.
   *
   *  Keep in mind that not all classes and functions working with 
   *  ValueWithError will always be able to handle all stored information
   *  correctly. For example, a weighted average of values with asymmetric 
   *  errors is far from trivial. A common best practise is to always store 
   *  a more or less meaningful symmetric error as well when using asymmetric 
   *  errors.
   *
   *  @section Usage
   *
   *  @code
   *  // value with error: 0.22883328 +/- 0.0052244
   *  doocore::statistics::general::ValueWithError<double> parameter(0.22883328,0.0052244);
   *  std::string labeltext("Parameter p = " + parameter.FormatString());
   *  doocore::io::sinfo << "My parameter is " << parameter << doocore::io::endmsg;
   *  @endcode
   *
   */
  template<typename T>
  class ValueWithError {
   public:
    ValueWithError(T val, T err, T err_lo, T err_hi, T wgt) : value(val), error(err), weight(wgt), error_lo(err_lo), error_hi(err_hi), full_precision_printout_(false) {}

    ValueWithError(T val, T err, T err_lo, T err_hi) : ValueWithError(val, err, err_lo, err_hi, 1.0) {}
    
    ValueWithError(T val, T err, T wgt) : ValueWithError(val, err, 0.0, 0.0, wgt) {}

    ValueWithError(T val, T err) : ValueWithError(val, err, 0.0, 0.0, 1.0) {}
    
    /**
     *  @brief Format value +/- error with PDG rounding
     *
     *  Based on the PDG rounding rules, the value and error will be formatted 
     *  as strings.
     *
     *  @return value +/- error as string
     */
    std::string FormatString() const;

    /**
     *  @brief Format value +/- error with PDG rounding in TLatex format
     *
     *  Based on the PDG rounding rules, the value and error will be formatted 
     *  as strings.
     *
     *  @return value +/- error as string in TLatex compatible format
     */
    std::string FormatStringTLatex() const {
      std::string str(FormatString());

      boost::replace_all(str, "+/-", "#pm");
      boost::replace_all(str, "-", "#minus");

      return str;
    }
    
    T value;
    T error;
    T weight;

    /**
     *  @brief Lower asymmetric error
     */
    T error_lo;

    /**
     *  @brief Upper asymmetric error
     */
    T error_hi;


    void set_full_precision_printout(bool full_precision_printout) {
      full_precision_printout_ = full_precision_printout;
    }
    
    const std::string& str_value() const { return str_value_; }
    const std::string& str_error() const { return str_error_; }
    const std::string& str_error_lo() const { return str_error_lo_; }
    const std::string& str_error_hi() const { return str_error_hi_; }
    
   private:
    bool full_precision_printout_;
    
    mutable std::string str_value_;
    mutable std::string str_error_;
    mutable std::string str_error_lo_;
    mutable std::string str_error_hi_;
  };
  
  template<typename T>
  std::string doocore::statistics::general::ValueWithError<T>::FormatString() const {
    std::stringstream output;
    
    if (full_precision_printout_) {
      std::stringstream sstr_value;
      sstr_value << std::setprecision(10) << value;
      str_value_ = sstr_value.str();
      
      std::stringstream sstr_error;
      sstr_error << std::setprecision(10) << error;
      str_error_ = sstr_error.str();
      
      std::stringstream sstr_error_lo;
      sstr_error_lo << std::setprecision(10) << error_lo;
      str_error_lo_ = sstr_error_lo.str();

      std::stringstream sstr_error_hi;
      sstr_error_hi << std::setprecision(10) << error_hi;
      str_error_hi_ = sstr_error_hi.str();
    } else {
      if (error == 0.0 && error_lo == 0.0 && error_hi == 0.0) {
        
        std::stringstream sstr_value;
        sstr_value << value;
        str_value_ = sstr_value.str();
        
        std::stringstream sstr_error;
        sstr_error << error;
        str_error_ = sstr_error.str();
        
        std::stringstream sstr_error_lo;
        sstr_error_lo << error_lo;
        str_error_lo_ = sstr_error_lo.str();
        
        std::stringstream sstr_error_hi;
        sstr_error_hi << error_hi;
        str_error_hi_ = sstr_error_hi.str();
      } else {
        double error_to_check = 0.0;
        if (error_lo == 0.0 && error_hi == 0.0) {
          error_to_check = error;
        } else {
          error_to_check = std::min(error_lo, error_hi);
        }
        
        using namespace std;
        // std::cout << value << " +/- " << error << " + " << error_hi << " - " << error_lo << std::endl;
        // std::cout << "error_to_check = " << error_to_check << std::endl;

        std::fesetround(FE_TONEAREST);
        int mantissa_err   = std::nearbyint(error_to_check*100.0*std::pow(10.0,-static_cast<int>(std::floor(std::log10(error_to_check)))));
        
        // cout << "mantissa_err   = " << mantissa_err << endl;

        // additional digits if mantissa of error <= 3.54
        int add_digits     = 0;
        if (mantissa_err <= 354) add_digits++;
        
        T exp_err_check     = std::log10(error_to_check);
        T exp_err           = std::log10(error);
//        T exp_err_lo        = std::log10(error_lo);
//        T exp_err_hi        = std::log10(error_hi);
        T abs_exp_err_check = std::abs(exp_err_check);
        
        std::string format;
        
        // cout << "exp_err_check   = " << exp_err_check << endl;
        // cout << "exp_err         = " << exp_err << endl;

        // depending on exponent use scientific notation or not
        if (abs_exp_err_check < 5) {
          if (exp_err_check < 1.0) {
            std::fesetround(FE_DOWNWARD);
            format = "%." + std::to_string(static_cast<int>(std::abs(std::nearbyint(exp_err_check))+add_digits)) + "f";
          } else {
            format = "%.0f";
          }

          // cout << format << endl;

          std::fesetround(FE_TONEAREST);
          str_value_ = str(boost::format(format) % value);
          str_error_ = str(boost::format(format) % error);
          str_error_lo_ = str(boost::format(format) % error_lo);
          str_error_hi_ = str(boost::format(format) % error_hi);
        } else {
          format = "%." + std::to_string(add_digits) + "f";

          // exponent to use for scientific notation
          T exp_new_err      = std::floor(exp_err);
          
          T mantissa_new_err    = error/std::pow(10.0,exp_new_err);
          T mantissa_new_err_lo = error/std::pow(10.0,exp_new_err);
          T mantissa_new_err_hi = error/std::pow(10.0,exp_new_err);
          T mantissa_new_val    = value/std::pow(10.0,exp_new_err);
          
          std::stringstream sstr_value;
          sstr_value << boost::format(format) % mantissa_new_val << "e" << exp_new_err;
          str_value_ = sstr_value.str();

          std::stringstream sstr_error;
          sstr_error << boost::format(format) % mantissa_new_err << "e" << exp_new_err;
          str_error_ = sstr_error.str();

          std::stringstream sstr_error_lo;
          sstr_error_lo << boost::format(format) % mantissa_new_err_lo << "e" << exp_new_err;
          str_error_lo_ = sstr_error_lo.str();

          std::stringstream sstr_error_hi;
          sstr_error_hi << boost::format(format) % mantissa_new_err_hi << "e" << exp_new_err;
          str_error_hi_ = sstr_error_hi.str();
        }
      }
      
    }
    
    if (error_lo == 0.0 && error_hi == 0.0) {
      output << str_value_ << " +/- " << str_error_;
    } else {
      output << str_value_ << " + " << str_error_hi_ << " - " << str_error_lo_;
    }
    return output.str();
  }

  
  /**
   *  @brief Function to output doocore::statistics::general::ValueWithError directly and nicely into MsgStreams
   */
  template<typename T>
  inline doocore::io::MsgStream& operator<<(doocore::io::MsgStream& lhs, const ValueWithError<T>& arg) {
    
    std::string format = "%.0f";
    if (arg.weight != 1.0) {
      lhs.stream() << arg.FormatString() << " (w: " << arg.weight << ")";
    } else {
      lhs.stream() << arg.FormatString();
    }
    return lhs;
  }

  /**
   *  @brief Calculate binomial errors of an efficiency
   *
   *  Assuming an efficiency num_subset/num_all where the number num_subset is a
   *  subset of num_all, this function will calculate the binomial error of the 
   *  efficiency.
   *
   *  @param num_subset number of events that are passing some criterion
   *  @param num_all number of all events
   *  @return binomial error of num_subset/num_all
   */
  inline double EfficiencyBinomialError(double num_subset, double num_all) {
    return (1.0/num_all)*TMath::Sqrt(num_subset*(1.0-num_subset/num_all));
  }

  /**
   *  @brief Calculate confidence interval of an efficiency according to FERMILAB-TM-2286-CD (Calculating Efficiencies and Their Uncertainties)
   *
   *  Assuming an efficiency num_subset/num_all where the number num_subset is a
   *  subset of num_all, this function will calculate the lower and upper 68.3% 
   *  confidence interval of the efficiency using the Bayesian approach 
   *  discussed in FERMILAB-TM-2286-CD (Calculating Efficiencies and Their 
   *  Uncertainties).
   *
   *  @param num_subset number of events that are passing some criterion
   *  @param num_all number of all events
   *  @return lower and upper confidence interval as std::pair
   */
  inline std::pair<double, double> EfficiencyBayesianErrorBayesian(double num_subset, double num_all) {
    double eff, low, high;
    effic2(num_subset, num_all, 0.683, eff, low, high);
    return std::make_pair(low, high);
  }

  /**
   *  @brief Calculate confidence interval of an efficiency according to Clopper-Pearson
   *
   *  Assuming an efficiency num_subset/num_all where the number num_subset is a
   *  subset of num_all, this function will calculate the binomial error of the 
   *  efficiency.
   *
   *  Assuming an efficiency num_subset/num_all where the number num_subset is a
   *  subset of num_all, this function will calculate the lower and upper 68.3% 
   *  confidence interval of the efficiency using Clopper-Pearson (as 
   *  recommended by the PDG)
   *
   *  @param num_subset number of events that are passing some criterion
   *  @param num_all number of all events
   *  @return lower and upper confidence interval as std::pair
   */
  inline std::pair<double, double> EfficiencyBayesianErrorClopperPearson(double num_subset, double num_all) {
    return std::make_pair(TEfficiency::ClopperPearson(num_all, num_subset, 0.683, false), 
                          TEfficiency::ClopperPearson(num_all, num_subset, 0.683, true));
  }
  
  /**
   *  @brief Calculate weighted average and its error based on values with errors and weights
   *
   *  Based on given iterators of values, errors and weights, the weighted 
   *  average and its error are computed. For the values first and last iterators
   *  are used for the range to calcutate upon. For errors and weights it is 
   *  assumed that the same range is valid.
   *
   *  @param first iterator for values to start with
   *  @param last iterator for values to end with
   *  @param first_weight iterator for weights to start with
   *  @param first_error iterator for errors to start with
   *  @return weighted average and its error as ValueWithError
   */
  template <typename T, typename ValueIterator, typename WeightIterator, typename ErrorIterator>
  inline ValueWithError<T> WeightedAverage(ValueIterator first, ValueIterator last,
                                    WeightIterator first_weight, ErrorIterator first_error) {
    T sum         = 0.0;
    T sum_weights = 0.0;
    T sum_error   = 0.0;
    while (first != last) {
      sum         += (*first_weight) * (*first);
      sum_weights += (*first_weight);
      sum_error   += (*first_weight)*(*first_weight)*(*first_error)*(*first_error);
      ++first_weight;
      ++first;
      ++first_error;
    }
    
    return ValueWithError<T>(sum/sum_weights,TMath::Sqrt(sum_error)/sum_weights);
  }
  
  /**
   *  @brief Calculate weighted average and its error based on values with errors and weights
   *
   *  Based on given iterators of value/error/weight compunds, the weighted
   *  average and its error are computed. The first and last iterators are used 
   *  for the range to calcutate upon. 
   *
   *  Reference for calculation of error:
   *  http://de.wikipedia.org/w/index.php?title=Arithmetisches_Mittel&oldid=131359158
   *  (Sorry, cannot find any better source anymore. Hard to find a general
   *  formula with weight_i != 1/error_i^2)
   *
   *  In the special case of chosing weight_i = 1/error_i^2, the error 
   *  simplifies to
   *
   *  error_mean = 1/sqrt(sum(1/error_i^2)) (which is often quoted in literature)
   *
   *  So in case you want this default behaviour, simply make sure that weights
   *  are defined accordingly.
   *
   *  @param first iterator for values to start with
   *  @param last iterator for values to end with
   *  @return weighted average and its error as ValueWithError
   */
  template <typename T, typename ValueWithErrorIterator>
  inline ValueWithError<T> WeightedAverage(ValueWithErrorIterator first, ValueWithErrorIterator last) {
    T sum         = 0.0;
    T sum_weights = 0.0;
    T sum_error   = 0.0;
    while (first != last) {
//      doocore::io::sdebug << "x = " << (*first).value << ", e = " << (*first).error << ", w = " << (*first).weight << doocore::io::endmsg;
      sum         += (*first).weight * (*first).value;
      sum_weights += (*first).weight;
      sum_error   += (*first).weight*(*first).weight*(*first).error*(*first).error;
      ++first;
    }
    
    return ValueWithError<T>(sum/sum_weights,TMath::Sqrt(sum_error)/sum_weights);
  }

  /**
   *  @brief Calculate weighted average and its error from sample variance based on values with and weights
   *
   *  Based on given iterators of value/weight compunds, the weighted
   *  average and its error are computed. The first and last iterators are used 
   *  for the range to calcutate upon. 
   *
   *  Errors for each ValueWithError are ignored. Instead, the error for the 
   *  computed average is derived from the weighted sample variance as
   *
   *  error = sqrt(sample_variance)/sqrt(sum(w_i))
   *
   *  where sample_variance is:
   *
   *  sample_variance = sum(w_i*(x_i - <x>)^2)/sum(w_i)
   *
   *  This error is an estimator for the error of the average if the individual
   *  values follow a Gaussian distribution with the width sample_variance.
   *
   *  @param first iterator for values to start with
   *  @param last iterator for values to end with
   *  @return weighted average and its error as ValueWithError
   */
  template <typename T, typename ValueWithErrorIterator>
  inline ValueWithError<T> WeightedAverageWithSampleVariance(ValueWithErrorIterator first, ValueWithErrorIterator last) {
    T sum         = 0.0;
    T sum_weights = 0.0;
    T sum_error   = 0.0;
    ValueWithErrorIterator start = first;
    while (first != last) {
//      doocore::io::sdebug << "x = " << (*first).value << ", e = " << (*first).error << ", w = " << (*first).weight << doocore::io::endmsg;
      sum         += (*first).weight * (*first).value;
      sum_weights += (*first).weight;
      ++first;
    }
    first = start;
    sum /= sum_weights;
    while (first != last) {
      sum_error   += (*first).weight * std::pow((*first).value-sum, 2);
      ++first;
    }
    
    return ValueWithError<T>(sum,TMath::Sqrt(sum_error)/sum_weights);
  }
  
  /**
   *  @brief Calculate weighted average based on values and weights
   *
   *  Based on given iterators of values and weights, the weighted 
   *  average is computed. For the values first and last iterators
   *  are used for the range to calcutate upon. For weights it is 
   *  assumed that the same range is valid.
   *
   *  @param first iterator for values to start with
   *  @param last iterator for values to end with
   *  @param first_weight iterator for weights to start with
   *  @return weighted average as T
   */
  template <typename T, typename ValueIterator, typename WeightIterator>
  inline T WeightedAverage(ValueIterator first, ValueIterator last,
                                    WeightIterator first_weight) {
    T sum         = 0.0;
    T sum_weights = 0.0;
    while (first != last) {
      sum         += (*first_weight) * (*first);
      sum_weights += (*first_weight);
      ++first_weight;
      ++first;
    }
    
    return sum/sum_weights;
  }

  /**
   *  @brief Calculate arithmetic mean based on values
   *
   *  Based on given iterators of values, the arithmetic mean and the 
   *  sqrt(sample variance) are computed.
   *
   *  @param parameter description
   *  @param first iterator for values to start with
   *  @param last iterator for values to end with
   *  @return arithmetic mean and sqrt(sample variance) as ValueWithError
   */
  template <typename T, typename ValueIterator>
  inline ValueWithError<T> ArithmeticMean(ValueIterator first, ValueIterator last) {
    T x_e          = *first;
    T sum          = 0.0;
    T sum_error    = 0.0;
    unsigned int n = 0;
    
    while (first != last) {
//      doocore::io::sdebug << "x = " << *first << doocore::io::endmsg;
      sum         += (*first) - x_e;
      sum_error   += std::pow(((*first) - x_e),2);
      ++first;
      ++n;
    }
    
    return ValueWithError<T>(x_e + sum/static_cast<double>(n),
                             std::sqrt((sum_error - (sum*sum)/static_cast<double>(n))/static_cast<double>(n-1)));
  }

    /**
     *  @brief Calculate sum based on values
     *
     *  Based on given iterators of values, the sum and the error on the sum are
     *  computed
     *
     *  @param parameter description
     *  @param first iterator for values to start with
     *  @param last iterator for values to end with
     *  @return sum and error as ValueWithError
     */
    template <typename T, typename ValueIterator>
    inline ValueWithError<T> Sum(ValueIterator first, ValueIterator last) {
      T sum          = 0.0;
      T sum_error    = 0.0;
      
      while (first != last) {
        // doocore::io::sdebug << "x = " << (*first).value << " +/- " << (*first).error<< doocore::io::endmsg;
        sum         += (*first).value;
        sum_error   += std::pow((*first).error,2);
        ++first;
      }
        // doocore::io::sdebug << "sum = " << sum << " +/- " << std::sqrt(sum_error) << doocore::io::endmsg;
      return ValueWithError<T>(sum, std::sqrt(sum_error));
    }

  /**
   *  @brief Calculate weighted covariance based on provides values and weights
   *
   *  Based on two given vectors of values and a vector of weights the 
   *  weighted covariance is calculated.
   *
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param w vector of weight values
   *  @return weighted covariance as double
   */
  inline double Covariance(const std::vector<double>&x, const std::vector<double>& y, const std::vector<double>&w){
    if ((x.size() != y.size()) || (x.size() != w.size())){
      doocore::io::serr << "Covariance: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }

    // using re-implementation to prevent unnecessary loops
    // double numerator = 0;
    // double denominator = 0;
    // double w_avg_x = WeightedAverage(x, w);
    // double w_avg_y = WeightedAverage(y, w);
    // for (unsigned long i = 0; i < x.size(); i++){
    //   numerator += w.at(i) * (x.at(i) - w_avg_x) * (y.at(i) - w_avg_y);
    //   denominator += w.at(i);
    // }
    // return numerator / denominator;

    double xy_sum(0), x_sum(0), y_sum(0), w_sum(0);
    for (unsigned long i = 0; i < x.size(); i++){
      xy_sum += w.at(i) * x.at(i) * y.at(i);
      x_sum += w.at(i) * x.at(i);
      y_sum += w.at(i) * y.at(i);
      w_sum += w.at(i);
    }

    xy_sum /= w_sum;
    x_sum /= w_sum;
    y_sum /= w_sum;

    return xy_sum - x_sum * y_sum;
  }

  /**
   *  @brief Calculate Pearson product-moment correlation coefficient
   *
   *  Based on two std::vector<double>'s, the Pearson product-moment 
   *  correlation coefficient is calculated
   *
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param stride stride/step-width (default value: 1)
   *  @return Pearson product-moment correlation coefficient as double
   */
  inline double PearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y) {
    int stride = 1;
    return gsl_stats_correlation( &x[0], stride, &y[0], stride, x.size());
  }

  /**
   *  @brief Calculate weighted Pearson product-moment correlation coefficient
   *
   *  Based on two std::vector<double>'s for the values and one additional vector 
   *  for the associated weights, the Pearson product-moment correlation 
   *  coefficient is calculated
   *
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param w vector of weight values
   *  @return weighted Pearson product-moment correlation coefficient as double
   */
  inline double PearsonCorrelation(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& w){
    if ((x.size() != y.size()) || (x.size() != w.size())){
      doocore::io::serr << "PearsonCorrelation: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }
    
    // using re-implementation to prevend unnecessary loops
    // double numerator = Covariance(x, y, w);
    // double denominator = sqrt(Covariance(x, x, w) * Covariance(y, y, w));
    // return numerator / denominator;

    double xy_sum(0), x_sum(0), x2_sum(0), y2_sum(0), y_sum(0), w_sum(0);
    for (unsigned long i = 0; i < x.size(); i++){
      xy_sum += w.at(i) * x.at(i) * y.at(i);
      x_sum += w.at(i) * x.at(i);
      y_sum += w.at(i) * y.at(i);
      x2_sum += w.at(i) * x.at(i) * x.at(i);
      y2_sum += w.at(i) * y.at(i) * y.at(i);
      w_sum += w.at(i);
    }

    xy_sum /= w_sum;
    x_sum /= w_sum;
    y_sum /= w_sum;
    x2_sum /= w_sum;
    y2_sum /= w_sum;

    return (xy_sum-x_sum*y_sum)/(sqrt(x2_sum-(x_sum*x_sum))*sqrt(y2_sum-(y_sum*y_sum)));
  }

  /**
   *  @brief Permutation null hypothesis test
   *
   *  Based on two std::vector<double>'s, a p-value for
   *  the compatibility of the data with the null hypothesis
   *  is calculated
   *
   *  see: http://en.wikipedia.org/wiki/Pearson_product-moment_correlation_coefficient#Use_a_permutation_test
   *  and: http://en.wikipedia.org/wiki/Permutation_test#Permutation_tests
   *  
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param n_permutations number of permutations (default value: 1000)
   *  @return p-value for the null hypothesis as double
   */
  inline double PermutationTest(const std::vector<double>& x, const std::vector<double>& y, const unsigned long n_permutations=1000){
    if (x.size() != y.size()){
      doocore::io::serr << "PermutationTest: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }
    double rho = PearsonCorrelation(x, y); // Pearson correlation coeff for original data

    // copy y to y_prime to shuffle
    std::vector<double> y_prime = y;
    std::random_device random_device;
    std::mt19937 mersenne_generator(random_device());

    unsigned long n_larger = 0; // number of correlations larger than rho
    for (unsigned long i = 0; i < n_permutations; ++i){
      std::shuffle(std::begin(y_prime), std::end(y_prime), mersenne_generator);
      double r = PearsonCorrelation(x, y_prime);
      if (((rho > 0) && (r > rho)) || ((rho < 0) && (r < rho))){
        n_larger++;
      }
    }
    double p = 1.0 * n_larger / n_permutations;
    // doocore::io::sdebug << "PermutationTest: rho = " << rho << ", p = " << p << doocore::io::endmsg;
    return p;
  }

  /**
   *  @brief Bootstrap test
   *
   *  Based on two std::vector<double>'s, a 95% CL for the
   *  underlying sample distribution is calculated
   *
   *  see: http://en.wikipedia.org/wiki/Bootstrapping_(statistics)
   *  
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param n_permutations number of permutations (default value: 1000)
   *  @return low (2.5%) and high (97.5%) quantile values as std::pair<double, double> 
   */
  inline std::pair<double, double> BootstrapTest(const std::vector<double>& x, const std::vector<double>& y, const unsigned long n_permutations=1000){
    if (x.size() != y.size()){
      doocore::io::serr << "BootstrapTest: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }
    std::vector<double> x_prime;
    std::vector<double> y_prime;

    unsigned long vx_size = x.size();

    std::random_device random_device;
    std::mt19937 mersenne_generator(random_device());
    std::uniform_int_distribution<> uniform_int(0, vx_size-1);

    std::vector<double> r;
    for (unsigned long i = 0; i < n_permutations; ++i){
      // resampling vectors
      x_prime.clear();
      y_prime.clear();

      for (unsigned long i = 0; i < vx_size; i++ ){
        unsigned long idx = uniform_int(mersenne_generator);
        x_prime.push_back(x.at(idx));
        y_prime.push_back(y.at(idx));
      }
      r.push_back(PearsonCorrelation(x_prime, y_prime));
    }
    std::sort(r.begin(), r.end());
    double quantile_lo = gsl_stats_quantile_from_sorted_data(&r[0], 1, n_permutations, 0.025);
    double quantile_hi = gsl_stats_quantile_from_sorted_data(&r[0], 1, n_permutations, 0.975);

    return std::make_pair(quantile_lo, quantile_hi);
  }

  /**
   *  @brief Bootstrap test for weighted data
   *
   *  Based on two std::vector<double>'s for the values and one additional vector 
   *  for the associated weights, a 95% CL for the underlying sample 
   *  distribution is calculated
   *
   *  see: http://en.wikipedia.org/wiki/Bootstrapping_(statistics)
   *  
   *  @param parameter description
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @param w vector of weight values
   *  @param n_permutations number of permutations (default value: 1000)
   *  @return low (2.5%) and high (97.5%) quantile values as std::pair<double, double> 
   */
  inline std::pair<double, double> BootstrapTest(const std::vector<double>& x, const std::vector<double>& y, const std::vector<double>& w, const unsigned long n_permutations=1000){
    if ((x.size() != y.size()) || (x.size() != w.size())){
      doocore::io::serr << "BootstrapTest: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }
    std::vector<double> x_prime;
    std::vector<double> y_prime;
    std::vector<double> w_prime;

    unsigned long vx_size = x.size();

    std::random_device random_device;
    std::mt19937 mersenne_generator(random_device());
    std::uniform_int_distribution<> uniform_int(0, vx_size-1);

    std::vector<double> r;
    for (unsigned long i = 0; i < n_permutations; ++i){
      // resampling vectors
      x_prime.clear();
      y_prime.clear();
      w_prime.clear();

      for (unsigned long i = 0; i < vx_size; i++ ){
        unsigned long idx = uniform_int(mersenne_generator);
        x_prime.push_back(x.at(idx));
        y_prime.push_back(y.at(idx));
        w_prime.push_back(w.at(idx));
      }
      r.push_back(PearsonCorrelation(x_prime, y_prime, w_prime));
    }
    std::sort(r.begin(), r.end());
    double quantile_lo = gsl_stats_quantile_from_sorted_data(&r[0], 1, n_permutations, 0.025);
    double quantile_hi = gsl_stats_quantile_from_sorted_data(&r[0], 1, n_permutations, 0.975);

    return std::make_pair(quantile_lo, quantile_hi);
  }

  /**
   *  @brief Get Mean from RooRealVar inside RooDataSet
   *
   *  @param dataset RooDataSet from which to extract values
   *  @param param_name parameter name to look for in dataset
   */
  inline double get_mean_from_dataset(const RooDataSet *dataset, const std::string& param_name){
    std::vector<double> data;

    for(int i=0; i<dataset->numEntries(); i++){
      const RooArgSet* params = dataset->get(i);
      data.push_back(params->getRealValue(param_name.c_str()));
    }

    double mean = gsl_stats_mean(&(data.at(0)), 1, data.size());

    return mean;
  }

  /**
   *  @brief Get Median from RooRealVar inside RooDataSet
   *
   *  @param dataset RooDataSet from which to extract values
   *  @param param_name parameter name to look for in dataset
   */
  inline double get_median_from_dataset(const RooDataSet *dataset, const std::string& param_name){
    std::vector<double> data;

    for(int i=0; i<dataset->numEntries(); i++){
      const RooArgSet* params = dataset->get(i);
      data.push_back(params->getRealValue(param_name.c_str()));
    }

    std::sort(data.begin(), data.end());
    double median = gsl_stats_median_from_sorted_data(&(data.at(0)), 1, data.size());

    return median;
  }

  /**
   *  @brief Get Quantile from RooRealVar inside RooDataSet
   *
   *  Takes a RooDataSet and a parameter name and calculates the quantile with the
   *  given fraction of lying values below that value. Also writes the sorted data
   *  to a vector of doubles for the caller, to save time the next time a quantile is computed.
   *  This vector needs to have the same dimension as data.
   *
   *  @param data RooDataSet from which to extract values
   *  @param param_name parameter name to look for in dataset
   *  @param fraction demanded fraction of data below quantile
   *  @param sorted_dataset vector to store sorted values (in ascending order).
   */
  inline double get_quantile_from_dataset(const RooDataSet *data, const std::string& param_name, double fraction, std::vector<double>& sorted_dataset){
    if(!sorted_dataset.empty())
      sorted_dataset.clear();

    for(int i=0; i<data->numEntries(); i++){
      const RooArgSet* params = data->get(i);
      sorted_dataset.push_back(params->getRealValue(param_name.c_str()));
    }

    std::sort(sorted_dataset.begin(), sorted_dataset.end());
    double quantile = gsl_stats_quantile_from_sorted_data(&(sorted_dataset.at(0)), 1, sorted_dataset.size(), fraction);
    
    return quantile;
  }

  /**
   *  @brief Get Quantile from RooRealVar inside RooDataSet
   *
   *  Takes a RooDataSet and a parameter name and calculates the quantile with the
   *  given fraction of lying values below that value. Also writes the sorted data
   *  to a vector for the caller, to save time the next time a quantile is computed.
   *  Does not store sorted values.
   *
   *  @param data RooDataSet from which to extract values
   *  @param param_name parameter name to look for in dataset
   *  @param fraction demanded fraction of data below quantile
   */
  inline double get_quantile_from_dataset(const RooDataSet *data, const std::string& param_name, double fraction){
    std::vector<double> dataptr(data->numEntries());
    double quantile = get_quantile_from_dataset(data, param_name, fraction, dataptr);
    return quantile;
  }
  
  /**
   *  @brief Get Quantile from sorted dataset
   *
   *  Simple wrapper of gsl_stats_quantile_from_sorted_data
   *  
   *  @param sorted_dataset dataset of values in ascending order
   *  @param fraction demanded fraction of data below quantile
   */
  inline double get_quantile_from_dataset(const std::vector<double>& sorted_dataset, double fraction){
    return gsl_stats_quantile_from_sorted_data(&(sorted_dataset.at(0)), 1, sorted_dataset.size(), fraction);
  }

  /**
   *  @brief Shuffles a RooDataSet with replacement
   *
   *  Draws entries from a RooDataSet with replacement.
   *  ATTENTION: User takes ownership of returned dataset.
   *  
   *  @param prototype proto dataset used for the shuffeling
   *  @param random_seed a random seed used for the shuffeling
   *  @param n_shuffles number of entries to draw
   */
  inline RooDataSet* shuffle_dataset_with_replacement(const RooDataSet* prototype, int random_seed, unsigned int n_shuffles = 0){
    RooDataSet* bootstrapped_data = new RooDataSet("shuffled_dataset",
                                                   "shuffled_dataset",
                                                   *prototype->get(0));

    unsigned int max_id = prototype->numEntries() - 1;
    n_shuffles = (n_shuffles == 0 ? prototype->numEntries() : n_shuffles);
    
    TRandom3 rgen(random_seed);
    for(int i=0; i<n_shuffles; i++) {
      unsigned int event_id = rgen.Integer(max_id);
      bootstrapped_data->add(*prototype->get(event_id));
    }
    return bootstrapped_data;
  }


} // namespace general
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_GENERAL_H
