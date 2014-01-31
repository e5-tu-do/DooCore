#ifndef DOOCORE_STATISTICS_GENERAL_H
#define DOOCORE_STATISTICS_GENERAL_H

// from STL
#include <utility>

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
   *  @struct doocore::statistics::general::ValueWithError
   *  @brief Simple value with error compound type (and optional weight)
   */
  template<typename T>
  struct ValueWithError {
    ValueWithError(T val, T err) : value(val), error(err), weight(1.0) {}
    ValueWithError(T val, T err, T wgt) : value(val), error(err), weight(wgt) {}
    
    T value;
    T error;
    T weight;
  };
  
  /**
   *  @brief Function to output doocore::statistics::general::ValueWithError directly and nicely into MsgStreams
   */
  template<typename T>
  inline doocore::io::MsgStream& operator<<(doocore::io::MsgStream& lhs, const ValueWithError<T>& arg) {
    if (arg.weight != 1.0) {
      lhs.stream() << arg.value << " +/- " << arg.error << " (w: " << arg.weight << ")";
    } else {
      lhs.stream() << arg.value << " +/- " << arg.error;
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

} // namespace general
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_GENERAL_H
