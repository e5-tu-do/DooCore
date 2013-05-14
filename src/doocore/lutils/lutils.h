#ifndef DOOCORE_LUTILS_LUTILS_H
#define DOOCORE_LUTILS_LUTILS_H

// from STL
#include <queue>

// from BOOST
#include <boost/thread.hpp>

// from ROOT
#include "TString.h"
#include "TLatex.h"
#include "TMath.h"
#include "TMatrixD.h" 

// from project
#include "doocore/io/MsgStream.h"

// forward declarations
class TH1;
class TH2;
class TH1D;
class TH2D;
class TF1;
class TCanvas;
class TPad;
class TTree;
class TFile;
class TLegend;

class RooAbsPdf;
class RooPlot;
class RooAbsRealLValue;
class RooDataSet;
class RooArgSet;

/**
 * @namespace doocore::lutils
 * @brief Legacy utils for DooSoftware core functionality
 * 
 * This is the jumble of all utils functions that came to us back in the 
 * inclusive phi days (and everythin added simce then). It might be a good idea
 * to move the functionality into better grouped namespaces/libraries and let
 * this one die for good.
 */
namespace doocore {
namespace lutils {
/** @class concurrent_queue
 *  @brief Thread-safe concurrent queue for pointers 
 *
 *  This is a thread-safe queue able for the producer/consumer pattern of a 
 *  thread adding objects to this and another consuming these. Utilising 
 *  std::queue it's restrictions apply.
 *
 *  Adapted from http://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
 */
template<typename Data>
class concurrent_queue
{
private:
  std::queue<Data> the_queue;
  mutable boost::mutex the_mutex;
  boost::condition_variable the_condition_variable;
  boost::condition_variable the_condition_variable_popped;
  int max_size_;
public:
  concurrent_queue(int max_size=-1) : max_size_(max_size) {}
  
  void push(const Data& data) {
    boost::mutex::scoped_lock lock(the_mutex);
    
    while (max_size_ > 0 && the_queue.size() >= max_size_) {
      the_condition_variable_popped.wait(lock);
    }
    
    the_queue.push(data);
    lock.unlock();
    the_condition_variable.notify_one();
  }
  
  bool empty() const {
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.empty();
  }
  
  void disable_queue() {
    the_condition_variable.notify_one();
  }
  
  bool wait_and_pop(Data& popped_value) {
    boost::mutex::scoped_lock lock(the_mutex);
    bool locked = true;
    if (the_queue.empty()) {
      locked = the_condition_variable.timed_wait(lock, boost::posix_time::seconds(1));
    }
    
    if (locked && !the_queue.empty()) {
      popped_value=the_queue.front();
      the_queue.pop();
      the_condition_variable_popped.notify_one();
      return true;
    } else {
      return false;
    }
  }
  
