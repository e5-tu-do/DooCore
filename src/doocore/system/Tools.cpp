#include "Tools.h"

// from STL

// from ROOT

// from RooFit

// from TMVA

// from BOOST
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

// from DooCore
#include "doocore/io/MsgStream.h"

// from here

// forward declarations

namespace doocore {
namespace system {

namespace tools {
  
void CreateDirectory(std::string target_directory){
  bool debug_mode = false;
  if (debug_mode) doocore::io::serr << "-debug- " << "Create directory '" << target_directory << "'" << doocore::io::endmsg;
  boost::filesystem::create_directories(target_directory);
}  

void CopyToDirectory(std::string source_file, std::string target_directory){
  bool debug_mode = false;
  boost::regex expr("^(.*/)([^/]*)$");
  boost::match_results<std::string::const_iterator> what;
    
  if( regex_search( source_file, what, expr ) ) {
    std::string dir( what[1].first, what[1].second );
    std::string filename( what[2].first, what[2].second );    

    boost::filesystem::path source(dir + filename);
    boost::filesystem::path target(target_directory + "/" + filename);

    if (!(boost::filesystem::exists(target_directory))){
      doocore::io::swarn << "-warning- " << "Target directory '" + target_directory + "' does not exists! Create directory...." << doocore::io::endmsg;
      boost::filesystem::create_directories(target_directory);
    }
    boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);
    if (debug_mode) doocore::io::serr << "-debug- " << "copied file '" << filename << "' to output directory '" << target_directory << "'" << doocore::io::endmsg;
  }
  else {
    doocore::io::swarn << "-warning- " << "CopyToDirectory -- RegEx matching failed" << doocore::io::endmsg;
  }
}

} // namespace tools
} // namespace system
} // namespace doocore