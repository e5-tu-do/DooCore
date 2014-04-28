#include "doocore/config/EasyConfig.h"

// from STL

// from ROOT

// from RooFit

// from TMVA

// from BOOST
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/info_parser.hpp>
#include <boost/foreach.hpp>

// from DooCore
#include "doocore/io/MsgStream.h"
#include "doocore/config/Summary.h"

namespace doocore {
namespace config {
EasyConfig::EasyConfig(int argc, char *argv[]){
  debug_mode_=false;
  std::string filename = "";
  for (int i = 0; i < argc; ++i)
  {
    if ((strcmp(argv[i], "-c")==0) && (i+1<argc)){
      filename = argv[i+1];
    }
  }
  if (filename!=""){
    LoadConfigFile(filename);
  }
  else{
    doocore::io::serr << "-ERROR- " << "No command line argument passed to EasyConfig!" << doocore::io::endmsg;
    doocore::io::serr << "-ERROR- " << "Use '-c' followed by the config file name as command line argument!" << doocore::io::endmsg;
  }
}

EasyConfig::EasyConfig(std::string filename, bool debug_mode){
  debug_mode_ = debug_mode;
  LoadConfigFile(filename);
}

EasyConfig::~EasyConfig(){
  using namespace doocore::io;
}

void EasyConfig::LoadConfigFile(std::string filename){
  doocore::config::Summary::GetInstance().AddFile(filename);
  if (debug_mode_) doocore::io::sdebug << "Reading config file " << filename << "..." << doocore::io::endmsg;
  filename_ = filename;
  read_info(filename, ptree_);
  
  LoadExternalConfigs(ptree_);
  
  if (debug_mode_) DisplayPTree(ptree_);
}

void EasyConfig::LoadExternalConfigs(boost::property_tree::ptree& tree) {
  using namespace doocore::io;
  using namespace boost::property_tree;
  for (auto element : tree) {
//    boost::property_tree::ptree& sub_tree = element.second;
//boost::property_tree::ptree& sub_tree = tree.get_child(element.first);
    boost::optional<boost::property_tree::ptree&> sub_tree_optional = tree.get_child_optional(element.first);
    if (sub_tree_optional && (*sub_tree_optional).size() > 0) {
      boost::property_tree::ptree& sub_tree = *sub_tree_optional;
      LoadExternalConfigs(sub_tree);
    } else {
      if (element.first == "load_config") {
        std::string filename_config = element.second.data();
        
        if (filename_config.size() > 0) {
          boost::property_tree::ptree new_ptree;
          read_info(filename_config, new_ptree);
          //DisplayPTree(new_ptree);
          
          for (auto element_ext : new_ptree) {
            tree.add_child(element_ext.first, element_ext.second);
          }
        }
      }
    }
  }
}
  
void EasyConfig::DisplayPTree(const boost::property_tree::ptree& tree, const int depth) const {
  using namespace doocore::io;
  for (auto element : tree) {
    boost::property_tree::ptree& subtree = element.second;
    std::string nodestr = element.second.data();
    
    // print current node
    doocore::io::sinfo << std::string("").assign(depth*2,' ') << "- ";
    doocore::io::sinfo << element.first;
    if (nodestr.length() > 0) {
      doocore::io::sinfo << " = \"" << element.second.data() << "\"";
    }
    doocore::io::sinfo << doocore::io::endmsg;
    
    if (subtree.size() > 0) {
      // recursive go down the hierarchy
      DisplayPTree(subtree, depth+1);
    }
  }
}

boost::property_tree::ptree EasyConfig::getPTree(){
  return ptree_;
}

std::string EasyConfig::getString(std::string name) const {
//  std::string tmp = ptree_.get(name, "");
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
//  if (debug_mode_ && (tmp == "")) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
//  return tmp;
  return Get<std::string>(name, "");
}

std::vector<std::string> EasyConfig::getVoStrings(std::string name) const {
//  std::set<std::string> set;
//  std::vector<std::string> vec;
//
//  BOOST_FOREACH(const boost::property_tree::ptree::value_type &t, ptree_.get_child(name))
//  vec.push_back(t.first.data());
//
//  return vec;
  return GetVector<std::string>(name);
}
  
std::vector<std::pair<std::string, std::string> > EasyConfig::getVoStringPairs(std::string name) const {
//  std::vector<std::pair<std::string, std::string> > vec;
//
//  for (const boost::property_tree::ptree::value_type &t : ptree_.get_child(name)) {
//    vec.push_back(std::make_pair(t.first.data(), t.second.data()));
//  }
//  
//  return vec;
  return GetVectorPairs<std::string,std::string>(name);
}


bool EasyConfig::getBool(std::string name) const {
//  bool tmp = ptree_.get(name, false);
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
//  if (debug_mode_ && (tmp == false)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
//  return tmp;
  return Get<bool>(name, false);
}

int EasyConfig::getInt(std::string name) const {
//  int tmp = ptree_.get(name, 0);
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
//  if (debug_mode_ && (tmp == 0)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
//  return tmp;
  return Get<int>(name, 0);
}

double EasyConfig::getDouble(std::string name) const {
//  double tmp = ptree_.get(name, 0.0);
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
//  if (debug_mode_ && (tmp == 0.0)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
//  return tmp;
  return Get<double>(name, 0.0);
}
} // namespace config
} // namespace doocore