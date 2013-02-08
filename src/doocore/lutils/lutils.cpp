#include "doocore/lutils/lutils.h"

// from STL
#include <iostream>
#include <fstream>
#include <cmath>

// from POSIX/UNIX
#include <sys/stat.h>

// from BOOST

// from ROOT
#include "TROOT.h"
#include "TStyle.h"
#include "TColor.h"
#include "TH1.h"
#include "TH2.h"
#include "TH2D.h"
#include "TPaveText.h"
#include "TCanvas.h"
#include "TTree.h"
#include "TFile.h"
#include "TLegend.h"

// from RooFit
#include "RooPlot.h"
#include "RooDataSet.h"
#include "RooAbsRealLValue.h"
#include "RooArgSet.h"
#include "RooRealVar.h"
#include "RooHist.h"
#include "RooLinkedListIter.h"

// from project
#include "doocore/io/MsgStream.h"

using namespace std;
using namespace doocore::io;

/*
 * Set global layout
 *
 *  option = "", "2d"
 */
void doocore::lutils::setStyle(TString option)
{
	if (option == "LHCb") {
		// Use times new roman, precision 2
	  Int_t lhcbFont        = 132;  // Old LHCb style: 62;
	  // Line thickness
	  Double_t lhcbWidth    = 2.00; // Old LHCb style: 3.00;
	  // Text size
	  Double_t lhcbTSize    = 0.06;
    
	  // use plain black on white colors
	  gROOT->SetStyle("Plain");
	  TStyle *lhcbStyle= new TStyle("lhcbStyle","LHCb plots style");
    
	  //lhcbStyle->SetErrorX(0); //  don't suppress the error bar along X
    
	  lhcbStyle->SetFillColor(1);
	  lhcbStyle->SetFillStyle(1001);   // solid
	  lhcbStyle->SetFrameFillColor(0);
	  lhcbStyle->SetFrameBorderMode(0);
	  lhcbStyle->SetPadBorderMode(0);
	  lhcbStyle->SetPadColor(0);
	  lhcbStyle->SetCanvasBorderMode(0);
	  lhcbStyle->SetCanvasColor(0);
	  lhcbStyle->SetStatColor(0);
	  lhcbStyle->SetLegendBorderSize(0);
    
	  // If you want the usual gradient palette (blue -> red)
	  lhcbStyle->SetPalette(1);
	  // If you want colors that correspond to gray scale in black and white:
	  int colors[8] = {0,5,7,3,6,2,4,1};
	  lhcbStyle->SetPalette(8,colors);
    
	  // set the paper & margin sizes
	  lhcbStyle->SetPaperSize(20,26);
	  lhcbStyle->SetPadTopMargin(0.05);
	  lhcbStyle->SetPadRightMargin(0.05); // increase for colz plots
	  lhcbStyle->SetPadBottomMargin(0.16);
	  lhcbStyle->SetPadLeftMargin(0.14);
    
	  // use large fonts
	  lhcbStyle->SetTextFont(lhcbFont);
	  lhcbStyle->SetTextSize(lhcbTSize);
	  lhcbStyle->SetLabelFont(lhcbFont,"x");
	  lhcbStyle->SetLabelFont(lhcbFont,"y");
	  lhcbStyle->SetLabelFont(lhcbFont,"z");
	  lhcbStyle->SetLabelSize(lhcbTSize,"x");
	  lhcbStyle->SetLabelSize(lhcbTSize,"y");
	  lhcbStyle->SetLabelSize(lhcbTSize,"z");
	  lhcbStyle->SetTitleFont(lhcbFont);
	  lhcbStyle->SetTitleFont(lhcbFont,"x");
	  lhcbStyle->SetTitleFont(lhcbFont,"y");
	  lhcbStyle->SetTitleFont(lhcbFont,"z");
	  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"x");
	  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"y");
	  lhcbStyle->SetTitleSize(1.2*lhcbTSize,"z");
    
	  // use medium bold lines and thick markers
	  lhcbStyle->SetLineWidth(lhcbWidth);
	  lhcbStyle->SetFrameLineWidth(lhcbWidth);
	  lhcbStyle->SetHistLineWidth(lhcbWidth);
	  lhcbStyle->SetFuncWidth(lhcbWidth);
	  lhcbStyle->SetGridWidth(lhcbWidth);
	  lhcbStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
	  lhcbStyle->SetMarkerStyle(20);
	  lhcbStyle->SetMarkerSize(1.0);
    
	  // label offsets
	  lhcbStyle->SetLabelOffset(0.010,"X");
	  lhcbStyle->SetLabelOffset(0.010,"Y");
    
	  // by default, do not display histogram decorations:
	  lhcbStyle->SetOptStat(0);
	  //lhcbStyle->SetOptStat("emr");  // show only nent -e , mean - m , rms -r
	  // full opts at http://root.cern.ch/root/html/TStyle.html#TStyle:SetOptStat
	  lhcbStyle->SetStatFormat("6.3g"); // specified as c printf options
	  lhcbStyle->SetOptTitle(0);
	  lhcbStyle->SetOptFit(0);
	  //lhcbStyle->SetOptFit(1011); // order is probability, Chi2, errors, parameters
	  //titles
	  lhcbStyle->SetTitleOffset(0.95,"X");
	  lhcbStyle->SetTitleOffset(0.95,"Y");
	  lhcbStyle->SetTitleOffset(1.2,"Z");
	  lhcbStyle->SetTitleFillColor(0);
	  lhcbStyle->SetTitleStyle(0);
	  lhcbStyle->SetTitleBorderSize(0);
	  lhcbStyle->SetTitleFont(lhcbFont,"title");
	  lhcbStyle->SetTitleX(0.0);
	  lhcbStyle->SetTitleY(1.0);
	  lhcbStyle->SetTitleW(1.0);
	  lhcbStyle->SetTitleH(0.05);
    
	  // look of the statistics box:
	  lhcbStyle->SetStatBorderSize(0);
	  lhcbStyle->SetStatFont(lhcbFont);
	  lhcbStyle->SetStatFontSize(0.05);
	  lhcbStyle->SetStatX(0.9);
	  lhcbStyle->SetStatY(0.9);
	  lhcbStyle->SetStatW(0.25);
	  lhcbStyle->SetStatH(0.15);
    
	  // put tick marks on top and RHS of plots
	  lhcbStyle->SetPadTickX(1);
	  lhcbStyle->SetPadTickY(1);
    
	  // histogram divisions: only 5 in x to avoid label overlaps
	  lhcbStyle->SetNdivisions(505,"x");
	  lhcbStyle->SetNdivisions(510,"y");
    
	  gROOT->SetStyle("lhcbStyle");
	  gROOT->ForceStyle();
	}
	
	else {
  	gROOT->SetStyle("Plain");
  	
		// text font
		int font = 132;
		gStyle->SetTitleFont(font, "xyz");	// set the all 3 axes title font
		gStyle->SetTitleFont(font, " ");	// set the pad title font
		gStyle->SetLabelFont(font, "xyz");
		gStyle->SetTextFont(font);
		gStyle->SetStatFont(font);
  	
		// no default boxes
		gStyle->SetOptStat(0);
		gStyle->SetOptTitle(0);
		gStyle->SetOptFit(0);
		
		// rainbow colors in 2d plots
		gStyle->SetPalette(1);
  	
		// use plain black on white colors
		gStyle->SetFrameBorderMode(0);
		gStyle->SetFrameFillColor(0);
		gStyle->SetCanvasBorderMode(0);
		gStyle->SetPadBorderMode(0);
		gStyle->SetPadColor(0);
		gStyle->SetCanvasColor(0);
		gStyle->SetStatColor(0);
		gStyle->SetTitleFillColor(0);
  	
  	/// \todo Put some more documentation in here.
  	
		// canvas default size
		gStyle->SetCanvasDefH(480);
		gStyle->SetCanvasDefW(640);
		
		// text
		gStyle->SetTextSize(0.07);
		gStyle->SetTextAlign(13);
		
		// set the paper & margin sizes Changed the values to look a bit nicer TOBI 20120823 does not work properly yet
		//gStyle->SetPadTopMargin(0.08);
		//gStyle->SetPadRightMargin(0.10);
		//gStyle->SetPadLeftMargin(0.17);
		//gStyle->SetPadBottomMargin(0.15);
		//if ( option=="2d" ) gStyle->SetPadBottomMargin(0.15);
		//gStyle->SetPadLeftMargin(0.14);
		//if ( option=="2d" ) gStyle->SetPadRightMargin(0.18);
		
		//old margins
		gStyle->SetPadTopMargin(0.05);
		gStyle->SetPadRightMargin(0.05);
		gStyle->SetPadBottomMargin(0.14);
		if ( option=="2d" ) gStyle->SetPadBottomMargin(0.15);
		gStyle->SetPadLeftMargin(0.14);
		if ( option=="2d" ) gStyle->SetPadRightMargin(0.18);
  	
		// use bold lines and markers
		gStyle->SetMarkerStyle(8);
		gStyle->SetHistLineWidth(2);
		gStyle->SetLineStyleString(2,"[12 12]"); // postscript dashes
		
		// get rid of X error bars
		gStyle->SetErrorX(0.001);
  	
		// axes
		gStyle->SetTitleSize(0.065,	"xyz");
		gStyle->SetLabelSize(0.06,	"xyz");
		gStyle->SetTitleOffset(1.07,	"y");
		gStyle->SetLabelOffset(0.015,	"x");
		gStyle->SetNdivisions(507,	"x");
		
		// put tick marks on top and right hand side of plots
		//if ( option!="2d" )
		//{
    gStyle->SetPadTickX(1);
    gStyle->SetPadTickY(1);
		//}
		
	}
}


