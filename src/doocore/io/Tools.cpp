#include "Tools.h"

// from STL
#include <iostream>
#include <fstream>
#include <ostream>

// from ROOT

// from RooFit

// from TMVA

// from BOOST
#include "boost/filesystem.hpp"
// #include <boost/xpressive/xpressive.hpp>
#include "boost/regex.hpp"
#include "boost/lexical_cast.hpp"

// from DooCore
#include "doocore/io/MsgStream.h"

// from here

// forward declarations

namespace doocore {
namespace io {

namespace tools {
  
void ReplaceScientificNotationInFiles(std::string filename){
  bool debug_mode = true;

  /// regex
  boost::regex expr("(\\d*)e(\\+|\\-)0+([0-9]*)");
  boost::regex expr2("\\d*e(\\+|\\-)0+[0-9]*");
  boost::match_results<std::string::const_iterator> what;

  /// open input file
  fstream filestr;
  filestr.open(filename.c_str());

  /// create temp file
  fstream tmpfilestr;
  tmpfilestr.open("tmp.txt");

  std::string in_line;
  std::string out_line;

  if (filestr.is_open())
  {
    while ( filestr.good() )
    {
      getline(filestr,in_line);
      std::string zeros;
      std::string replacement;

      if( regex_search( in_line, what, expr ) ) {
        std::string all( what[0].first, what[0].second );
        std::string coefficient( what[1].first, what[1].second );
        std::string sign( what[2].first, what[2].second );
        std::string exponent( what[3].first, what[3].second );
        int coefficient_int(boost::lexical_cast<int>(coefficient));
        int exponent_int(boost::lexical_cast<int>(exponent));

        for (int i = 0; i < exponent_int; ++i){zeros+="0";}
        if (sign=="+"){
          replacement=coefficient+zeros;
        }
        else if (sign=="-"){
          doocore::io::serr << "-ERROR- " << "FUCK!" << doocore::io::endmsg;
        }
        out_line = boost::regex_replace(in_line, expr, replacement);
        if (debug_mode) doocore::io::serr << "-debug- " << "input line: " << in_line << doocore::io::endmsg;
        if (debug_mode) doocore::io::serr << "-debug- " << "output line: " << out_line << doocore::io::endmsg;
        tmpfilestr << out_line << "\n";
      }
      else {
        doocore::io::swarn << "-warning- " << "RegExFindAndReplace -- RegEx matching failed" << doocore::io::endmsg;
      }
    }
  }
  filestr.close();
  tmpfilestr.close();
}

} // namespace tools
} // namespace io
} // namespace doocore