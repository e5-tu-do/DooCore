#ifndef DOOCORE_CONFIG_SUMMARY_H
#define DOOCORE_CONFIG_SUMMARY_H

// from STL
#include <set>

// from ROOT
#include <TString.h>

// from RooFit

// from TMVA

// from BOOST
#ifdef __GNUG__
#define BOOST_NO_CXX11_SCOPED_ENUMS
#endif
#include <boost/filesystem.hpp>

// from DooCore
#include <doocore/io/MsgStream.h>

// from here

// forward declarations
class TCut;

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
  //static void DestroyInstance();
  
  /**
   *  @brief add a key/value pair to the summary
   */
  void Add(TString description, TString argument);
  void Add(TString description, TCut argument);
  void Add(TString description, bool argument);
  void Add(TString description, std::string argument);
  void Add(TString description, double argument);
  void Add(TString description, int argument);
  
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
  void Print(doocore::io::MsgStream& stream=doocore::io::scfg);

  /// not yet implemented (write to output file)
  void Write(std::string filename="");
  
  /// not yet implemented
  void StartClock();
  
  /// not yet implemented
  void StopClock();
  
  /**
   *  @brief Add file to run summary
   *
   *  Add a specific file to the run summary. The file will be copied to the
   *  summary directory upon program termination.
   *
   *  @param file file to include in run summary directory
   */
  Summary& set_output_directory(std::string output_directory) {
    output_directory_ = output_directory;
    return *this;
  }
  
  /**
   *  @brief Add file to run summary
   *
   *  Add a specific file to the run summary. The file will be copied to the 
   *  summary directory upon program termination.
   *
   *  @param file file to include in run summary directory
   */
  void AddFile(const boost::filesystem::path& file);
  
  /**
   *  @brief Flush all summary information and reset
   *
   *  Flush all summary logs and copy files to summary. Afterwards, the logs and
   *  file lists will be reset in order to allow a fresh new Summary. This is 
   *  useful if summaries have to be given at certain stages of the overall 
   *  program execution.
   */
  void SummarizeAndReset() {
    CopyFiles();
    log_.clear();
    files_.clear();
  }
  
 protected:
  
 private:
  /// static private instance
  // static Summary *instance_;

  /// private constructor
  Summary();

  /// private copy constructor
  Summary(const Summary&);

  /// private destructor
  ~Summary() {
    if (log_.size() > 0 || files_.size() > 0) CopyFiles();
  };
  
  /**
   *  @brief Copy all previously added files to summary directory
   *
   *  This function is called upon destruction of Summary and will copy all 
   *  relevant files previously added via AddFile() to the summary directory.
   */
  void CopyFiles();
  
  /// internal debug mode
  bool debug_mode_;
  
  /// internal vector to log all key,value pairs  
  std::vector< std::pair< TString,TString > > log_;
  
  /**
   *  @brief List of files to add to the summary
   */
  std::set<boost::filesystem::path> files_;
  
  /**
   *  @brief Output directory for summary
   */
  std::string output_directory_;
}; // class Summary
} // namespace config
} // namespace doocore

#endif // DOOCORE_CONFIG_SUMMARY_H
