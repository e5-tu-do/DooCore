#include "doocore/config/Summary.h"

// from STL
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>

// from ROOT
#include <TString.h>
#include <TCut.h>

// from TMVA

// from BOOST
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

// from DooCore
#include "doocore/io/MsgStream.h"

namespace doocore {
namespace config {
//Summary *Summary::instance_ = NULL;

Summary::Summary() :
  debug_mode_(false),
  output_directory_("summary")
{}

Summary::Summary(const Summary&){}

Summary& Summary::GetInstance() {
//  if(!instance_){
//    instance_ = new Summary();
//  }
//  return *instance_;
  static Summary instance;
  return instance;
}

//void Summary::DestroyInstance(){
//  if(instance_){
//    delete instance_;
//  }
//  instance_ = 0;
//}

void Summary::Add(TString description, TString argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = argument;
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::Add(TString description, TCut argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = argument.GetName();
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::Add(TString description, bool argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  if (argument){
    tpair.second = "true";
  }
  else{
    tpair.second = "false";
  }
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::Add(TString description, std::string argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = argument;
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::Add(TString description, double argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = boost::lexical_cast<std::string>(argument);
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::Add(TString description, int argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = boost::lexical_cast<std::string>(argument);
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << tpair.first  << " with value " << tpair.second << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::AddSection(TString name){
  Add("Summary::SECTION", name);
}

void Summary::AddHLine(){
  Add("Summary::HLINE", "");
}

void Summary::Print(){
  using namespace doocore::io;
  
  doocore::io::scfg << "- ==================== Summary ====================" << doocore::io::endmsg;
  for(size_t i = 0; i < log_.size(); ++i)
  {
    if (log_.at(i).first == "Summary::SECTION"){
      doocore::io::scfg << "- -------------------- " << log_.at(i).second << " --------------------" << doocore::io::endmsg;
    }
    else if(log_.at(i).first == "Summary::HLINE"){
      doocore::io::scfg << "- --------------------------------------------------" << doocore::io::endmsg;
    }
    else{
      doocore::io::scfg << "--- " << log_.at(i).first << "\r\t\t\t\t" << " : " << log_.at(i).second << doocore::io::endmsg;
    }
  }
  scfg << "The following files are added to the run summary: " << endmsg;
  for (std::set<boost::filesystem::path>::const_iterator it = files_.begin(), end = files_.end(); it != end; ++it) {
    scfg << " " << *it << endmsg;
  }
  doocore::io::scfg << "- ==================================================" << doocore::io::endmsg;
  doocore::io::scfg << "" << doocore::io::endmsg;
}

void Summary::Write(std::string filename) {
  doocore::io::MsgStream fileoutput(doocore::io::kTextBlue, filename);

  fileoutput << "- ==================== Summary ====================" << doocore::io::endmsg;
  for(size_t i = 0; i < log_.size(); ++i)
  {
    if (log_.at(i).first == "Summary::SECTION"){
      fileoutput << "- -------------------- " << log_.at(i).second << " --------------------" << doocore::io::endmsg;
    }
    else if(log_.at(i).first == "Summary::HLINE"){
      fileoutput << "- --------------------------------------------------" << doocore::io::endmsg;
    }
    else{
      fileoutput << "--- " << log_.at(i).first << "\r\t\t\t\t" << " : " << log_.at(i).second << doocore::io::endmsg;
    }
  }
  fileoutput << "- ==================================================" << doocore::io::endmsg;
  fileoutput << "" << doocore::io::endmsg;
}

void Summary::StartClock() {
  // to be implemented
}

void Summary::StopClock() {
  // to be implemented
}
  
void Summary::AddFile(const boost::filesystem::path& file) {
  files_.insert(file);
}

void Summary::CopyFiles() {
  namespace fs = boost::filesystem;
  using namespace doocore::io;
  fs::path dir_output = output_directory_;
  
  if (!fs::exists(dir_output)) {
    fs::create_directories(dir_output);
  }
  
  for (std::set<boost::filesystem::path>::const_iterator it = files_.begin(), end = files_.end(); it != end; ++it) {
    fs::path input  = fs::canonical(*it);
    fs::path target = dir_output / input.filename();
    if (fs::exists(input) && fs::is_regular_file(input)) {
      fs::copy_file(input, target, boost::filesystem::copy_option::overwrite_if_exists);
    } else {
      serr << "Summary::CopyFiles(): Cannot copy " << *it << "!" << endmsg;
    }
  }
  }
} // namespace config
} // namespace doocore