#include "Progress.h"

// from STL
#include <string>
#include <chrono>

// POSIX/UNIX
#include <unistd.h>

// from DooCore
#include "doocore/io/MsgStream.h"

doocore::io::Progress::Progress(std::string name_task, long long num_steps_total) :
name_task_(name_task),
num_steps_total_(num_steps_total),
position_(0),
steps_since_update_(0),
step_position_update_tty_(num_steps_total/10000),
step_position_update_notty_(num_steps_total/200),
progress_fraction_(0.0),
tty_(isatty(fileno(stdout))),
time_last_(std::chrono::high_resolution_clock::now()),
time_now_(std::chrono::high_resolution_clock::now())
{
  sinfo << "Progress: " << name_task_ << endmsg;
}
