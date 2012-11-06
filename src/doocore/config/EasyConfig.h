#ifndef DOOCORE_CONFIG_EASYCONFIG_H
#define DOOCORE_CONFIG_EASYCONFIG_H

// from STL
#include <string>

// from ROOT

// from RooFit

// from TMVA

// from BOOST
#include <boost/property_tree/ptree.hpp>

// from here

// forward declarations

namespace doocore {
namespace config {
/*! @class doocore::config::EasyConfig
 * @brief Easy configuration through config files from outside the code
 *
 * EasyConfig allows configuration using a text-only file without knowledge
 * of the underlying boost::property_tree techniques.
 *
 * @section ec_Usage Usage
 * 
 * Usage of EasyConfig is simple. Consider this example:
 * 
 * @code
 * #include "doocore/config/EasyConfig.h"
 * int main(){
 *   // initialize config object and pass config file
 *   doocore::config::EasyConfig myConfig("/path/to/config/name.cfg");
 *   
 *   // get different data types from config file
 *   std::string superstring = myConfig.getString("general.name");
 *   double trouble          = myConfig.getDouble("special.number");
 *   int count               = myConfig.getInt("special.count");
 *   bool decision           = myConfig.getBool("special.decision");
 * }
 * @endcode 
 * 
 * The config file has to be in the INFO format (http://www.boost.org/doc/libs/1_41_0/doc/html/boost_propertytree/parsers.html#boost_propertytree.parsers.info_parser).
 * A file to start with could be:
 * 
 * @code
 * general
 * { 
 *   name "harry"
 * }
 * 
 * special
 * {
 *   number "13.37"
 *   count "42"
 *   decision "false"
 * }
 * @endcode
 */
class EasyConfig {
 public:
  /**
   *  @brief Constructor for EasyConfig with config file to use
   *
   *  Based on the supplied file name the property tree will be populated
   *  and is ready to use.
   *
   *  @param filename file name of config file to use
   */
  EasyConfig(std::string filename);
  
  /**
   *  @brief Destructor for EasyConfig
   */
  ~EasyConfig();
  
  /**
   *  @brief Get string from config file
   *
   *  @return an std::string
   */
  std::string getString(std::string name);

  /**
   *  @brief Get boolean from config file
   *
   *  @return a boolean value
   */
  bool getBool(std::string name);

  /**
   *  @brief Get integer from config file
   *
   *  @return an integer
   */
  int getInt(std::string name);

  /**
   *  @brief Get double from config file
   *
   *  @return a double value
   */
  double getDouble(std::string name);
  
 protected:
  
 private:
  /**
   *  @brief load config file and set internal property tree
   */
  void LoadConfigFile(std::string filename);

  /**
   *  @brief display property tree
   */
  void DisplayPTree(const boost::property_tree::ptree& tree, const int depth = 0);

  /**
   *  @brief debug mode
   */
  bool debug_mode_;

  /**
   *  @brief filename
   */
  std::string filename_;

  /**
   *  @brief property tree
   */
  boost::property_tree::ptree ptree_;

}; // class EasyConfig
} // namespace config
} // namespace doocore

#endif // DOOCORE_CONFIG_EASYCONFIG_H
