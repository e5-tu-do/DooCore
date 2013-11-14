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
 *  Detailed description.
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
  
  Progress& operator++();
  
 protected:
  
 private:
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
   *  @brief Last printed position
   */
  long long last_position_;
  
  /**
   *  @brief Minimum step size for position updates
   */
  long long step_position_update_;
  
  /**
   *  @brief Current progress as fraction of 1
   */
  double progress_fraction_;
  
  /**
   *  @brief Last printed progress fraction (in case on non-tty terminal)
   */
  double last_progress_fraction_;
  
  /**
   *  @brief Are we running on a tty terminal
   */
  bool tty_;
  
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
