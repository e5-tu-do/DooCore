
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
  using namespace doocore::statistics::general;
  
  std::vector<ValueWithError<double>> values;
  
  values  += ValueWithError<double>(1.21, 0.12, 56.8149), ValueWithError<double>(0.30, 0.04, 10.804);
  
  sinfo << values << endmsg;
  
  auto mean_error = doocore::statistics::general::WeightedAverage<double>(values.begin(), values.end());
  
  sinfo << mean_error.value << " +/- " << mean_error.error << endmsg;
}