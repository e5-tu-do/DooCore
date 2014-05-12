#ifndef DOOCORE_STATISTICS_GENERAL_H
#define DOOCORE_STATISTICS_GENERAL_H

// from STL
#include <utility>
#include <cmath>
#include <cfenv>
#include <string>
#include <chrono>
#include <random>

// from Boost
#include <boost/format.hpp>

// from ROOT
#include "TMath.h"

// from RooFit

// from DooCore
#include "doocore/io/MsgStream.h"

// from GSL
#include "gsl/gsl_statistics.h"

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
    std::stringstream output;
    if (error == 0.0) {
      output << value << " +/- " << error;
    } else {
      std::fesetround(FE_TONEAREST);
      int mantissa_err   = std::nearbyint(error*100.0*std::pow(10.0,-static_cast<int>(std::floor(std::log10(error)))));
      T exp_err     = std::log10(error);
      T abs_exp_err = std::abs(exp_err);
      
      // additional digits if mantissa of error <= 3.54
      int add_digits     = 0;
      if (mantissa_err <= 354) add_digits++;
      
      std::string format;
      
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
//      doocore::io::sdebug << "x = " << (*first).value << ", e = " << (*first).error << ", w = " << (*first).weight << doocore::io::endmsg;
      sum         += (*first).weight * (*first).value;
      sum_weights += (*first).weight;
      sum_error   += (*first).weight*(*first).weight*(*first).error*(*first).error;
      ++first;
    }
    
    return ValueWithError<T>(sum/sum_weights,TMath::Sqrt(sum_error)/sum_weights);
  }

  /**
   *  @brief Calculate weighted average based on values and weights
   *
   *  Based on a provided dataset including weights the weighted average is computed. 
   *
   *  @param x vector of first set of values
   *  @param y vector of second set of values
   *  @return weighted average as double
   */
  inline double WeightedAverage(const std::vector<double>&x, const std::vector<double>& w){
    if (x.size() != w.size()){
      doocore::io::serr << "WeightedAverage: Different size of vectors!" << doocore::io::endmsg;
      abort();
    }
    double numerator = 0;
    double denominator = 0;
    for (unsigned long i = 0; i < x.size(); i++){
      numerator += x.at(i) * w.at(i);
      denominator += w.at(i);
    }
    return numerator / denominator;
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

} // namespace general
} // namespace statistics
} // namespace doocore

#endif // DOOCORE_STATISTICS_GENERAL_H
