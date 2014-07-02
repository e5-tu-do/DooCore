#include "doocore/plot/styles.h"


// from DooCore - IO
#include "doocore/io/MsgStream.h"

// from ROOT
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>

namespace doocore {
namespace plot {
namespace styles {

 
TStyle* CreateLHCbStyle(const TString& style_name, bool force_style_creation) {
  TStyle* style = nullptr;
  // check if style already exists
  if ( !force_style_creation && gROOT->GetListOfStyles()->Contains(style_name) ){
    io::serr << "Cannot create style " << style_name << "! "
             << "Style with this name is already known to gROOT!" << io::endmsg;
    return style;
  } else {
    style = new TStyle(style_name, "Dortmund E5 LHCb style");
  }
  
  //============================================================================
  // Basic options
  
  // Use times-medium-r-normal with precision 2. This allows for scalable and
  // rotatable hardware fonts. Precision 2 (compared to precisions 3) ll text sizes to be
  // interpreted as fractions of the pad size (see TAttText documentation).
  int font_id = 132;
  
  // text size (relative to pad size)
  double text_basic_size = 0.06;

  // basic colour (black for foreground, white for background)
  int fg_basic_colour = 1;
  int bg_basic_colour= 0;
  
  // general line width (pixel)
  double line_basic_width = 2.00;

  //============================================================================
  // general font and text
  style->SetTextFont(font_id);
  style->SetTextSize(text_basic_size);
  
  //============================================================================
  // frame options
  style->SetFrameBorderMode(0);
  style->SetFrameFillColor(bg_basic_colour);

  //============================================================================
  // paper options (related to PostScript output)
  style->SetPaperSize(20,26);
  
  //============================================================================
  // canvas options
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(bg_basic_colour);
  
  //============================================================================
  // pad options
  style->SetPadBorderMode(0);
  style->SetPadColor(bg_basic_colour);
  style->SetPadTopMargin(0.07);
  style->SetPadRightMargin(0.10101010101010101010); // increase for colz plots
  style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.18);
  
  //============================================================================
  // title options
  style->SetTitleStyle(0);
  style->SetTitleBorderSize(0);
  style->SetTitleFillColor(bg_basic_colour);

  style->SetTitleFont(font_id,"title"); // set title font of pad ("title" is arbitrarily chosen)
  style->SetTitleFont(font_id,"x");
  style->SetTitleFont(font_id,"y");
  style->SetTitleFont(font_id,"z");
  
  style->SetTitleSize(1.2*text_basic_size,"x");
  style->SetTitleSize(1.2*text_basic_size,"y");
  style->SetTitleSize(1.2*text_basic_size,"z");
  
  style->SetTitleOffset(0.95,"x");
  style->SetTitleOffset(1.20,"y");
  style->SetTitleOffset(1.20,"z");
  
  style->SetTitleX(0.00);
  style->SetTitleY(1.00);
  
  style->SetTitleW(1.00);
  style->SetTitleH(0.05);
  
  //============================================================================
  // label options
  style->SetLabelFont(font_id,"x");
  style->SetLabelFont(font_id,"y");
  style->SetLabelFont(font_id,"z");
  
  style->SetLabelSize(text_basic_size,"x");
  style->SetLabelSize(text_basic_size,"y");
  style->SetLabelSize(text_basic_size,"z");
  
  style->SetLabelOffset(0.010,"X");
  style->SetLabelOffset(0.005,"Y");
  
  
  //============================================================================
  // stat box options (get rid of it)
  style->SetStatColor(0);
  style->SetStatBorderSize(1);
  style->SetLegendBorderSize(1);

  //============================================================================
  // legend options
  style->SetLegendBorderSize(1);

  //============================================================================
  // fill options (black, solid)
  style->SetFillColor(1);
  style->SetFillStyle(1001);
  
  //============================================================================
  // palette options
  
  
  
  return style;
}
      
} // namespace styles
} // namespace plot
} // namespace doocore