/*
 * Set a 2d color palette which emphasizes the region
 * around 0. Positive numbers are printed reddish ("hot"),
 * negative numbers blueish ("cold").
 *
 * Note: It is important to use a symmetric z-axis range!
 * The axis is adjusted automatically when the histogram
 * is given as an argument.
 */
void doocore::lutils::setHotColdPalette(TH2* h)
{
	const Int_t NRGBs = 5;
	const Int_t NCont = 255;

	Double_t stops[NRGBs] = { 0,0.4,0.5,0.6,1 };
	Double_t red[NRGBs]   = { 0,0,1,1,1 };
	Double_t green[NRGBs] = { 1,0,1,0,1 };
	Double_t blue[NRGBs]  = { 1,1,1,0,0 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
	gStyle->SetPaintTextFormat(".1f");

	if ( h )
	{
		float max = h->GetBinContent(h->GetMaximumBin());
		float min = h->GetBinContent(h->GetMinimumBin());
		float range = Max(max, min);
		h->SetAxisRange(-range, range, "Z");
	}
}

void doocore::lutils::setRedBluePalette(TH2* h)
{
	const Int_t NRGBs = 5;
	const Int_t NCont = 255;

	Double_t stops[NRGBs] = { 0,  0.25, 0.5,  0.75, 1 };
	Double_t red[NRGBs]   = { 0,  0,    1,    1,    0.5 };
	Double_t green[NRGBs] = { 0,  0,    1,    0,    0 };
	Double_t blue[NRGBs]  = { 0.5,1,    1,    0,    0 };
	TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
	gStyle->SetNumberContours(NCont);
	gStyle->SetPaintTextFormat(".1f");

	if ( h )
	{
		float max = h->GetBinContent(h->GetMaximumBin());
		float min = h->GetBinContent(h->GetMinimumBin());
		float range = Max(max, min);
		h->SetAxisRange(-range, range, "Z");
	}
}


void doocore::lutils::drawDate(float x1, float y1, float x2, float y2)
{
	TDatime timer;
	TPaveText *pt = new TPaveText(x1, y1, x2, y2,"BRNDC");
	pt->SetBorderSize(0);
	pt->SetFillStyle(0);
	pt->SetTextAlign(13);
	pt->SetTextSize(0.03);
	pt->SetTextFont(42);
	pt->SetTextColor(kGray);
	pt->AddText(timer.AsString());
	pt->Draw();
}


void doocore::lutils::printSystemRecources(TString cmd)
{
	cout << endl;
	system("ps aux|head -n 1");
	system("ps aux|grep " + cmd + " |grep -v grep");
	cout << endl;
}


void doocore::lutils::printPlot(TCanvas* c, TString name, TString dir)
{
  sinfo << "doocore::lutils::printPlot(...): printing plots..." << endmsg;

  if ( dir!="" && !dir.EndsWith("/") ) dir += "/";

	system("mkdir -p " + dir+"eps/");
	system("mkdir -p " + dir+"C/");
	system("mkdir -p " + dir+"png/");
	system("mkdir -p " + dir+"pdf/");

  c->Print(dir+"eps/" + name + ".eps");
	c->Print(dir+"C/"   + name + ".C");
  c->Print(dir+"png/" + name + ".png");
  c->Print(dir+"pdf/" + name + ".pdf");
}
  
void doocore::lutils::printPlotOpenStack(TCanvas* c, TString name, TString dir)
{
  cout << "doocore::lutils::printPlot() : printing plots ..." << endl;
  
  if ( dir!="" && !dir.EndsWith("/") ) dir += "/";
  
  //	system("mkdir -p " + dir+"eps/");
  system("mkdir -p " + dir+"pdf/");
  
  //  c->Print(dir+"eps/" + name + ".eps");
  c->Print(dir+"pdf/" + name + ".pdf[");
}

void doocore::lutils::printPlotCloseStack(TCanvas* c, TString name, TString dir)
{
  cout << "doocore::lutils::printPlot() : printing plots ..." << endl;
  
  if ( dir!="" && !dir.EndsWith("/") ) dir += "/";
  
  //	system("mkdir -p " + dir+"eps/");
  system("mkdir -p " + dir+"pdf/");
  
  //  c->Print(dir+"eps/" + name + ".eps");
  c->Print(dir+"pdf/" + name + ".pdf]");
}

/* 
 * doesn't work with 3GB files.
 *
 */
bool doocore::lutils::fileExists( TString strFilename )
{
	struct stat stFileInfo;
	bool blnReturn;
	int intStat;

	// Attempt to get the file attributes
	intStat = stat( strFilename, &stFileInfo );
	
	if(intStat == 0) {
		// We were able to get the file attributes
		// so the file obviously exists.
		blnReturn = true;
	} else {
		// We were not able to get the file attributes.
		// This may mean that we don't have permission to
		// access the folder which contains this file. If you
		// need to do that level of checking, lookup the
		// return values of stat which will give you
		// more details on why stat failed.
		blnReturn = false;
	}

	return(blnReturn);
}


int doocore::lutils::fileNLines( TString strFilename )
{
	if ( ! fileExists(strFilename) )
	{
		cout << "FileNLines() : ERROR : file not found : " + strFilename << endl;
		return 0;
	}

	ifstream file(strFilename);
	int n = 0;
	string tmpstr;
	while(!file.eof())
	{
		getline(file, tmpstr);
		n++;
	}
	file.close();
	return n-1;
}

void doocore::lutils::Sleep(double sleep_time) {
  boost::this_thread::sleep(boost::posix_time::microseconds(sleep_time*1e6));
}

/*
 * round doubles
 */
double doocore::lutils::Round(double value, unsigned int digits)
{
	value *= pow(10., (int)digits);
	if (value >= 0)
		value = floor(value + 0.5);
	else
		value = ceil(value - 0.5);
	value /= pow(10., (int)digits);
	return value;
}


/*
 * Calculate the covariance between two variables.
 */
Double_t doocore::lutils::Covariance(Int_t n, Double_t xdata[], Double_t ydata[])
{
	if ( n==0 )
	{
		cout << "doocore::lutils::Covariance() : ERROR : No datapoints! Returning 0." << endl;
		return 0.0;
	}
	
	Double_t sumx(0), sumy(0), sumxy(0);

	for(Int_t i= 0; i < n; i++)
	{
		//cout << xdata[i] << " " << ydata[i] << endl;
		sumx  += xdata[i];
		sumy  += ydata[i];
		sumxy += xdata[i]*ydata[i];
	}
	
	Double_t v = sumxy/(float)n - sumx/(float)n*sumy/(float)n;

	if ( v<0.0 )
	{
		cout << "doocore::lutils::Covariance() : ERROR : Variance < 0. Returning 0." << endl;
		return 0.0;
	}	

	return v;
}


/*
 * Calculate the variance of a dataset.
 */
Double_t doocore::lutils::Variance(Int_t n, Double_t data[])
{
	return Covariance(n, data, data);
}


/*
 * Given up to four histograms, draw the largest one first, then
 * the smaller ones upon it in order. This is to avoid the clash with the "same"
 * option of TH1::Draw().
 */
void doocore::lutils::drawOrdered(TH1* h1, TH1* h2, TH1* h3, TH1* h4)
{
	TH1* h[4];
	h[0] = h1;
	h[1] = h2;
	h[2] = h3;
	h[3] = h4;
	
	int nmax = 2;
	if ( h3!=0 ) nmax++;
	if ( h4!=0 ) nmax++;
	
	// copy over axes because the plot will have the axes from
	// the histogram we draw first
	for ( int i=1; i<nmax; i++ )
	{
		h[i]->GetXaxis()->SetTitle(h[0]->GetXaxis()->GetTitle());
		h[i]->GetYaxis()->SetTitle(h[0]->GetYaxis()->GetTitle());
	}	
	
	int ord[4] = {0};

	for ( int i=0; i<nmax; i++ )
	{
		int greaterThan = 0;
		
		for ( int j=0; j<nmax; j++ )
		{
			if ( h[i]->GetMaximum() > h[j]->GetMaximum() ) greaterThan++;
		}
		
		ord[nmax-1-greaterThan] = i;
	}
	
	// don't use up all plot space
	h[ord[0]]->GetYaxis()->SetRangeUser(0., h[ord[0]]->GetMaximum()*1.3);
	
	for ( int i=0; i<nmax; i++ )
	{
		cout << "doocore::lutils::DrawOrdered() : Drawing " << h[ord[i]]->GetName() << " ..." << endl;
		if ( i==0 ) h[ord[i]]->Draw();
		else        h[ord[i]]->Draw("same");
	}
}

/*
 * Given up to four histograms, draw the largest one first, then
 * the smaller ones upon it in order. This is to avoid the clash with the "same"
 * option of TH1::DrawNormalized().
 */
void doocore::lutils::drawNormalizedOrdered(TH1* h1, TH1* h2, TH1* h3, TH1* h4)
{
	TH1* h[4];
	h[0] = h1;
	h[1] = h2;
	h[2] = h3;
	h[3] = h4;
	
	int nmax = 2;
	if ( h3!=0 ) nmax++;
	if ( h4!=0 ) nmax++;
	
	// copy over axes because the plot will have the axes from
	// the histogram we draw first
	for ( int i=1; i<nmax; i++ )
	{
		h[i]->GetXaxis()->SetTitle(h[0]->GetXaxis()->GetTitle());
		h[i]->GetYaxis()->SetTitle(h[0]->GetYaxis()->GetTitle());
	}	
	
	int ord[4] = {0};

	for ( int i=0; i<nmax; i++ )
	{
		int greaterThan = 0;
		
		for ( int j=0; j<nmax; j++ )
		{
			if ( h[i]->GetMaximum() > h[j]->GetMaximum() ) greaterThan++;
		}
		
		ord[nmax-1-greaterThan] = i;
	}
	
	// don't use up all plot space
	h[ord[0]]->GetYaxis()->SetRangeUser(0., h[ord[0]]->GetMaximum()*1.3);
	
	for ( int i=0; i<nmax; i++ )
	{
		cout << "doocore::lutils::DrawNormalizedOrdered() : Drawing " << h[ord[i]]->GetName() << " ..." << endl;
		if ( i==0 ) h[ord[i]]->DrawNormalized();
		else        h[ord[i]]->DrawNormalized("same");
	}
}

/*
 * Given a vector of histogram pointers, draw the largest one first, then
 * the smaller ones upon it in order. This is to avoid the clash with the "same"
 * option of TH1::DrawNormalized().
 */
void doocore::lutils::drawNormalizedOrdered(std::vector<TH1*> hists)
{
	int nhists = hists.size();
	doocore::io::serr << "-debug- " << "#hists " << nhists << doocore::io::endmsg;
	std::vector<int> ord(nhists);

	// copy over axes because the plot will have the axes from
	// the histogram we draw first
	for ( int i=1; i<nhists; i++ )
	{
		hists[i]->GetXaxis()->SetTitle(hists[0]->GetXaxis()->GetTitle());
		hists[i]->GetYaxis()->SetTitle(hists[0]->GetYaxis()->GetTitle());
	}	
	
	// int ord[nhists] = {0};

	for ( int i=0; i<nhists; i++ )
	{
		int greaterThan = 0;
		
		for ( int j=0; j<nhists; j++ )
		{
			if ( hists[i]->GetMaximum() > hists[j]->GetMaximum() ) greaterThan++;
		}
		
		ord[nhists-1-greaterThan] = i;
	}
	
	// don't use up all plot space
	hists[ord[0]]->GetYaxis()->SetRangeUser(0., hists[ord[0]]->GetMaximum()*1.3);
	
	for ( int i=0; i<nhists; i++ )
	{
		cout << "doocore::lutils::DrawNormalizedOrdered() : Drawing " << hists[ord[i]]->GetName() << " ..." << endl;
		if ( i==0 ) hists[ord[i]]->DrawNormalized();
		else        hists[ord[i]]->DrawNormalized("same");
	}
}

/*
 * Symmetrize a matrix by copying the upper left triangle
 * to the lower right one, which needs to be entirely filled
 * with zeroes.
 */
void doocore::lutils::Symmetrize(TMatrixD &m)
{
	if ( m.GetNrows()!=m.GetNcols() )
	{
		cout << "doocore::lutils::Symmetrize() : ERROR : Matrix not quadratic!" << endl;
		return;
	}
	
	for ( int i=0;   i<m.GetNrows(); i++ )
	for ( int j=i+1; j<m.GetNcols(); j++ )
	{
		if ( m[j][i]!=0.0 )
		{
			cout << "doocore::lutils::Symmetrize() : ERROR : Matrix element is non-zero!" << endl;
			return;
		}
		m[j][i] = m[i][j];
	}
}


/*
 * Add axis labels to the IncPhi phase space histograms. Sure this is
 * somewhere else, too, but I have no idea where. TMK.
 */
void doocore::lutils::addEtaPtLabels(TH2D* h)
{
	if ( !h ) return;
	h->GetXaxis()->SetTitle("p_{T} [MeV/c^{2}]");
	h->GetYaxis()->SetTitle("y");
}

void doocore::lutils::PlotSimple(TString pName, RooPlot * pFrame, const RooAbsRealLValue * pVar, TString pDir, bool plot_logy, TLatex label, bool plot_logx) {
//	setStyle();
  gStyle->SetTitle(0);
  
  // some global definitions
  double pad_border       = 0.02;
  double pad_relysplit    = 0.00;
  double left_margin      = 0.16;
  double top_label_size   = 0.06;
  double top_title_offset = 1.2;
  double title2label_size_ratio = 1.1;
  
  // derived definitions
  double pad_ysplit     = (1.0-2.*pad_border)*pad_relysplit;
  double bottom_label_size = top_label_size*(1.-pad_relysplit)/pad_relysplit;
  double bottom_title_offset = top_title_offset/(1.-pad_relysplit)*pad_relysplit;
  
  double plot_min = pFrame->GetXaxis()->GetXmin();
  double plot_max = pFrame->GetXaxis()->GetXmax();
  
  TCanvas c1("c1","c1",900,900);
  TPad* pad = (TPad*)c1.cd();
  label.Draw();
  if(plot_logy){
    pad->SetLogy(1);
  }
  if (plot_logx) {
    pad->SetLogx(1);
  }
  pad->SetPad(pad_border,pad_ysplit,1.-pad_border,1.-pad_border);
  pad->SetLeftMargin(left_margin);
  pad->SetBottomMargin(left_margin);

//  pFrame->SetLabelSize(0.0,"x");
  pFrame->SetLabelSize(top_label_size,"y");
  pFrame->SetTitleSize(top_label_size*title2label_size_ratio,"y");
  pFrame->GetYaxis()->SetTitleOffset(top_title_offset);
  
	pFrame->Draw();
            
  pad = (TPad*)c1.cd(0);
  label.SetTextSize(0.05);
  label.Draw();
  
  printPlot(&c1, pName, pDir);
}
  
void doocore::lutils::PlotResiduals(TString pName, RooPlot * pFrame, const RooAbsRealLValue * pVar, RooAbsPdf * pPDF, 
                          TString pDir, bool normalize, bool plot_logy,
                          TLatex label, bool plot_logx
                          ) {
	setStyle();
  gStyle->SetTitle(0);
  
  // some global definitions
  double pad_border       = 0.02;
  double pad_relysplit    = 0.3;
  double left_margin      = 0.16;
  double top_label_size   = 0.06;
  double top_title_offset = 1.2;
  double title2label_size_ratio = 1.1;
  
  // derived definitions
  double pad_ysplit     = (1.0-2.*pad_border)*pad_relysplit;
  double bottom_label_size = top_label_size*(1.-pad_relysplit)/pad_relysplit;
  double bottom_title_offset = top_title_offset/(1.-pad_relysplit)*pad_relysplit;
  
  double plot_min = pFrame->GetXaxis()->GetXmin();
  double plot_max = pFrame->GetXaxis()->GetXmax();
  
  TCanvas c1("c1","c1",900,900);
  c1.Divide(1,2);
  
  TPad* pad = (TPad*)c1.cd(1);
  label.Draw();
  if(plot_logy){
    pad->SetLogy(1);
  }
  if (plot_logx) {
    pad->SetLogx(1);
  }
  pad->SetPad(pad_border,pad_ysplit,1.-pad_border,1.-pad_border);
  pad->SetLeftMargin(left_margin);
  pad->SetBottomMargin(0.);

	RooHist* resid = pFrame->residHist(0,0,normalize);
  pFrame->SetLabelSize(0.0,"x");
  pFrame->SetLabelSize(top_label_size,"y");
  pFrame->SetTitleSize(top_label_size*title2label_size_ratio,"y");
  pFrame->SetXTitle("");
  pFrame->GetYaxis()->SetTitleOffset(top_title_offset);
  //pFrame->SetMaximum(1.3*pFrame->GetMaximum());
  
	pFrame->Draw();
          
  // lower frame with residuals plot
  pad = (TPad*)c1.cd(2);
  pad->SetPad(pad_border,pad_border,1.-pad_border,pad_ysplit);
  pad->SetLeftMargin(left_margin);
  pad->SetTopMargin(0.);
  pad->SetBottomMargin(0.4);
  if (plot_logx) {
    pad->SetLogx(1);
  }
  RooPlot * residFrame = pVar->frame(RooFit::Title("Residuals"));
  
  residFrame->GetXaxis()->SetLimits(plot_min,plot_max);
  
  residFrame->addPlotable(resid, "P");
  residFrame->SetAxisRange(-5.8,5.8,"Y");
  
  residFrame->SetTitle("");
  if (normalize){
    residFrame->SetYTitle("Pull");
  } else {
    residFrame->SetYTitle("Residual");
  }
  residFrame->SetLabelSize(bottom_label_size, "xy");
  residFrame->SetTitleSize(bottom_label_size*title2label_size_ratio, "xy");
  residFrame->GetYaxis()->SetTitleOffset(bottom_title_offset);  
  residFrame->GetYaxis()->SetNdivisions(5,5,0);
  residFrame->Draw();
  
  pad = (TPad*)c1.cd(0);
  label.SetTextSize(0.05);
  label.Draw();
  
  printPlot(&c1, pName, pDir);
  
  // residFrame will also delete resid, as it is owned after RooPlot::addPlotable(...)
  delete residFrame;
}

void doocore::lutils::PlotResiduals(TString pName, RooPlot * pFrame, const RooAbsRealLValue* pVar, RooAbsPdf * pPDF,
                          TString pDir, bool normalize, bool plot_logy,
                          TLegend * label, bool plot_logx
                          ) {
	setStyle();
  gStyle->SetTitle(0);
  
  // some global definitions
  double pad_border       = 0.02;
  double pad_relysplit    = 0.3;
  double left_margin      = 0.16;
  double top_label_size   = 0.06;
  double top_title_offset = 1.2;
  double title2label_size_ratio = 1.1;
  
  // derived definitions
  double pad_ysplit     = (1.0-2.*pad_border)*pad_relysplit;
  double bottom_label_size = top_label_size*(1.-pad_relysplit)/pad_relysplit;
  double bottom_title_offset = top_title_offset/(1.-pad_relysplit)*pad_relysplit;
  
  double plot_min = pFrame->GetXaxis()->GetXmin();
  double plot_max = pFrame->GetXaxis()->GetXmax();
  
  TCanvas c1("c1","c1",900,900);
  c1.Divide(1,2);
  
  TPad* pad = (TPad*)c1.cd(1);
  if (label) {
		label->Draw();
	}
  if(plot_logy){
    pad->SetLogy(1);
  }
  if (plot_logx) {
    pad->SetLogx(1);
  }
  pad->SetPad(pad_border,pad_ysplit,1.-pad_border,1.-pad_border);
  pad->SetLeftMargin(left_margin);
  pad->SetBottomMargin(0.);
  
	RooHist* resid = pFrame->residHist(0,0,normalize);
  pFrame->SetLabelSize(0.0,"x");
  pFrame->SetLabelSize(top_label_size,"y");
  pFrame->SetTitleSize(top_label_size*title2label_size_ratio,"y");
  pFrame->SetXTitle("");
  pFrame->GetYaxis()->SetTitleOffset(top_title_offset);
  //pFrame->SetMaximum(1.3*pFrame->GetMaximum());
  
	pFrame->Draw();
  
  // lower frame with residuals plot
  pad = (TPad*)c1.cd(2);
  pad->SetPad(pad_border,pad_border,1.-pad_border,pad_ysplit);
  pad->SetLeftMargin(left_margin);
  pad->SetTopMargin(0.);
  pad->SetBottomMargin(0.4);
  if (plot_logx) {
    pad->SetLogx(1);
  }
  RooPlot * residFrame = pVar->frame(RooFit::Title("Residuals"));
  
  residFrame->GetXaxis()->SetLimits(plot_min,plot_max);
  
  residFrame->addPlotable(resid, "P");
  residFrame->SetAxisRange(-5.8,5.8,"Y");
  
  residFrame->SetTitle("");
  if (normalize){
    residFrame->SetYTitle("Pull");
  } else {
    residFrame->SetYTitle("Residual");
  }
  residFrame->SetLabelSize(bottom_label_size, "xy");
  residFrame->SetTitleSize(bottom_label_size*title2label_size_ratio, "xy");
  residFrame->GetYaxis()->SetTitleOffset(bottom_title_offset);
  residFrame->GetYaxis()->SetNdivisions(5,5,0);
  residFrame->Draw();
  
  pad = (TPad*)c1.cd(0);
	if (label) {
  	label->Draw();
	}
  
  printPlot(&c1, pName, pDir);
  
  // residFrame will also delete resid, as it is owned after RooPlot::addPlotable(...)
  delete residFrame;
}

std::pair<double,double> doocore::lutils::MedianLimitsForTuple(const RooDataSet& dataset, std::string var_name) {
  int num_entries = dataset.numEntries();
  std::vector<double> entries;
  
  // convert entries into vector (for sorting)
  const RooArgSet* args = NULL;
  for (int i = 0; i < num_entries; ++i) {
    args = dataset.get(i);
    if (isfinite(dynamic_cast<RooRealVar*>(args->find(var_name.c_str()))->getVal())) {
      entries.push_back(dynamic_cast<RooRealVar*>(args->find(var_name.c_str()))->getVal());
    }
  }
  
//  sdebug << "#non-finite entries neglected: " << num_entries-entries.size() << endmsg;
  
  std::sort(entries.begin(), entries.end());
  int idx_median = entries.size()/2;       
  
//  for (int i = 0; i < entries.size(); ++i) {
//    sdebug << entries[i] << endmsg;
//  }
  
  std::pair<double, double> minmax;
  
  minmax.first  = -4*entries[idx_median]+5*entries[(int)(idx_median*0.32)];
  minmax.second = -4*entries[idx_median]+5*entries[(int)(entries.size()-idx_median*0.32)];
  
//  sdebug << "idx_median = " << idx_median << ", entries[idx_median] = " << entries[idx_median] << endmsg;
//  sdebug << "(int)(idx_median*0.32) = " << (int)(idx_median*0.32) << endmsg;
//  sdebug << "(int)(entries.size()-idx_median*0.32) = " << (int)(entries.size()-idx_median*0.32) << endmsg;
//  sdebug << "-4*entries[idx_median] = " << -4*entries[idx_median] << endmsg;
//  sdebug << "5*entries[(int)(idx_median*0.32)] = " << 5*entries[(int)(idx_median*0.32)] << endmsg;
//  sdebug << "5*entries[(int)(entries.size()-idx_median*0.32)] = " << 5*entries[(int)(entries.size()-idx_median*0.32)] << endmsg;
//  sdebug << "first: " << minmax.first << endmsg;
//  sdebug << "second: " << minmax.second << endmsg;
  
  // if computed range is larger than min/max value choose those
  if (minmax.first < entries.front()){
  	minmax.first = entries.front();
  }
  if (minmax.second > entries.back()){
  	minmax.second = entries.back();
  }
  
  if (minmax.first >= minmax.second) {
    minmax.first  = entries[idx_median]*(minmax.first  > 0 ? 0.98 : 1.02);
    minmax.second = entries[idx_median]*(minmax.second > 0 ? 1.02 : 0.98);
  }
  
  // if everything fails, just take all
  if (minmax.first == 0 && minmax.second == 0) {
    minmax.first  = entries[0]-0.1*(entries[entries.size()-1]-entries[0]);
    minmax.second = entries[num_entries-1]+0.1*(entries[entries.size()-1]-entries[0]);
  }
  
  // if still empty range, go from -1 to +1
  if (minmax.first == 0 && minmax.second == 0) {
    minmax.first  = -1;
    minmax.second = +1;
  }

//  sdebug << "first: " << minmax.first << endmsg;
//  sdebug << "second: " << minmax.second << endmsg;
  
  return minmax;
}

std::pair<double,double> doocore::lutils::MedianLimitsForTuple(TTree& tree, std::string var_name) {
  int num_entries = tree.GetEntries();
  std::vector<double> entries;
  
  // convert entries into vector (for sorting)
  float entry;
  tree.SetBranchAddress(TString(var_name), &entry);
  for (int i = 0; i < num_entries; ++i)
  {
    tree.GetEvent(i);
    if (isfinite(entry)) {
      entries.push_back(entry);
    }
  }
  std::sort(entries.begin(), entries.end());

  int idx_median = entries.size()/2;       
  std::pair<double, double> minmax;
  
  minmax.first  = -4*entries[idx_median]+5*entries[(int)(idx_median*0.32)];
  minmax.second = -4*entries[idx_median]+5*entries[(int)(entries.size()-idx_median*0.32)];

  // if computed range is larger than min/max value choose those
  if (minmax.first < entries.front()){
  	minmax.first = entries.front();
  }
  if (minmax.second > entries.back()){
  	minmax.second = entries.back();
  }

  if (minmax.first >= minmax.second) {
    minmax.first  = entries[idx_median]*(minmax.first  > 0 ? 0.98 : 1.02);
    minmax.second = entries[idx_median]*(minmax.second > 0 ? 1.02 : 0.98);
  }
  
  // if everything fails, just take all
  if (minmax.first == 0 && minmax.second == 0) {
    minmax.first  = entries[0]-0.1*(entries[entries.size()-1]-entries[0]);
    minmax.second = entries[entries.size()-1]+0.1*(entries[entries.size()-1]-entries[0]);
  }
  
  // if still empty range, go from -1 to +1
  if (minmax.first == 0 && minmax.second == 0) {
    minmax.first  = -1;
    minmax.second = +1;
  }

  return minmax;
}

void doocore::lutils::plotAsymmetry(TString pPlotName, TTree * pTuple, TString pVarTime, TString pVarMix, int pBins, double pRngMin, double pRngMax, TString pTimeUnit) {
        doocore::lutils::setStyle();
        TCanvas c("c","c",800,600);

        int nBins = pBins;
        double rngMax = pRngMax;
        double rngMin = pRngMin;

        TTree *tree = pTuple;

        //get histograms for time distributions of (non)oszilated B0 candidates from datafile
        TH1D * hUpOsz = new TH1D("hUpOs",TString("MagUp oszilated;time (ns);nr. of candidates/")+Form("%f",(rngMax-rngMin)/(double)nBins) + pTimeUnit,nBins,rngMin,rngMax);
        tree->Draw(pVarTime + ">>hUpOs",        "(" + pVarTime + " > -1) & (" + pVarMix + " == -1)");

        TH1D * hUpNos = new TH1D("hUpNo",TString("MagUp non oszilated;time (ns);nr. of candidates/")+Form("%f",(rngMax-rngMin)/(double)nBins) + pTimeUnit,nBins,rngMin,rngMax);
        tree->Draw(pVarTime + ">>hUpNo",        "(" + pVarTime + " > -1) & (" + pVarMix + " == 1)");

        //build histograms with N_notoszilated +/- N_oszilated
        TH1D * hUpSum = (TH1D*)hUpNos->Clone("hUpSum");
        TH1D * hUpDif = (TH1D*)hUpNos->Clone("hUpDif");

        hUpSum->Add(hUpOsz);
        hUpDif->Add(hUpOsz,-1);

        //build asymmetry histogram
        TH1D * hUpAsy = (TH1D*)hUpDif->Clone("hUpAsy");
        hUpAsy->Divide(hUpSum);


        hUpAsy->Draw();

        doocore::lutils::printPlot(&c,pPlotName,"Plot");

        delete hUpOsz;
        delete hUpNos;
        delete hUpSum;
        delete hUpDif;
        delete hUpAsy;
}
  