  int size() {
    boost::mutex::scoped_lock lock(the_mutex);
    return the_queue.size();
  }
};

const int kLHCbFont = 132;
  
//	
// plots
//
void setStyle(TString option="LHCb");
void setHotColdPalette(TH2* h=0);
void setRedBluePalette(TH2* h=0);

void printPlotOpenStack(TCanvas* c, TString name, TString dir="");
void printPlot(TCanvas* c, TString name, TString dir="");
void printPlotCloseStack(TCanvas* c, TString name, TString dir="");

void drawDate(float x1=0.10, float y1=0.02, float x2=0.5, float y2=0.1);	
void drawOrdered(TH1* h1, TH1* h2, TH1* h3=0, TH1* h4=0);
void drawNormalizedOrdered(TH1* h1, TH1* h2, TH1* h3=0, TH1* h4=0);
void drawNormalizedOrdered(std::vector<TH1*> hists);

void addEtaPtLabels(TH2D* h);

/**
 *  @brief Plot simple RooPlot frame with correct formatting
 *
 *  For a given RooPlot, this function will plot it and save it to common 
 *  formats in files. 
 *
 *  @param pName Name for the output file (e.g. "mass" results in files "mass.pdf", "mass.eps" and so on
 *  @param pFrame RooPlot to plot
 *  @param label Label to plot in the plots (can be TLatex(1.,1.,"") for no label)
 *  @param pDir Directory to put plots in (will be created on demand)
 *  @param plot_logy whether to plot with log scale on y axis
 *  @param plot_logx whether to plot with log scale on x axis
 */
void PlotSimple(TString pName, RooPlot * pFrame, TLatex& label, TString pDir = "", bool plot_logy = false, bool plot_logx = false);

/**
 *  @brief (DEPRECATED:) Plot simple RooPlot frame
 *
 *  Just a compatibility wrapper for the other PlotSimple function.
 */
void PlotSimple(TString pName, RooPlot * pFrame, TString pDir, bool plot_logy, TLatex& label, bool plot_logx = false);
  
///Do a run test as in http://de.wikipedia.org/wiki/Run-Test
double RunTest(const TH1 & hist);
///Returns a residual HISTOGRAM and no fucking RooHist TGraph for the given RooPlot
TH1D 		GetPulls(RooPlot * pFrame, bool normalize = true);
///Prepare canvas with two pads for pull and residual plots, returns numbers for text formatting
void PreparePadForPulls(TCanvas * c1, RooPlot * pFrame, bool plot_logx, bool plot_logy,
												double & top_label_size, double & top_title_offset, double & title2label_size_ratio,
												double & bottom_label_size, double & bottom_title_offset);

/**
 *  @brief Create a plot of the pull distribution and fit a Gaussian to that
 *
 *  Due to ROOT limitations, all objects created inside this function need to 
 *  persist until the Canvas is saved completely. Therefore, these objects are
 *  created on the heap and returned via the pointers f_gauss_norm through 
 *  legend. The caller is responsible of deleting these.
 *
 *  @param pulls pulls vs. bins of fit
 *  @param pad TPad to draw on
 *  @param f_gauss_norm normal Gaussian PDF to draw (will be created)
 *  @param f_gauss_fit fitted Gaussian PDF to draw (will be created)
 *  @param h_pulls histogram with pull distribution (will be created)
 *  @param h_error error band of Gaussian fit (will be created)
 *  @param legend legend with information (will be created)
 */
void PlotPullDistributionWithGaussian(const TH1& pulls, TPad& pad, TF1* f_gauss_norm, TF1* f_gauss_fit, TH1* h_pulls, TH1* h_error, TLegend* legend);

  
/**
 *  Plot a distribution histogram of the pulls overlaid with a Gaussian
 */
void PlotGauss(TString pName, const TH1 & pulls, TString pDir = "");

///Construct and plot a pull histogram beneath a RooPlot. The last added dataset and curve are used to calculate the pulls.
///The last bool defines a grey (true) or colored (false) scheme for the pull fill color.

/**
 *  @brief Plot a RooPlot frame with pulls underneath
 *
 *  If unsure, you probably want to use this version.
 */
void PlotPulls(TString pName, RooPlot * pFrame, TLatex& label,
               TString pDir = "", bool plot_logy = false,
               bool plot_logx = false, bool greyscale = true,
               std::string gauss_suffix="_Gauss");

/**
 *  Overloaded version for compatibility with Legends (no idea how to solve this nicely Tobi 2013-04-17)
 */
void PlotPulls(TString pName, RooPlot * pFrame, TString pDir = "",
               bool plot_logy = false, bool plot_logx = false,
               bool greyscale = true, TLegend * label = NULL,
               std::string gauss_suffix="_Gauss");
  
/**
 *  @brief (DEPRECATED:) Plot RooPlot frame with pull distribution
 *
 *  Just a compatibility wrapper for the other PlotPull functions.
 */  
void PlotPulls(TString pName, RooPlot * pFrame, const RooAbsRealLValue* pVar,
               RooAbsPdf * pPDF, TLatex& label, TString pDir = "",
               bool normalize_residuals = true, bool plot_logy = false,
               bool plot_logx = false, std::string gauss_suffix="_Gauss");

/**
 *  @brief (DEPRECATED:) Plot RooPlot frame with pull distribution
 *
 *  Just a compatibility wrapper for the other PlotPull functions.
 */
void PlotPulls(TString pName, RooPlot * pFrame, TString pDir, bool plot_logy, bool plot_logx, bool greyscale, TLatex& label, std::string gauss_suffix="_Gauss");
  
/**
 *  @brief (DEPRECATED:) Plot RooPlot frame with pull distribution
 *
 *  Just a compatibility wrapper for the other PlotPull functions.
 */
void PlotPulls(TString pName, RooPlot * pFrame, const RooAbsRealLValue* pVar, RooAbsPdf * pPDF, TString pDir, bool plot_logy, bool plot_logx, bool greyscale, TLatex& label, std::string gauss_suffix="_Gauss");
  
void PlotResiduals(TString pName, RooPlot * pFrame, const RooAbsRealLValue * pVar, RooAbsPdf * pPDF, TLatex& label, TString pDir = "", bool normalize_residuals = true, bool plot_logy = false, bool plot_logx = false);

void PlotResiduals(TString pName, RooPlot * pFrame, const RooAbsRealLValue* pVar, RooAbsPdf * pPDF, TString pDir, bool normalize_residuals, bool plot_logy, TLegend * label = NULL, bool plot_logx = false);
  
/**
 *  @brief (DEPRECATED:) Plot RooPlot frame with old residuals distribution
 *
 *  Just a compatibility wrapper for the other PlotResiduals functions.
 */
void PlotResiduals(TString pName, RooPlot * pFrame, const RooAbsRealLValue * pVar, RooAbsPdf * pPDF, TString pDir, bool normalize_residuals, bool plot_logy, TLatex& label, bool plot_logx = false);
  
///Do an Asymmetry Plot for a given NTuple the name of the time variable and a variable name that is used for a cut (+/-1) to separate two mixing states
void plotAsymmetry(TString pPlotName, TTree * pTuple, TString pVarTime, TString pVarMix, int pBins = 20, double pRngMax = 0.01, double pRngMin = 0.00, TString pTimeUnit = "ns");
//
// stuff
//
bool		fileExists(TString strFilename);
int		  fileNLines(TString strFilename);
void 		printSystemRecources(TString cmd);
/// \brief Sleep, i.e. halt everything.
/// \param sleep_time Time to sleep in seconds.
void    Sleep(double sleep_time);

//
// math
//
double		Round(double value, unsigned int digits);
Double_t	Covariance(Int_t n, Double_t xdata[], Double_t ydata[]);
Double_t	Variance(Int_t n, Double_t data[]);
inline double	Sq(double value){return pow(value, 2);}
inline double	Max(double v1, double v2){return v1>v2 ? v1 : v2;}
inline double	RadToDeg(double rad){return rad*180.0/TMath::Pi();}
inline double	DegToRad(double deg){return deg/180.0*TMath::Pi();}
void		Symmetrize(TMatrixD &m);

/**
 *  @brief Median limits for tuple (here: RooDataSet)
 *
 *  This function will evaluate the distribution of the variable with name 
 *  var_name in the dataset. It will return a pair as (min, max) as plotting
 *  range for the distribution neglecting outliers by a quantile based 
 *  approach.
 *
 *  @param dataset RooDataSet to evaluate
 *  @param var_name name of variable in dataset to evaluate
 *  @return pair of (double,double) as (min,max) to use for plotting
 */
std::pair<double,double> MedianLimitsForTuple(const RooDataSet& dataset, std::string var_name);

/**
 *  @brief Median limits for tuple (here: TTree)
 *
 *  This function will evaluate the distribution of the variable with name 
 *  var_name in the tree. It will return a pair as (min, max) as plotting
 *  range for the distribution neglecting outliers by a quantile based 
 *  approach.
 *
 *  @param dataset RooDataSet to evaluate
 *  @param var_name name of variable in dataset to evaluate
 *  @return pair of (double,double) as (min,max) to use for plotting
 */
std::pair<double,double> MedianLimitsForTuple(TTree& tree, std::string var_name); 

} // namespace lutils
} // namespace doocore

#endif // DOOCORE_LUTILS_LUTILS_H
