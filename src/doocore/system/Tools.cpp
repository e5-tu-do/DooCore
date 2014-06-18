#include "Tools.h"

// from STL

// from ROOT

// from RooFit

// from TMVA

// from BOOST
//#if defined(__GNUG__) && !defined(__clang__)
//#define BOOST_NO_CXX11_SCOPED_ENUMS
//#endif
#include "boost/filesystem.hpp"
#include "boost/regex.hpp"

// from DooCore
#include "doocore/io/MsgStream.h"

// from here

// forward declarations

namespace doocore {
namespace system {

namespace tools {

std::pair<std::string, std::string> SeparatePathAndFilename(std::string complete_path){
  bool debug_mode = false;
  if (debug_mode) doocore::io::serr << "-debug- " << "starting SeparatePathAndFilename…" << doocore::io::endmsg;
  
  std::pair<std::string, std::string> path_and_filename;

  boost::regex expr("^(.*/)([^/]*)$");
  boost::match_results<std::string::const_iterator> what;

  if( regex_search( complete_path, what, expr ) ){
    std::string dir( what[1].first, what[1].second );
    std::string filename( what[2].first, what[2].second );    

    path_and_filename.first = dir;
    path_and_filename.second = filename;
  }
  else {
    doocore::io::swarn << "-warning- " << "SeparatePathAndFilename -- RegEx matching failed" << doocore::io::endmsg;
  }
  if (debug_mode) doocore::io::serr << "-debug- \t" << "Path: " << path_and_filename.first << doocore::io::endmsg;
  if (debug_mode) doocore::io::serr << "-debug- \t" << "Filename: " << path_and_filename.second << doocore::io::endmsg;
  return path_and_filename;
}

std::pair<std::string, std::string> SeparateFilenameAndType(std::string complete_filename){
  bool debug_mode = false;
  if (debug_mode) doocore::io::serr << "-debug- " << "starting SeparateFilenameAndType" << doocore::io::endmsg;
  
  std::pair<std::string, std::string> filename_and_type;

  boost::regex expr("^(.*)(\\..*)$");
  boost::match_results<std::string::const_iterator> what;

  if( regex_search( complete_filename, what, expr ) ){
    std::string name( what[1].first, what[1].second );
    std::string type( what[2].first, what[2].second );    

    filename_and_type.first = name;
    filename_and_type.second = type;
  }
  else {
    doocore::io::swarn << "-warning- " << "SeparateFilenameAndType -- RegEx matching failed" << doocore::io::endmsg;
  }
  if (debug_mode) doocore::io::serr << "-debug- \t" << "Name: " << filename_and_type.first << doocore::io::endmsg;
  if (debug_mode) doocore::io::serr << "-debug- \t" << "Extension: " << filename_and_type.second << doocore::io::endmsg;
  return filename_and_type;
}

void RemoveFile(std::string target_file){
  boost::filesystem::remove(target_file);
}

void CopyFileToDirectory(std::string source_file, std::string target_directory){
  bool debug_mode = false;

  boost::filesystem::path source(source_file);
  boost::filesystem::path target_path(target_directory);
  boost::filesystem::path target = target_path / source.filename();

  if (!(boost::filesystem::exists(target_directory))){
    doocore::io::swarn << "-warning- " << "Target directory '" + target_directory + "' does not exists! Create directory...." << doocore::io::endmsg;
    boost::filesystem::create_directories(target_directory);
  }
  boost::filesystem::copy_file(source, target, boost::filesystem::copy_option::overwrite_if_exists);
  if (debug_mode) doocore::io::serr << "-debug- " << "copied file '" << source.filename() << "' to output directory '" << target_directory << "'" << doocore::io::endmsg;
}

void ReplaceFile(std::string source_file, std::string target_file){
  boost::filesystem::copy_file(source_file, target_file, boost::filesystem::copy_option::overwrite_if_exists);
}

void CreateDirectory(std::string target_directory){
  bool debug_mode = false;
  if (debug_mode) doocore::io::serr << "-debug- " << "Create directory '" << target_directory << "'" << doocore::io::endmsg;
  boost::filesystem::create_directories(target_directory);
} 

} // namespace tools
} // namespace system
} // namespace doocore