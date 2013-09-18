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

EasyConfig::~EasyConfig(){}

void EasyConfig::LoadConfigFile(std::string filename){
  doocore::config::Summary::GetInstance().AddFile(filename);
  doocore::io::sinfo << "Reading config file " << filename << "..." << doocore::io::endmsg;
  filename_ = filename;
  read_info(filename, ptree_);
  if (debug_mode_) DisplayPTree(ptree_);
}

void EasyConfig::DisplayPTree(const boost::property_tree::ptree& tree, const int depth) {  
 BOOST_FOREACH( boost::property_tree::ptree::value_type const&v, tree.get_child("") ) {  
  boost::property_tree::ptree subtree = v.second;  
  std::string nodestr = tree.get<std::string>(v.first);  
  
  // print current node  
  doocore::io::sinfo << std::string("").assign(depth*2,' ') << "- ";  
  doocore::io::sinfo << v.first;  
  if ( nodestr.length() > 0 )   
    doocore::io::sinfo << " = \"" << tree.get<std::string>(v.first) << "\"";  
  doocore::io::sinfo << doocore::io::endmsg;  
  
  // recursive go down the hierarchy  
  DisplayPTree(subtree, depth+1);  
 }
}

boost::property_tree::ptree EasyConfig::getPTree(){
  return ptree_;
}

std::string EasyConfig::getString(std::string name) const {
  std::string tmp = ptree_.get(name, "");
  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
  if (debug_mode_ && (tmp == "")) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
  return tmp;
}

std::vector<std::string> EasyConfig::getVoStrings(std::string name) const {
  std::set<std::string> set;
  std::vector<std::string> vec;
//  BOOST_FOREACH(boost::property_tree::ptree::value_type &t, ptree_.get_child(name))
//  set.insert(t.first.data());
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Values: " << doocore::io::endmsg;
//  for(std::set<std::string>::iterator it = set.begin(); it != set.end(); it++){
//    if (debug_mode_) doocore::io::swarn << "\t\t\t" << (*it) << doocore::io::endmsg;
//    vec.push_back(*it);
//  }
//  return vec;
  BOOST_FOREACH(const boost::property_tree::ptree::value_type &t, ptree_.get_child(name))
  vec.push_back(t.first.data());
//  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Values: " << doocore::io::endmsg;
//  for(std::set<std::string>::iterator it = set.begin(); it != set.end(); it++){
//    if (debug_mode_) doocore::io::swarn << "\t\t\t" << (*it) << doocore::io::endmsg;
//    vec.push_back(*it);
//  }
  return vec;
}

bool EasyConfig::getBool(std::string name) const {
  bool tmp = ptree_.get(name, false);
  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
  if (debug_mode_ && (tmp == false)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
  return tmp;
}

int EasyConfig::getInt(std::string name) const {
  int tmp = ptree_.get(name, 0);
  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
  if (debug_mode_ && (tmp == 0)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
  return tmp;
}

double EasyConfig::getDouble(std::string name) const {
  double tmp = ptree_.get(name, 0.0);
  if (debug_mode_) doocore::io::swarn << "Key: " << name << ", Value: " << tmp << doocore::io::endmsg;
  if (debug_mode_ && (tmp == 0.0)) doocore::io::swarn << "-warning- \t EasyConfig: " << "Check if variable '" << name << "' is set properly in the config file. Default value is set!" << doocore::io::endmsg;
  return tmp;
}
} // namespace config
} // namespace doocore