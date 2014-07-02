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
    style = dynamic_cast<TStyle*>(gROOT->GetListOfStyles()->FindObject(style_name));
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
  double basic_text_size = 0.06;

  // basic colour (black for foreground, white for background)
  int basic_fg_colour = 1;
  int basic_bg_colour= 0;
  
  // general line width (pixel)
  double basic_line_width = 2.00;

  //============================================================================
  // general font and text
  style->SetTextFont(font_id);
  style->SetTextSize(basic_text_size);
  
  //============================================================================
  // frame options
  style->SetFrameBorderMode(0);
  style->SetFrameFillColor(basic_bg_colour);
  style->SetFrameLineWidth(basic_line_width);

  //============================================================================
  // paper options (related to PostScript output)
  style->SetPaperSize(20,26);
  
  //============================================================================
  // canvas options
  style->SetCanvasBorderMode(0);
  style->SetCanvasColor(basic_bg_colour);
  
  style->SetOptStat(0);  // no stat box
  style->SetOptTitle(0); // no title
  style->SetOptFit(0);   // no fit parameters
  style->SetOptDate(0);  // no date
  
  //============================================================================
  // pad options
  style->SetPadBorderMode(0);
  style->SetPadColor(basic_bg_colour);
  style->SetPadTopMargin(0.07);
  style->SetPadRightMargin(0.10101010101010101010); // increase for colz plots
  style->SetPadBottomMargin(0.16);
  style->SetPadLeftMargin(0.18);
  
  //============================================================================
  // tick options
  style->SetPadTickX(1); // put tick marks on top
  style->SetPadTickY(1); // put tick marks on rhs
  
  style->SetTickLength(0.03,"x");
  style->SetTickLength(0.03,"y");
  style->SetTickLength(0.03,"z");

  //============================================================================
  // grid options
  style->SetPadGridX(false); // no grid in x
  style->SetPadGridY(false); // no grid in y
  
  style->SetGridWidth(basic_line_width);
  style->SetGridColor(basic_fg_colour);
  
  //============================================================================
  // title options
  style->SetTitleStyle(0);
  style->SetTitleBorderSize(0);
  style->SetTitleTextColor(basic_fg_colour);
  style->SetTitleFillColor(basic_bg_colour);
  
  style->SetTitleFont(font_id,"title"); // set title font of pad ("title" is arbitrarily chosen)
  style->SetTitleFont(font_id,"x");
  style->SetTitleFont(font_id,"y");
  style->SetTitleFont(font_id,"z");
  
  style->SetTitleSize(1.2*basic_text_size,"x");
  style->SetTitleSize(1.2*basic_text_size,"y");
  style->SetTitleSize(1.2*basic_text_size,"z");
  
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
  
  style->SetLabelSize(basic_text_size,"x");
  style->SetLabelSize(basic_text_size,"y");
  style->SetLabelSize(basic_text_size,"z");
  
  style->SetLabelOffset(0.010,"X");
  style->SetLabelOffset(0.005,"Y");
  
  
  //============================================================================
  // stat box options
  style->SetStatColor(0);
  style->SetStatBorderSize(0);
  style->SetStatFont(font_id);
  style->SetStatFontSize(0.05);
  style->SetStatFormat("6.3g");
  style->SetStatX(0.9);
  style->SetStatY(0.9);
  style->SetStatW(0.25);
  style->SetStatH(0.15);
  
  //============================================================================
  // legend options
  style->SetLegendBorderSize(1);

  //============================================================================
  // fill options (black, solid)
  style->SetFillColor(1);
  style->SetFillStyle(1001);
  
  //============================================================================
  // palette options: Use violet -> red palette with 50 colours
  style->SetPalette(1,0);
  
  //============================================================================
  // histogram options
  style->SetHistLineColor(basic_fg_colour);
  style->SetHistLineWidth(basic_line_width);
  style->SetHistLineStyle(0);
  
  style->SetHistFillColor(basic_bg_colour);
  style->SetHistFillStyle(0);
  
  style->SetHistMinimumZero(false);
  style->SetHistTopMargin(0.05);
  
  // hist divisions: only 5 in x to avoid label overlaps
  style->SetNdivisions(505,"x");
  style->SetNdivisions(510,"y");
  
  //============================================================================
  // marker options
  style->SetMarkerStyle(20);
  style->SetMarkerSize(1.0);
  style->SetMarkerColor(basic_fg_colour);
  
  style->SetErrorX(0.);      // percent of bin width for x uncertainty
  style->SetEndErrorSize(2); // size in pixel of error bars
  
  //============================================================================
  // function options
  style->SetFuncColor(basic_fg_colour);
  style->SetFuncWidth(basic_line_width);
  

  
  io::sinfo << "Style " << style_name
            << " created. To use it call gROOT->SetStyle(\""
            << style_name << "\") or gROOT->ForceStyle(\"" << style_name
            << "\") to enforce the style to all canvases." << io::endmsg;
  
  return style;
}
      
} // namespace styles
} // namespace plot
} // namespace doocore