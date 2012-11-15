#ifndef DOOCORE_CONFIG_SUMMARY_H
#define DOOCORE_CONFIG_SUMMARY_H

// from STL

// from ROOT
#include <TString.h>

// from RooFit

// from TMVA

// from BOOST

// from here

// forward declarations

namespace doocore {
namespace config {
/*! @class doocore::config::Summary
 * @brief On the fly summary of important variables and settings
 *
 * This class allows to create a summary object to collect important
 * variables for bookkeeping. 
 *
 * NOTE: This class is a singleton!
 * ----------------------------------------------------------------
 * Wherever you create an instance through: GetInstance() the same
 * Summary object is called/used/created. So you can use this for
 * bookkeeping in different parts of the program.
 * ----------------------------------------------------------------

 * @section summary_Usage Usage
 * 
 * Usage of Summary is simple. Consider this example:
 * 
 * @code
 * #include "doocore/config/Summary.h"
 *   int main(){
 *   // initialize Summary object
 *   doocore::config::Summary& mySummary = doocore::config::Summary::GetInstance();
 *    
 *   // add an entry
 *   mySummary.Add("Description", "Value");
 *   // add a section 
 *   mySummary.AddSection("Section name");
 *   // add a horizontal line
 *   mySummary.AddHLine();
 *   // get promt output
 *   mySummary.Print();
 * }
 * @endcode 
 */
class Summary {
 public:
  /**
   * @brief Constructor for Summary
   *
   */
  static Summary& GetInstance();
  
  /**
   *  @brief Destructor for Summary
   */
  static void DestroyInstance();
  
  /**
   *  @brief add a key/value pair to the summary
   */
  void Add(TString description, TString argument);
  
  /**
   *  @brief add a section to the summary
   */  
  void AddSection(TString name);
  
  /**
   *  @brief add a horizontal to the summary
   */
  void AddHLine();
  
  /**
   *  @brief print the summary
   */
  void Print();

  /// not yet implemented (write to output file)
  void Write(std::string filename="");
  
  /// not yet implemented
  void StartClock();
  
  /// not yet implemented
  void StopClock();
  
 protected:
  
 private:
  /// static private instance
  static Summary *instance_;

  /// private constructor
  Summary();

  /// private copy constructor
  Summary(const Summary&);

  /// private destructor
  ~Summary() {};
  
  /// internal debug mode  
  bool debug_mode_;
  
  /// internal vector to log all key,value pairs  
  std::vector< std::pair< TString,TString > > log_;

}; // class Summary
} // namespace config
} // namespace doocore

#endif // DOOCORE_CONFIG_SUMMARY_H
