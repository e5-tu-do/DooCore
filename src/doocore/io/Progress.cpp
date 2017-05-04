#include "Progress.h"

// from STL
#include <string>
#include <chrono>
#include <cmath>

// POSIX/UNIX
#include <unistd.h>

// from DooCore
#include "doocore/io/MsgStream.h"
#include "doocore/io/Tools.h"

doocore::io::Progress::Progress(std::string name_task, long long num_steps_total) :
name_task_(name_task),
num_steps_total_(num_steps_total),
position_(0),
steps_since_update_(0),
step_position_update_tty_(num_steps_total/10000),
step_position_update_notty_(num_steps_total/20),
progress_fraction_(0.0),
tty_(isatty(fileno(stdout))),
time_start_(std::chrono::high_resolution_clock::now()),
time_now_(std::chrono::high_resolution_clock::now()),
elapsed_(0)
{
  if (name_task.size() > 0) {
    sinfo << "Progress: " << name_task_ << endmsg;
  }
  Update(true);
}

std::string doocore::io::Progress::SecondsToTimeString(double seconds) const {
  return doocore::io::tools::SecondsToTimeString(seconds);
}

std::string doocore::io::Progress::MakeProgressBar(double fraction) const {
  static unsigned int cols = 42;
  unsigned int cols_filled = round(fraction*(cols-2));
  unsigned int steps = (cols-2)/4;
  
  std::string progress_bar(cols, ' ');

  progress_bar[0]       = '|';
  progress_bar[steps]   = '|';
  progress_bar[steps*2] = '|';
  progress_bar[steps*3] = '|';
  progress_bar[cols-1]  = '|';
  
  for (unsigned int i=1; i<1+cols_filled; ++i) {
    progress_bar[i] = '=';
  }
  
  progress_bar[0]       = '|';
  progress_bar[steps]   = '|';
  progress_bar[steps*2] = '|';
  progress_bar[steps*3] = '|';
  progress_bar[cols-1]  = '|';
  
  progress_bar[cols_filled>0 ? cols_filled : 1] = '>';
  
//  sdebug << progress_bar << endmsg;
  
  return progress_bar;
}