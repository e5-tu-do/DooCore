#ifndef DOOCORE_IO_TOOLS_H
#define DOOCORE_IO_TOOLS_H

// from STL
#include <string>

// from ROOT

// from RooFit

// from TMVA

// from BOOST

// from here

// forward declarations

namespace doocore {
namespace io {

/*! @namespace doocore::io::tools
 * @brief Collection of io tools
 *
 * This namespace is a collection of tools for io handling
 *
 */
namespace tools {

/**
 *  @brief Find and replace text in a text file
 * 
 * 
 * 
 */
void ReplaceScientificNotationInFile(std::string filename, bool debug_mode=false);

} // namespace tools
} // namespace io
} // namespace doocore


#endif // DOOCORE_IO_TOOLS_H