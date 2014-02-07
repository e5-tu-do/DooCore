
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
  sinfo << ValueWithError<double>(0.335646548e-6, 0.335646548e-6) << endmsg;
  sinfo << ValueWithError<double>(122.572427568, 122.572427568) << endmsg;
  sinfo << ValueWithError<double>(3.1415927, 3.1415927) << endmsg;

  sinfo << ValueWithError<double>(34000000, 340000) << endmsg;
  sinfo << ValueWithError<double>(3400000, 34000) << endmsg;
  sinfo << ValueWithError<double>(340000, 3400) << endmsg;
  sinfo << ValueWithError<double>(34000, 340) << endmsg;
  sinfo << ValueWithError<double>(3400, 34) << endmsg;
  sinfo << ValueWithError<double>(340, 3.4) << endmsg;
  sinfo << ValueWithError<double>(34, 0.34) << endmsg;
  sinfo << ValueWithError<double>(3.4, 0.034) << endmsg;
  sinfo << ValueWithError<double>(0.34, 0.0034) << endmsg;
  sinfo << ValueWithError<double>(0.034, 0.00034) << endmsg;
  sinfo << ValueWithError<double>(0.0034, 0.000034) << endmsg;
  sinfo << ValueWithError<double>(0.00034, 0.0000034) << endmsg;

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
  
  auto mean_error = doocore::statistics::general::WeightedAverage<double>(values.begin(), values.end());
  
  sinfo << mean_error << endmsg;
  //sinfo << mean_error.value << " +/- " << mean_error.error << endmsg;
}