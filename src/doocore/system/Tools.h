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
 *  @brief Separate path and filename
 *
 *  Given the complete path as input argument, the path and filename are returned as std::pair<std::string, std::string>
 *
 */
std::pair<std::string, std::string> SeparatePathAndFilename(std::string complete_path);

/**
 *  @brief Separate filename and file type extension
 *
 *  Given the complete filename as input argument, the filename and file type extension are returned as std::pair<std::string, std::string>
 *
 */
std::pair<std::string, std::string> SeparateFilenameAndType(std::string complete_filename);

/**
 *  @brief Remove file
 *
 *  Remove file...
 *
 */
void RemoveFile(std::string target_file);

/**
 *  @brief Copy file to given directory
 *
 *  Based on the given filename and target directory, the file will
 *  be copied. If the target directory does not exist, it will be
 *  created.
 *
 */
void CopyFileToDirectory(std::string source_file, std::string target_directory);

/**
 *  @brief Replace target_file by source file
 *
 *
 */
void ReplaceFile(std::string source_file, std::string target_file);

/**
 *  @brief Create directory
 *
 *  Create directory...
 *
 */
void CreateDirectory(std::string target_directory);

} // namespace tools
} // namespace system
} // namespace doocore


#endif // DOOCORE_SYSTEM_TOOLS_H