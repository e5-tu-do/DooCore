#ifndef DOOCORE_PLOT_STYLES_H
#define DOOCORE_PLOT_STYLES_H

// from STL
#include <string>

// from ROOT
#include <TString.h>

// forward declarations
class TStyle;


namespace doocore {
namespace plot {
namespace styles {

/**
 *  @brief Creates the standard style for LHCb plots at E5 (Dortmund).
 *
 *  This function creates a standard style called "LHCbStyle" within ROOTs
 *  gROOT list of styles. If a style with this name already exists, an error
 *  message will be printed and the style in gROOT will remain.
 */
TStyle* CreateLHCbStyle(const TString& style_name = "LHCbStyle",
                        bool force_style_creation = false);

TStyle* CreateLHCbLogyStyle(const TString& style_name = "LHCbLogyStyle",
                           bool force_style_creation = false);
  
} // namespace styles
} // namespace plot
} // namespace doocore

#endif // DOOCORE_PLOT_STYLES_H