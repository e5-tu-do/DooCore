#ifndef DOOCORE_CONFIG_EASYCONFIG_H
#define DOOCORE_CONFIG_EASYCONFIG_H

// from STL
#include <string>
#include <vector>
#include <sstream>

// from ROOT

// from RooFit

// from TMVA

// from BOOST
#include <boost/property_tree/ptree.hpp>

// from here
#include "doocore/io/MsgStream.h"

// forward declarations

namespace doocore {
/*! @namespace doocore::config
 * @brief Collection of program configuration tools
 *
 * This namespace is a collection of tools for program configuration
 *
 */

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
 *   myvector 
 *   {
 *     one
 *     two
 *     three
 *   }
 *   mypairvector
 *   {
 *     one "un"
 *     two "deux"
 *     three "trois"
 *   }
 *
 *   load_config "external_file_to_parse.cfg"
 * }
 * @endcode
 * 
 * WARNING: If your variable is not set in the config file, a default value will be used!
 * WARNING: The default value is 'false' for bools, '0' for ints, '0.0' for doubles and an emptry string for strings!
 * 
 */
class EasyConfig {
 public:
  /**
   *  @brief Constructor for EasyConfig with interprets given command line arguments
   *
   *  Based on the supplied command line arguments the property tree will be populated
   *  and is ready to use.
   *
   *  To understand a given argument as the string to the option file, use the "-c" before the argument
   *
   *  @param filename file name of config file to use
   */
  EasyConfig(int argc, char *argv[]);

  /**
   *  @brief Constructor for EasyConfig with config file to use
   *
   *  Based on the supplied file name the property tree will be populated
   *  and is ready to use.
   *
   *  @param filename file name of config file to use
   */
  EasyConfig(std::string filename, bool debug_mode=false);
  
  /**
   *  @brief Destructor for EasyConfig
   */
  ~EasyConfig();
  
  /**
   *  @brief Get the underlying property tree
   *
   *  @return a boost::property_tree::ptree
   */
  boost::property_tree::ptree getPTree();

  /**
   *  @brief Get string from config file
   *
   *  @return an std::string
   */
  std::string getString(std::string name) const;

  /**
   *  @brief Get vector of strings from config file
   *
   *  @return an std::vector<std::string>
   */
  std::vector<std::string> getVoStrings(std::string name) const;
  
  /**
   *  @brief Get vector of string pairs from config file
   *
   *  For a given node, this will return all child elements as key and value
   *  in the std::pair. Example for the node pair_elements:
   *
   * @code
   * pair_elements 
   * {
   *   key1 "value1"
   *   key2 "value2"
   * }
   * @endcode
   * 
   *  Here, the vector will then contain ((key1,value1), (key2, value2)).
   *
   *  @return an std::vector<std::pair<std::string,std::string> >
   */
  std::vector<std::pair<std::string, std::string>> getVoStringPairs(std::string name) const;

  /**
   *  @brief Get boolean from config file
   *
   *  @return a boolean value
   */
  bool getBool(std::string name) const;

  /**
   *  @brief Get integer from config file
   *
   *  @return an integer
   */
  int getInt(std::string name) const;

  /**
   *  @brief Get double from config file
   *
   *  @return a double value
   */
  double getDouble(std::string name) const;
  
  /**
   *  @brief Templated function to get any type from config file
   *
   *  @return value or default_value
   */
  template<typename Type>
  Type Get(const std::string& name, Type default_value=Type()) const;
  
  /**
   *  @brief Templated function to get vector for key of any type from config file
   *
   *  @warning As no property_tree translator can be used for the keys, take caution in case you want to use non-string objects as keys.
   *
   *  @return vector for given key
   */
  template<typename Type>
  std::vector<Type> GetVector(const std::string& name) const;
  
  /**
   *  @brief Templated function to get vector of key value pairs for key of any types from config file
   *
   *  @warning As no property_tree translator can be used for the keys, take caution in case you want to use non-string objects as keys.
   *
   *  @return vector for given key
   */
  template<typename KeyType, typename ValueType>
  std::vector<std::pair<KeyType,ValueType>> GetVectorPairs(const std::string& name) const;
 
  /**
   *  @brief Check if key exists
   *
   *  @return whether key exists (true) or not (false)
   */
  bool KeyExists(const std::string& name) const {
    //doocore::io::sinfo << "looking for " << name << ptree_.find(name)->first.data() << doocore::io::endmsg;
    boost::optional<const boost::property_tree::ptree&> child = ptree_.get_child_optional(name);
    return !(!child);
    
//    return (ptree_.find(name) != ptree_.not_found());
//    return (ptree_.count(name) > 0);
  }
  
  /**
   * @brief Set debug mode
   *
   */
  void set_debug_mode(bool debug_mode){debug_mode_ = debug_mode;}
 
  /**
   *  @brief Get filename of config file
   *
   *  @return the filename
   */
  const std::string& filename() const {
    using namespace doocore::io;
    return filename_; 
  }
  
  /**
   *  @brief Print the property tree
   */
  void Print() const { DisplayPTree(ptree_); }
  
 protected:
  
 private:
  /**
   *  @brief load config file and set internal property tree
   */
  void LoadConfigFile(std::string filename);

  /**
   *  @brief display property tree
   */
  void DisplayPTree(const boost::property_tree::ptree& tree, const int depth = 0) const;

  /**
   *  @brief Iterate property tree and check for load_config statements
   *
   *  @param tree tree to check for load_config
   */
  void LoadExternalConfigs(boost::property_tree::ptree& tree);
  
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
  
template<typename Type>
Type EasyConfig::Get(const std::string& name, Type default_value) const {
  Type tmp = ptree_.get<Type>(name, default_value);
  return tmp;
}
  
template<typename Type>
std::vector<Type> EasyConfig::GetVector(const std::string& name) const {
  std::vector<Type> v;
  for (auto t : ptree_.get_child(name)) {
    std::istringstream ss(t.first.data());
    Type el;
    ss >> el;
    v.push_back(el);
  }
  return v;
}
  
template<typename KeyType, typename ValueType>
std::vector<std::pair<KeyType,ValueType>> EasyConfig::GetVectorPairs(const std::string& name) const {
  std::vector<std::pair<KeyType,ValueType>> v;
  for (auto t : ptree_.get_child(name)) {
    std::istringstream ss_key(t.first.data());
    KeyType el_key;
    ss_key >> el_key;
    
    const boost::property_tree::ptree& sec = t.second;
    v.push_back(std::make_pair(el_key,sec.get_value<ValueType>()));
  }
  return v;
}

} // namespace config
} // namespace doocore

#endif // DOOCORE_CONFIG_EASYCONFIG_H
