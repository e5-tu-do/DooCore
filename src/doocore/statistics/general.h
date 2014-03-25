#ifndef DOOCORE_STATISTICS_GENERAL_H
#define DOOCORE_STATISTICS_GENERAL_H

// from STL
#include <utility>
#include <cmath>
#include <cfenv>
#include <string>

// from Boost
#include <boost/format.hpp>

// from ROOT
#include "TMath.h"

// from RooFit

// from DooCore
#include "doocore/io/MsgStream.h"

// forward decalarations

/**
 * @namespace doocore::statistics
 * @brief DooCore statistics functionality namespace
 *
 * This namespace contains all DooCore functionality that does statistics
 * calculations.
 *
 * @author Florian Kruse
 */

/**
 * @namespace doocore::statistics::general
 * @brief DooCore general statistics functionality namespace
 *
 * This namespace contains all DooCore functionality that does general 
 * statistics calculations that is not part os a more specialised namespace.
 */

namespace doocore {
namespace statistics {
namespace general {
  
  /**
   *  @class doocore::statistics::general::ValueWithError
   *  @brief Simple value with error compound type (and optional weight)
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
    ValueWithError(T val, T err) : value(val), error(err), weight(1.0) {}
    ValueWithError(T val, T err, T wgt) : value(val), error(err), weight(wgt) {}
    
    /**
     *  @brief Format value +/- error with PDG rounding
     *
     *  Based on the PDG rounding rules, the value and error will be formatted 
     *  as strings.
     *
     *  @return value +/- error as string
     */
    std::string FormatString() const;
    
    T value;
    T error;
    T weight;
  };
  
  template<typename T>
  std::string doocore::statistics::general::ValueWithError<T>::FormatString() const {
    std::fesetround(FE_TONEAREST);
    int mantissa_err   = std::nearbyint(error*100.0*std::pow(10.0,-static_cast<int>(std::floor(std::log10(error)))));
    T exp_err     = std::log10(error);
    T abs_exp_err = std::abs(exp_err);
    
    // additional digits if mantissa of error <= 3.54
    int add_digits     = 0;
    if (mantissa_err <= 354) add_digits++;
    
    std::string format;
    std::stringstream output;
    
    // depending on exponent use scientific notation or not
    if (abs_exp_err < 5) {
      if (exp_err < 1.0) {
        std::fesetround(FE_DOWNWARD);
        format = "%." + std::to_string(static_cast<int>(std::abs(std::nearbyint(exp_err))+add_digits)) + "f";
      } else {
        format = "%.0f";
      }
      output << boost::format(format) % value << " +/- " << boost::format(format) % error;
    } else {
      format = "%." + std::to_string(add_digits) + "f";
      T exp_new_err      = std::floor(exp_err);
      T mantissa_new_err = error/std::pow(10.0,exp_new_err);
      T mantissa_new_val = value/std::pow(10.0,exp_new_err);
      
      output << boost::format(format) % mantissa_new_val << "e" << exp_new_err << " +/- " << boost::format(format) % mantissa_new_err << "e" << exp_new_err;
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
      sum         += (*first).weight * (*first).value;
      sum_weights += (*first).weight;
      sum_error   += (*first).weight*(*first).weight*(*first).error*(*first).error;
      ++first;
    }
    
    return ValueWithError<T>(sum/sum_weights,TMath::Sqrt(sum_error)/sum_weights);
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
      sum         += (*first) - x_e;
      sum_error   += std::pow(((*first) - x_e),2);
      ++first;
      ++n;
    }
    
    return ValueWithError<T>(x_e + sum/static_cast<double>(n),
                             (sum_error - (sum*sum)/static_cast<double>(n))/static_cast<double>(n-1));
  }


} // namespace general
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_GENERAL_H
