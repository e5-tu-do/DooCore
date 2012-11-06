#include "doocore/config/Summary.h"

// from STL
#include <cmath>
#include <fstream>
#include <iostream>
#include <stdio.h>

// from ROOT
#include <TString.h>

// from TMVA

// from BOOST

// from DooCore
#include "doocore/io/MsgStream.h"

namespace doocore {
namespace config {
Summary *Summary::instance_ = NULL;

Summary::Summary(){}

Summary::Summary(const Summary&){}

Summary& Summary::GetInstance() {
  if(!instance_){
    instance_ = new Summary();
  }
  return *instance_;
}

void Summary::DestroyInstance(){
  if(instance_){
    delete instance_;
  }
  instance_ = 0;
}

void Summary::Add(TString description, TString argument){
  std::pair< TString,TString > tpair;
  tpair.first = description;
  tpair.second = argument;
  log_.push_back(tpair);
  if(debug_mode_) {doocore::io::sinfo << description << " with value " << argument << " saved to project summary." << doocore::io::endmsg;}
}

void Summary::AddSection(TString name){
  Add("Summary::SECTION", name);
}

void Summary::AddHLine(){
  Add("Summary::HLINE", "");
}

void Summary::Print(){
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
  doocore::io::scfg << "- ==================================================" << doocore::io::endmsg;
  doocore::io::scfg << "" << doocore::io::endmsg;
}

void Summary::Write(TString filename) {
  // to be implemented
}

void Summary::StartClock() {
  // to be implemented
}

void Summary::StopClock() {
  // to be implemented
}

} // namespace config
} // namespace doocore