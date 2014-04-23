#ifndef DOOCORE_IO_PROGRESS_H
#define DOOCORE_IO_PROGRESS_H

// from STL
#include <string>
#include <chrono>

// from DooCore
#include "doocore/io/MsgStream.h"

namespace doocore {
namespace io {

/** @class doocore::io::Progress
 *  @brief Progress indicator class with cool features
 *
 *  A progress output indicator. It will be initialised with a name of the #
 *  current task and the total number of steps to perform. For each step 
 *  operator++ is called on the Progress object which will update the progress
 *  indicator on demand. Alternatively, operator+= can be called for larger
 *  steps.
 *
 *  The operator++ function is implemented such that not each call will trigger
 *  an update of the indicator to optimise the cost per call. Nevertheless, each
 *  call of operator++ costs (on an arbitrary reference machine): 
 * 
 *  -O0: ~60 ns.
 *  -O3: ~4 ns.
 *  
 *  @section p_example Usage example
 *
 * @code
 * int main () {
 *   using namespace doocore::io;
 *   long long steps = 1000000000;
 *   Progress p("my task", steps);
 *
 *   for (long long i=0; i<steps; ++i) {
 *     ++p;
 *   }
 *   p.Finish();
 * }
 * @endcode
 */

// forward declarations

class Progress {
 public:
  /**
   *  @brief Constructor
   *
   *  The constructor constructs. What else did you think it does?
   *
   *  @param name_task name of the task to print on the terminal
   *  @param num_steps_total total number of steps to do
   */
  Progress(std::string name_task, long long num_steps_total);

  virtual ~Progress() {}
  
  /**
   *  @brief Increase step counter by 1
   */
  Progress& operator++() {
    ++steps_since_update_;
    Update();
    
    return *this;
  }
  
  /**
   *  @brief Increase step counter by increment
   */
  Progress& operator+=(int steps) {
    steps_since_update_ += steps;
    Update();
    
    return *this;
  }
  
  /**
   *  @brief Finish progress writing by printing the progress permanently
   */
  void Finish() {
    Update(true);
    printf("\n");
  }
  
 protected:
  
 private:
  
  void Update(bool force_update=false) {
    if ((tty_ && steps_since_update_ > step_position_update_tty_) || force_update) {
      position_ += steps_since_update_;
      if (position_>num_steps_total_) {
        position_ = num_steps_total_;
      }
      
      steps_since_update_ = 0;
      
      progress_fraction_ = static_cast<double>(position_)/num_steps_total_;

      time_now_ = std::chrono::high_resolution_clock::now();
      elapsed_ = std::chrono::duration_cast<std::chrono::microseconds>(time_now_ - time_start_).count()*1e-6;
      double remaining = static_cast<double>(elapsed_)/progress_fraction_-static_cast<double>(elapsed_);
      printf("%s %.2f %% (time elapsed / remaining / per step[ms]: %s / %s / %.2f)        \xd", MakeProgressBar(progress_fraction_).c_str(), (progress_fraction_*100.0), SecondsToTimeString(elapsed_).c_str(), SecondsToTimeString(remaining).c_str(), (elapsed_/position_*1000.0));
      fflush(stdout);
    } else if ((!tty_ && steps_since_update_ > step_position_update_notty_) || force_update){
      position_ += steps_since_update_;
      if (position_>num_steps_total_) {
        position_ = num_steps_total_;
      }
      steps_since_update_ = 0;
      
      progress_fraction_ = static_cast<double>(position_)/num_steps_total_;
      
      time_now_ = std::chrono::high_resolution_clock::now();
      elapsed_ = std::chrono::duration_cast<std::chrono::microseconds>(time_now_ - time_start_).count()*1e-6;
      double remaining = static_cast<double>(elapsed_)/progress_fraction_-static_cast<double>(elapsed_);
      printf("%s %.2f %% (time elapsed / remaining / per step[ms]: %s / %s / %.2f)        \n", MakeProgressBar(progress_fraction_).c_str(), progress_fraction_*100.0, SecondsToTimeString(elapsed_).c_str(), SecondsToTimeString(remaining).c_str(), elapsed_/position_*1000.0);
      fflush(stdout);
    }
  }
  
  std::string SecondsToTimeString(double seconds) const;
  
  std::string MakeProgressBar(double fraction) const;
  
  /**
   *  @brief Name of the task to perform
   */
  std::string name_task_;
  
  /**
   *  @brief Number of total steps
   */
  long long num_steps_total_;
  
  /**
   *  @brief Current position in progress
   */
  long long position_;
  
  /**
   *  @brief Steps since last update
   */
  long long steps_since_update_;
  
  /**
   *  @brief Minimum step size for position updates (tty)
   */
  const long long step_position_update_tty_;
  
  /**
   *  @brief Minimum step size for position updates (no tty)
   */
  const long long step_position_update_notty_;
  
  /**
   *  @brief Current progress as fraction of 1
   */
  double progress_fraction_;
  
  /**
   *  @brief Are we running on a tty terminal
   */
  const bool tty_;
  
  /**
   *  @brief Time of last update
   */
  std::chrono::high_resolution_clock::time_point time_start_;
  
  /**
   *  @brief Time of now (updated regularly)
   */
  std::chrono::high_resolution_clock::time_point time_now_;
  
  /**
   *  @brief Elapsed time
   */
  double elapsed_;
};

} // namespace io
} // namespace doocore

#endif // DOOCORE_IO_PROGRESS_H
