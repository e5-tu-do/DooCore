#ifndef DOOCORE_SYSTEM_TOOLS_H
#define DOOCORE_SYSTEM_TOOLS_H

// from STL
#include <string>

// from ROOT

// from RooFit

// from TMVA

// from BOOST

// from here

// forward declarations

namespace doocore {
namespace system {

/*! @namespace doocore::system::tools
 * @brief Collection of system tools
 *
 * This namespace is a collection of tools for filesystem handling
 *
 */
namespace tools {

/**
 *  @brief Create directory
 *
 *  Create directory...
 *
 */
void CreateDirectory(std::string target_directory);

/**
 *  @brief Copy file to given directory
 *
 *  Based on the given filename and target directory, the file will
 *  be copied. If the target directory does not exist, it will be
 *  created.
 *
 */
void CopyToDirectory(std::string source_file, std::string target_directory);

}; // namespace tools
} // namespace system
} // namespace doocore


#endif // DOOCORE_SYSTEM_TOOLS_H