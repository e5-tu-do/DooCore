
// from STL
#include <vector>
#include <utility>

// from Boost
#include <boost/assign/std/vector.hpp> // for 'operator+=()'
using namespace boost::assign; // bring 'operator+=()' into scope

// from DooCore
#include "doocore/statistics/general.h"
#include "doocore/io/MsgStream.h"
#include "doocore/lutils/lutils.h"

int main() {
  using namespace doocore::io;
  
  std::vector<double> values, errors, weights;
  
  values  += 1.21, 0.30;
  errors  += 0.12, 0.04;
  weights += 56.8149, 10.804;
  
  auto mean_error = doocore::statistics::general::WeightedAverage<double>(values.begin(), values.end(),
                                                                          weights.begin(), errors.begin());
  
  sinfo << mean_error.value << " +/- " << mean_error.error << endmsg;
}