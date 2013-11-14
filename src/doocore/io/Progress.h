#ifndef DOOCORE_IO_PROGRESS_H
#define DOOCORE_IO_PROGRESS_H

// from STL
#include <string>
#include <chrono>

namespace doocore {
namespace io {

/** @class doocore::io::Progress
 *  @brief Progress indicator class with cool features
 *
 *  A progress output indicator. It will be initialised with a name of the #
 *  current task and the total number of steps to perform. For each step 
 *  operator++ is called on the Progress object which will update the progress
 *  indicator on demand. 
 *
 *  The operator++ function is implemented such that not each call will trigger
 *  an update of the indicator to optimise the cost per call. Nevertheless, each
 *  call of operator++ costs (on an arbitrary reference machine): 
 * 
 *  -O0: ~30 ns.
 *  -O3: ~4 ns.
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
  
  Progress& operator++() {
    ++steps_since_update_;
    Update();
    
    return *this;
  }
  
  Progress& operator+=(int steps) {
    steps_since_update_ += steps;
    Update();
    
    return *this;
  }
  
 protected:
  
 private:
  
  void Update() {
    if (tty_ && steps_since_update_ > step_position_update_tty_) {
      //time_now_ = std::chrono::high_resolution_clock::now();
      position_ += steps_since_update_;
      steps_since_update_ = 0;
      
      //if (std::chrono::duration_cast<std::chrono::milliseconds>(time_now_ - time_last_).count() > 1) {
      //time_last_ = time_now_;
      progress_fraction_ = static_cast<double>(position_)/num_steps_total_;
      printf("Progress %.2f %         \xd", progress_fraction_*100.0);
      fflush(stdout);
      //}
    } else if (!tty_ && steps_since_update_ > step_position_update_notty_){
      position_ += steps_since_update_;
      steps_since_update_ = 0;
      
      progress_fraction_ = static_cast<double>(position_)/num_steps_total_;
      
      printf("Progress %.2f %         \n", progress_fraction_*100.0);
      fflush(stdout);
    }
  }
  
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
  std::chrono::high_resolution_clock::time_point time_last_;
  
  /**
   *  @brief Time of now (updated regularly)
   */
  std::chrono::high_resolution_clock::time_point time_now_;
};

} // namespace io
} // namespace doocore

#endif // DOOCORE_IO_PROGRESS_H
