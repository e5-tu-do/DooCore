
// from STL
#include <chrono>

// from DooCore
#include "doocore/io/Progress.h"
#include "doocore/io/MsgStream.h"
#include "doocore/lutils/lutils.h"

void func() {
  int i = 0;
  
  i = 42;
}

int main () {
  using namespace doocore::io;
  long long steps = 1000000;
  
  Progress p("my task", steps);
  
  std::chrono::high_resolution_clock::time_point time_start = std::chrono::high_resolution_clock::now();
  for (long long i=0; i<steps; ++i) {
    ++p;
    //func();
  }
  std::chrono::high_resolution_clock::time_point time_stop = std::chrono::high_resolution_clock::now();
  
  std::chrono::high_resolution_clock::time_point time_noop_start = std::chrono::high_resolution_clock::now();
  for (long long i=0; i<steps; ++i) {
    //++p;
    //func();
  }
  std::chrono::high_resolution_clock::time_point time_noop_stop = std::chrono::high_resolution_clock::now();
  
  p.Finish();
  
  sinfo << "Time per operator++ call: " << static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(time_stop - time_start).count()-std::chrono::duration_cast<std::chrono::nanoseconds>(time_noop_stop - time_noop_start).count())/steps << " ns." << endmsg;
  sinfo << "Time per no call loop: " << static_cast<double>(std::chrono::duration_cast<std::chrono::nanoseconds>(time_noop_stop - time_noop_start).count())/steps << " ns." << endmsg;
}